/* ------------------------------
	ABAKARIM MAROUANE
	AFRASS ILIAS
   
*/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include "hardware_ini.h"
#include "drive.h"
#include "tools.h"


#include "mbr.h"
#include "superbloc.h"

unsigned current_volume;

/* load super bloc of the $CURRENT_VOLUME
   set current_volume accordingly */
static void
load_current_volume ()
{
    char* current_volume_str;
    int status;
    
    current_volume_str = getenv("CURRENT_VOLUME");
    ffatal(current_volume_str != NULL, "no definition of $CURRENT_VOLUME"); 

    errno = 0;
    current_volume = strtol(current_volume_str, NULL, 10);
    ffatal(!errno, "bad value of $CURRENT_VOLUME %s", current_volume_str);
    
    status = load_super(current_volume);
    ffatal(!status, "unable to load super of vol %d", current_volume);
}

/* return hw_config filename */
static char *
get_hw_config ()
{
    char* hw_config;

    hw_config = getenv("HW_CONFIG");
    return hw_config ? hw_config : HW_CONFIG;
}

static void
emptyIT()
{
    return;
}

/* ------------------------------
   Initialization and finalization fucntions
   ------------------------------------------------------------*/
void
mount()
{
    char *hw_config;
    int status, i; 

    /* Hardware initialization */
    hw_config = get_hw_config();
    status = init_hardware(hw_config);
    ffatal(status, "error in hardware initialization with %s\n", hw_config);

    /* Interrupt handlers */
    for(i=0; i<16; i++)
	IRQVECTOR[i] = emptyIT;
    
    /* Allows all IT */
    _mask(1);

    /* Load MBR and current volume */
    read_mbr();
    load_current_volume();
}

void
umount()
{
    /* save current volume super bloc */
    save_super();

    /* bye */
}
 

