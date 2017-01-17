#ifndef TP2_SYSTEMES_FICHIER_H_INCLUDED
#define TP2_SYSTEMES_FICHIER_H_INCLUDED

#define size_sector 256
#define MAX_VOL 8
#define MAGIX 0x55AA

typedef enum{base,annexe,autre}type_vol;
struct vol_s
{
    int f_sec;
    int f_cyl;
    type_vol type;
    int nbBloc;

};

struct MBR_s mbr_cu;

struct MBR_s
{
    struct vol_s vol[MAX_VAL];
    int nbVol;
    int magic;
};

#endif // TP2_SYSTEMES_FICHIER_H_INCLUDED
