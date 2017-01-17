#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<assert.h>

#include "hardware.h"





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
    _out(0x3F6,0x04)
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

void load_mbr(struct MBR_s *mbr)
{

    assert(sizeof(MBR_s) <= size_sector);
    read_sector(0,0,&mbr_cu,sizeof(mbr));
    return mbr->magic = MAGIC;

}

void save_mbr()
{
    write_sector(0,0,&mbr,sizeof(mbr));
}
int transformer_cyl(int num_vol,int num_bloc)
{
    return mbr.vol[nul_vol]->f_cyl+(mbr_cu.vol[num_vol]->f_sec+num_bloc)/MAX_SEC;

}

void transformer_sec(int num_vol,int num_bloc)
{
    return (mbr_cu.vol[num_vol]->f_sec+num_bloc)%MAX_SEC;

}

void read_bloc(unsigned int vol,unsigned int nbloc,unsigned char* buffer)
{
    int cyl = transformer_cyl(mbr_cu.vol[vol]->f_cyl,nbloc);
    int sec = transformer_sec(mbr_cu.vol[vol]->f_sec,nbloc);

        read_sector(cyl,sec,buffer);
}
void write_bloc(unsigned int vol,unsigned int nbloc,unsigned char* buffer)
{
    int cyl = transformer_cyl(mbr_cu.vol[vol]->f_cyl,nbloc);
    int sec = transformer_sec(mbr_cu.vol[vol]->f_sec,nbloc);

        write_sector(cyl,sec,buffer);
}

void write_bloc(unsigned int vol)
{
    int cyl = transformer_cyl(mbr_cu.vol[vol]->f_cyl,nbloc);
    int sec = transformer_sec(mbr_cu.vol[vol]->f_sec,nbloc);

        formater_sector(cyl,sec,mbr_cu.vol[vol]->nbBloc,0);
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


    return 0;
}
