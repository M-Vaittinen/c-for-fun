#define GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <termio.h>
#include <sgtty.h>
#include <sys/select.h>
#include <sys/types.h>
//#include <fcntl.h>
//#include <sys/ioctl.h>
//#include <linux/kd.h>
#include "areena.h"


#define YLOS_NUOLI 0x415b1b
#define ALAS_NUOLI 0x425b1b
#define VAS_NUOLI 0x445b1b
#define OIK_NUOLI 0x435b1b
#define SUUNTA_OIKEA 1
#define SUUNTA_VASEN -1
#define AUTOMERKKI 'H'
#define TIEMERKKI_VAS '#'
#define TIEMERKKI_OIK '#'

#define NAME_MAX 1024
#define TALL_TIEDOSTO "scores.txt"
#define HISCORE_AMNT 5


typedef struct estestats
{
    int nopeus;
    int piste;
    int este;
}estestats;


typedef struct hiscore
{
    int used;
    char name[NAME_MAX];
    unsigned int max_speed;
    unsigned int points;
    estestats stats;
}hiscore;


void read_scores()
{
    FILE *rf;
    int ptmp,stmp,st_etmp,st_ntmp,st_ptmp,rval;
    //char name[1024];
    char *name;
    memset(fivebests,0,sizeof(fivebests));
    rf=fopen(TALL_TIEDOSTO,"r");
    if(rf)
    {
        int i=0;
        for(i=0; i< HISCORE_AMNT &&6==(rval=fscanf(rf,"%m[^!]!%u!%u!%d!%d!%d\n",&name,&stmp,&ptmp,&st_etmp,&st_ntmp,&st_ptmp));i++)
        {
        //    printf("Scanf returned %d, name=%s\n",rval,name);
            strncpy(fivebests[i].name,name,1024);
            fivebests[i].name[sizeof(fivebests[i].name)-1]='\0';
            fivebests[i].max_speed=stmp;
            fivebests[i].points=ptmp;
            fivebests[i].stats.este=st_etmp;
            fivebests[i].stats.nopeus=st_ntmp;
            fivebests[i].stats.piste=st_ptmp;
            fivebests[i].used=1;
            free(name);
        }
        //printf("Scanf returned %d, name=%s\n",rval,name);
        fclose(rf);
    }
}

int write_scores()
{
    FILE *wf;
    wf=fopen(TALL_TIEDOSTO,"w");
    if(wf)
    {
        int i=0;
       for(i=0; i<HISCORE_AMNT && fivebests[i].used; i++)
        {
            fprintf
            (
                wf,
                "%s!%u!%u!%d!%d!%d\n",
                fivebests[i].name,
                fivebests[i].max_speed,
                fivebests[i].points,
                fivebests[i].stats.este,
                fivebests[i].stats.nopeus,
                fivebests[i].stats.piste
            );
        }
       fclose(wf);
    }
    else
        return errno;
    return 0;
}

void update_fivebests(struct areena *ar, char *name)
{
    int i;
//    printf("Vanhat Huippupisteet:\n");
//    print_fivebests();
    for(i=0;i<5&&fivebests[i].used;i++)
    {
        if(fivebests[i].points<ar->pisteet)
            break;
    }
    if(i==5)
        return;
    if(i<4)
    {
        memmove(&(fivebests[i+1]),&(fivebests[i]),sizeof(fivebests[i])*(4-i));
    }
    if(i<5)
    {
        strcpy(fivebests[i].name,name);
//        fivebests[i].max_speed=pi->nopeus;
        fivebests[i].points=ar->pisteet;
        fivebests[i].used=1;
//        fivebests[i].stats.este=pi->stats.este;
//        fivebests[i].stats.nopeus=pi->stats.nopeus;
//        fivebests[i].stats.piste=pi->stats.piste;
    }
    write_scores();
}

