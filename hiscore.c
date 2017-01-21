#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "hiscore.h"

#define TALL_TIEDOSTO "scores.txt"

void print_fivebests(hiscore *fivebests,int hcamnt)
{
    int i;
    printf("Best Of The Best:\n\n");
    for(i=0;i<hcamnt&&fivebests[i].used;i++)
    {
        printf("%d:\t%s: Speed %u, Points %u \t\t#(%d) V(%d) $(%d)\n",
                i+1,
                fivebests[i].name,
                fivebests[i].max_speed,
                fivebests[i].points,
                fivebests[i].stats.este,
                fivebests[i].stats.nopeus,
                fivebests[i].stats.piste
              );
    }
}

void read_scores(hiscore *fivebests,int hcamnt)
{
    FILE *rf;
    int ptmp,stmp,st_etmp,st_ntmp,st_ptmp,rval;
    char name[1024];
    memset(fivebests,0,sizeof(hiscore)*hcamnt);
    rf=fopen(TALL_TIEDOSTO,"r");
    if(rf)
    {
        int i=0;
        for(i=0;i<hcamnt&&6==(rval=fscanf(rf,"%1024[^!]!%u!%u!%d!%d!%d\n",name,&stmp,&ptmp,&st_etmp,&st_ntmp,&st_ptmp));i++)
        {
            strncpy(fivebests[i].name,name,1024);
            fivebests[i].name[sizeof(fivebests[i].name)-1]='\0';
            fivebests[i].max_speed=stmp;
            fivebests[i].points=ptmp;
            fivebests[i].stats.este=st_etmp;
            fivebests[i].stats.nopeus=st_ntmp;
            fivebests[i].stats.piste=st_ptmp;
            fivebests[i].used=1;
        }
        fclose(rf);
    }
}

int write_scores(hiscore *fivebests,int hcamnt)
{
    FILE *wf;
    wf=fopen(TALL_TIEDOSTO,"w");
    if(wf)
    {
        int i=0;
        for(i=0;i<hcamnt&&fivebests[i].used;i++)
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

void update_fivebests(peliinfo *pi,char *name,hiscore *fivebests,int hcamnt)
{
    int i;
    for(i=0;i<hcamnt&&fivebests[i].used;i++)
    {
        if(fivebests[i].points<pi->pisteet)
            break;
    }
    if(i==hcamnt)
        return;
    if(i<hcamnt-1)
    {
        memmove(&(fivebests[i+1]),&(fivebests[i]),sizeof(fivebests[i])*((hcamnt-1)-i));
    }
    if(i<hcamnt)
    {
        strcpy(fivebests[i].name,name);
        fivebests[i].max_speed=pi->nopeus;
        fivebests[i].points=pi->pisteet;
        fivebests[i].used=1;
        fivebests[i].stats.este=pi->stats.este;
        fivebests[i].stats.nopeus=pi->stats.nopeus;
        fivebests[i].stats.piste=pi->stats.piste;
    }
    write_scores(fivebests,hcamnt);
}
