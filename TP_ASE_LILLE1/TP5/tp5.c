#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<assert.h>

#include "tp5.h"
#include "hardware.h"

#define size_sector 256


void SEEK(unsigned int piste,unsigned int sector)
{

    _out(0x110,(cyl>>8)&0xFF);
    _out(0x111,cyl); 
    _out(0x112,(sector>>8)&0xFF);
    _out(0x113,sector&0xFF);
    _out(0x3F6,0x04);
    sleep(14); 
}

void read_sector(unsigned int pi,unsigned int sec,unsigned char *buffer)
{
    SEEK(pi,sec); 
    _out(0x110,0); 
    _out(0x110,1);
    _out(0x3F6,0x04);
    sleep(14); 

    for(int i=0,i<size_sector ;i++)
    {
        buffer[i] = MASTERBUFFER[i];
    }
}

void write_sector(unsigned int cyl,unsigned int sec,unsigned char buffer)
{

    SEEK(cyl,sec);
    _out(0x110,0);
    _out(0x111,1);
    _out(0x3F6,0x06);
    sleep(14);

    for(int i=0;i<size_sector;i++)
    {
        MASTERBUFFER[i] = buffer[i];
    }

}

void formater_un_sector(int cyl,int sec,unsigned int val)
{

    SEEK(cyl,sec);
    _out(0x110,0);
    _out(0x111,1);
    _out(0x112,(val>>8)&&0xFF);
    _out(0x113,val);
    _out(0x3F6,0x08);
    sleep(14);

}

void formater_sector(int cyl,int sec,unsigned nbsector,unsigned int val)
{

   for(int i=sector,i<sector+nbsector;i++)
   {
       formater_sector(cyl,i,val);
   }

}

void formater_disque()
{
    for(int i=0;i<size_cylidre;i++)
    {
        for(int j=0;j<max_sector;i++)
            format_sector(i,j,0);
    }
}



int main()
{
        int sec,cyl;
    printf("entre le cylindre et le sector \n");
    scanf("%d",&cyl);
    scanf("%d",&sec);

    if(!init_hardware("hardware.ini"))
        printf("erreur de initialiser le materiél");

    for(int i=0;i<15;i++)
        IRQVECTOR[i] = empty_rq();

    sleep(0);
	
	SEEK(cyl,sec);
	
	for(int i=0;i<size_sector;i++)
	{
		printf(MASTERBUFFER[i]);
		if(i%16 == 15)
			printf("\n");
	}	

    return 0;
}
