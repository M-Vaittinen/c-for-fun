#define GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <termio.h>
#ifndef  __CYGWIN__
	#include <sgtty.h>
#endif
#include <sys/select.h>
#include <sys/types.h>

#include "estestats.h"
#include "hiscore.h"
#include "este.h"
#include "tienpala.h"
#include "peliinfo.h"

#define F1 0x504f1b
#define YLOS_NUOLI 0x415b1b
#define ALAS_NUOLI 0x425b1b
#define VAS_NUOLI 0x445b1b
#define OIK_NUOLI 0x435b1b
#define SUUNTA_OIKEA 1
#define SUUNTA_VASEN -1
#define AUTOMERKKI 'H'
#define TIEMERKKI_VAS '#'
#define TIEMERKKI_OIK '#'
#define GUN_RELOAD 1

#define HISCORE_AMNT 5

#define BG_MAGNETA    "\x1B[45m"
#define BG_RED    "\x1B[41m"
#define COLOR_RESTORE "\x1B[0m"
#define FG_WHITE      "\x1B[37m"
#define FG_GREEN      "\x1B[32m"
#define FG_BLUE       "\x1B[34m"

static int TIEN_PITUUS=22;
/*static const char *RED_START="\[\033[31m\]" __attribute__((unused));
static const char *GREEN_START="\[\033[32m\]";
static const char *BLUE_START="\[\033[34m\]";*/
//static const char *COLOR_END="\x1B[0m";



#define VARIN_ALKU_LEVEYS 5 /* start 5 bytes, end 4 bytes */
#define VARIN_LEVEYS (5+5) /* start 5 bytes, end 4 bytes */
//#define TIEN_MAX_LEVEYS 60
#define RADAN_LEVEYS 20
#define TIEN_MAX_LEVEYS 80
#define TIEN_MAX_LEVEYS_VAREINEEN (TIEN_MAX_LEVEYS+3*VARIN_LEVEYS/*esteet*/+VARIN_ALKU_LEVEYS/*taustaväri*/)
//7 for spaces and "speed", 7 for spaces and"points", 6 for points 7 for speed, one for NULL byte
#define GUESS_SPEED_SIZE  28

typedef enum extratexts
{
    EXTRAT_POINTS = 0,
    EXTRAT_SPEED,
    EXTRAT_LASER,
    EXTRA_AMNT
}extratexts;
#define EXTRA_MAX (EXTRA_AMNT-1)

const char *extratext_s[EXTRA_AMNT] =
{
    [EXTRAT_POINTS] = "+10000 Points!!!",
    [EXTRAT_SPEED] = "Super Speed!!!", 
    [EXTRAT_SPEED] = "Laser hit +5000!!!" 
};
typedef struct extratext
{
    const char *text;
}extratext;

struct extratext extra_array[1024];

static const char *vari_loppu=FG_WHITE;

static const char *vari_alut[este_last]=
{
    [este_speed]=FG_GREEN "\x1B[32m",
    [este_pisteet]=FG_BLUE "\x1B[34m",
};

   

void advance_extratext_ctr(peliinfo *pi)
{
    pi->extratext_array_current++;
}
void add_new_extratext(peliinfo *pi,extratexts extra_type)
{
    extratext *new=&(extra_array[(pi->extratext_array_current+TIEN_PITUUS-1)%1024]);
    new->text=extratext_s[extra_type];
}

const char *get_extratext_for_tienpala_index(peliinfo *pi,int tienpala_no)
{
    return extra_array[(pi->extratext_array_current+tienpala_no)%1024].text;
}

void clear_extratext(peliinfo *pi)
{
    extra_array[(pi->extratext_array_current)%1024].text=NULL;
}


void init_extra_array()
{
    memset(extra_array,0,sizeof(extra_array));
}

/*
void init_beeb_tty()
{
    ttyfd=open("/dev/tty10",O_RDONLY);
}

void beebbeeb(int duration, int freq)
{
    if(ttyfd!=-1)
    {
        if(-1==ioctl(ttyfd, KDMKTONE, duration<<16|(0x0000ffff&(1193180/freq))))
            printf("ioctl %s\n",strerror(errno));
    }
    else
        printf("tty not opened\n");
}
*/


int init_sizes(void);
int init_sizes()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    TIEN_PITUUS=w.ws_row-1;
    if(w.ws_col<TIEN_MAX_LEVEYS + GUESS_SPEED_SIZE)
       return -1;
   return 0; 
}
int hae_esteen_paikka(tienpala *tiep,int tukkoindex)
{
    int rval=tiep->tukko[tukkoindex].paikka;
    if(tiep->esteamnt>1)
    {
        int i;
        for(i=0;i<tiep->esteamnt;i++)
            if(i==tukkoindex)
                continue;
            else
                if(tiep->tukko[i].varileveys)
                    if(tiep->tukko[i].paikka<tiep->tukko[tukkoindex].paikka)
                        rval+=tiep->tukko[i].varileveys;
    }
    return rval;

}
int hae_oikea_tiepiste(tienpala *tiep)
{
    int i,rval=tiep->oik;
    for(i=0;i<tiep->esteamnt;i++)
        rval+=tiep->tukko[i].varileveys;
   return rval;
}
int hae_oikea_autonpaikka(tienpala *tiep,int autonpaikka)
{
    int rval=autonpaikka;
    if(tiep->esteamnt)
    {
        int i;
        for(i=0;i<tiep->esteamnt;i++)
            if(tiep->tukko[i].varileveys)
            {
                if(tiep->tukko[i].paikka<autonpaikka)
                    rval+=tiep->tukko[i].varileveys;
                else if(tiep->tukko[i].paikka==autonpaikka)
                    rval+=tiep->tukko[i].varialkuleveys;
            }
    }
    return rval;
}

void piirra_laser(char *tie, tienpala *tiep, int autonpaikka)
{
    if(tiep->vas < autonpaikka && tiep->oik > autonpaikka)
        tie[hae_oikea_autonpaikka(tiep, autonpaikka)] = '|';
}
void piirra_esteet(char *tie,tienpala *tiep)
{
    int i;
    int estepaikka;
    for(i=0;i<tiep->esteamnt && i<3;i++)
        if(tiep->tukko[i].type==este_block || tiep->tukko[i].type==este_speed || tiep->tukko[i].type==este_pisteet)
        {
            estepaikka=hae_esteen_paikka(tiep,i);
            if(!tiep->tukko[i].varileveys)
                tie[estepaikka]=tiep->tukko[i].merkki;
            else
            {
                memcpy(&(tie[estepaikka]),tiep->tukko[i].vari_alku,tiep->tukko[i].varialkuleveys);
                tie[estepaikka+tiep->tukko[i].varialkuleveys]=tiep->tukko[i].merkki;
                memcpy(&(tie[estepaikka+tiep->tukko[i].varialkuleveys+1]),tiep->tukko[i].vari_loppu,tiep->tukko[i].varileveys-tiep->tukko[i].varialkuleveys);
            }
        }
}
void hit_points_handler(peliinfo *pi)
{
    pi->pisteet+=10000;
    pi->stats.piste++;
    add_new_extratext(pi,EXTRAT_POINTS);
}
void hit_speed_handler(peliinfo *pi)
{
    pi->hidastus_saved=pi->hidastus;
    pi->hidastus-=50000;
    if(pi->hidastus<0)
        pi->hidastus=0;
    pi->speed_kierros=50;
    add_new_extratext(pi,EXTRAT_SPEED);
    pi->stats.nopeus++;
}
int tarkista_esteet(peliinfo *pi, tienpala *tiep)
{
    int i,rval=0;
    for(i=0;i<tiep->esteamnt && i<3;i++)
        if(tiep->tukko[i].paikka==pi->autonpaikka)
        {
            if(tiep->tukko[i].type==este_block)
            {
                rval= 1;
                pi->stats.este++;
            }
            else if(tiep->tukko[i].type==este_speed)
                hit_speed_handler(pi);
            else if(tiep->tukko[i].type==este_pisteet)
                hit_points_handler(pi);
        }
    return rval;
}
void lisaa_reunat(tienpala *tiep, char *tie)
{
    int tiepiste;
    memcpy(&tie[tiep->vas-VARIN_ALKU_LEVEYS],BG_RED,VARIN_ALKU_LEVEYS);
    tie[tiep->vas]=TIEMERKKI_VAS;
    tiepiste=hae_oikea_tiepiste(tiep);
    tie[tiepiste]=TIEMERKKI_OIK;
    tie[tiepiste+1]='\0';
}
int piirra_ylarivi(tienpala *tiep, peliinfo *pi, char *kuski)
{
    char tie[TIEN_MAX_LEVEYS_VAREINEEN+1];
    int rval=0;

    memset(tie,' ',sizeof(tie));
    lisaa_reunat(tiep, tie);

    tie[hae_oikea_autonpaikka(tiep,pi->autonpaikka)]=AUTOMERKKI;
    piirra_esteet(tie,tiep);
        

    if(pi->autonpaikka<=tiep->vas || pi->autonpaikka>=tiep->oik || tarkista_esteet(pi,tiep))
    {
        tie[hae_oikea_autonpaikka(tiep,pi->autonpaikka)]='X';
        rval=-1;
    }
    printf("%s" COLOR_RESTORE " Ajajana %s\n",tie,kuski);
    clear_extratext(pi);
    return rval;
}
void piirra_alarivi(peliinfo *pi,tienpala *tiep,unsigned int nopeus, unsigned int pisteet)
{
    char tie[TIEN_MAX_LEVEYS_VAREINEEN+1];
    memset(tie,' ',sizeof(tie));
    lisaa_reunat(tiep, tie);
    piirra_esteet(tie,tiep);
    if(pi->ammu==GUN_RELOAD)
        piirra_laser(tie,tiep,pi->autonpaikka);
    printf("%s" COLOR_RESTORE " speed %u points %u\n",tie,nopeus,pisteet);
}
void piirra_rivi(peliinfo *pi,tienpala *tiep, const char *et)
{
    char tie[TIEN_MAX_LEVEYS_VAREINEEN+1];
    memset(tie,' ',sizeof(tie));

    lisaa_reunat(tiep, tie);
    piirra_esteet(tie,tiep);
    if(pi->ammu==GUN_RELOAD)
        piirra_laser(tie,tiep,pi->autonpaikka);
    if(et)
        printf("%s" COLOR_RESTORE " %s\n",tie,et);
    else
        printf("%s" COLOR_RESTORE "\n",tie);
}

int piirra_ja_tarkista(peliinfo *pi, char *kuski)
{
    int rval=0;
    int i;
 
    rval=piirra_ylarivi(&(pi->tie[0]),pi,kuski);
    for(i=1;i<TIEN_PITUUS-1;i++)
        piirra_rivi
        (
            pi,
            &(pi->tie[i]),
            get_extratext_for_tienpala_index(pi,i)
        );
    piirra_alarivi(pi,&(pi->tie[i]),pi->nopeus,pi->pisteet);

    fflush(stdout);
    if(rval)
        sleep(2);
    return rval;
}

void lopeta(peliinfo *pi, char *nimi,hiscore *fivebests,int hcamnt)
{
    int i;
    for(i=0;i<TIEN_PITUUS;i++)
        printf("\n");
    if(nimi)
        printf("Olipa hauska ajella vaikka meninkin päin seinää - Pisteesi %s: %u\n",nimi,(unsigned)pi->pisteet);
    else
        printf("Olipa hauska ajella vaikka meninkin päin seinää - pisteet %u\n",(unsigned)pi->pisteet);
    if(!pi->huijaukset.huijaus)
    {
        update_fivebests(pi,nimi,fivebests,hcamnt);
        print_fivebests(fivebests,hcamnt);
    }
    printf("Matkalla\nKeräsit %d pistebonusta\nOsuit %d kohteeseen laserilla\nnappasit %d supervauhtia\nja törmäsit %d esteeseen\n",pi->stats.piste,pi->stats.laser,pi->stats.nopeus,pi->stats.este);
}
int kbhit()
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}
int check_possible_directions(tienpala *tiep,int autopaikka)
{
    int suunnat=0,i;
    if(tiep->vas<autopaikka-1) 
        for(suunnat |=1, i=0; i<tiep->esteamnt;i++)
            if(tiep->tukko[i].type == este_block && tiep->tukko[i].paikka==autopaikka-1)
               suunnat&=(~1);
    if(tiep->oik>autopaikka+1)
        for(suunnat |=(1<<1), i=0; i<tiep->esteamnt;i++)
            if(tiep->tukko[i].type == este_block && tiep->tukko[i].paikka==autopaikka+1)
                suunnat&=(~(1<<1));
    return suunnat;
}
#if 0
int leads_to_deadend(peliinfo *pi,int autopaikka,int suunta, int depth)
{
    for(i=0;i<depth;i++)
    {
        get_next_position(&autonpaikka,suunta);
        if(0==check_possible_directions(&(pi->tie[i+2])autonpaikka))
        {

            suunta*=-1;
            get_next_position(&autonpaikka,suunta); /* original place */
            get_next_position(&autonpaikka,suunta);
            if(0==check_possible_directions(&(pi->tie[i+2])autonpaikka))
                return 1;
        }
}
#endif
int try_find_path(peliinfo *pi,int autopaikka,int suunta, int current_attempt, int target_attempt)
{
    //int rval;
    int next_possibilities = check_possible_directions(&(pi->tie[current_attempt+2]),autopaikka+suunta);
    if(!next_possibilities)
        return -1;
    if(target_attempt==current_attempt)
        return 0;
    if(suunta == SUUNTA_VASEN && 1&next_possibilities)
    {
        /* Ok, let's try Vasen */
        if(0!=try_find_path(pi,autopaikka-1,SUUNTA_VASEN,current_attempt+1,target_attempt))
            if((1<<1)&next_possibilities)
                if(0!=try_find_path(pi,autopaikka+1,SUUNTA_OIKEA,current_attempt+1,target_attempt))

            return -1;
                else
                    return 1;
            else
                return -1;
        else
            return 0;

    }
    else if(suunta == SUUNTA_OIKEA && (1<<1)&next_possibilities)
    {
            if(0!=try_find_path(pi,autopaikka+1,SUUNTA_OIKEA,current_attempt+1,target_attempt))
                if((1)&next_possibilities)
                    if(0!=try_find_path(pi,autopaikka-1,SUUNTA_VASEN,current_attempt+1,target_attempt))
                        return -1;
                    else
                        return 1;
                else
                    return -1;
            else
                return 0;
    }
    else
    {
        if(0!=try_find_path(pi,autopaikka+(-1*suunta),-1*suunta,current_attempt+1,target_attempt))
            return -1;
    }
    return 1;
}
void autopilotti_suunnista(peliinfo *pi)
{
    int current_attempt=0,target_attempt=5;
    int rval;
    if(1==(rval=try_find_path(pi,pi->autonpaikka,pi->suunta,current_attempt,target_attempt)))
        pi->suunta*=-1;
    if(-1==rval)
        pi->hidastus=pi->hidastus_alku;
/*
    if(pi->autonpaikka-1<=pi->tie[1].vas)
        pi->suunta=SUUNTA_OIKEA;
    else if(pi->autonpaikka+1==pi->tie[0].oik)
        pi->suunta=SUUNTA_VASEN;
    else
    {
        int i;
        for(i=0;i<pi->tie[1].esteamnt;i++)
            if(pi->autonpaikka+pi->suunta == pi->tie[1].tukko[i].paikka)
            {
                pi->suunta*=-1;
                break;
            }
    }
    */
}

void hit_laser_handler(peliinfo *pi)
{
    pi->pisteet+=5000;
    pi->stats.laser++;
    add_new_extratext(pi,EXTRAT_LASER);
}


void poista_este(tienpala *tiep, int esteindeksi)
{
    if(tiep->esteamnt > (esteindeksi+1))
        memcpy(&(tiep->tukko[esteindeksi]),&(tiep->tukko[esteindeksi+1]),sizeof(este)*(tiep->esteamnt-(esteindeksi+1)));
    memset(&(tiep->tukko[tiep->esteamnt-1]),0,sizeof(este));
    tiep->esteamnt--;

}
void poista_esteet(peliinfo *pi)
{
    int i,j;
    /* etsi auton paikka */
    for(i=0;i<TIEN_PITUUS;i++)
        if(pi->tie[i].vas >= pi->autonpaikka || pi->tie[i].oik <= pi->autonpaikka)
            break;
        else
            for(j=0;j<pi->tie[i].esteamnt;j++)
                if(pi->tie[i].tukko[j].paikka == pi->autonpaikka)
                {
                    poista_este(&(pi->tie[i]),j);
                    hit_laser_handler(pi);
                }
}
int ohjaa(peliinfo *pi)
{
    int nappi=0;
    int arvo=0;

    if(pi->huijaukset.autopilotti)
        autopilotti_suunnista(pi);
    else
        while(kbhit())
            if(3==(arvo=read(0,&nappi,sizeof(int))))
            {
                if(VAS_NUOLI==nappi)
                    pi->suunta=SUUNTA_VASEN;
                else if(OIK_NUOLI==nappi)
                    pi->suunta=SUUNTA_OIKEA;
                else if(ALAS_NUOLI==nappi && !pi->speed_kierros)
                {
                    pi->hidastus-=10000;
                    if(pi->hidastus<0)
                        pi->hidastus=0;
            /* lisää nopeutta ja pistekerrointa */
                }
                else if(YLOS_NUOLI==nappi && !pi->speed_kierros)
                {
            /* vähennä nopeutta ja pistekerrointa */
                    pi->hidastus+=10000;
                    if(pi->hidastus>pi->hidastus_alku)
                        pi->hidastus=pi->hidastus_alku;

                }
                else if(F1==nappi && !pi->ammu)
                {
                    pi->ammu=GUN_RELOAD; //vähennä tätä joka kierroksella 
                }
            }

    return 0;
}
void uusi_este(peliinfo *pi,tienpala *tie,int index)
{
    int luku;
    int tyyppi;
uus:
    luku=rand()%RADAN_LEVEYS+1;
    if(index==2)
        if(tie->vas+luku==tie->tukko[1].paikka)
            goto uus;
    if(index)
        if(tie->vas+luku==tie->tukko[0].paikka)
            goto uus;

    tie->esteamnt++;
    tie->tukko[index].paikka=tie->vas+luku;
    tyyppi=rand()%10;
    if(!pi->huijaukset.estehuijaus && tyyppi<5)
    {
        tie->tukko[index].varialkuleveys=0;
        tie->tukko[index].varileveys=0;
        tie->tukko[index].merkki='#';
        tie->tukko[index].type=este_block;
    }
    else if(!pi->huijaukset.estehuijaus && tyyppi<8)
    {
        tie->tukko[index].varialkuleveys=VARIN_ALKU_LEVEYS;
        tie->tukko[index].vari_alku=vari_alut[este_speed];
        tie->tukko[index].vari_loppu=vari_loppu;
        tie->tukko[index].varileveys=VARIN_LEVEYS;
        tie->tukko[index].merkki='V';
        tie->tukko[index].type=este_speed;
    }
    else
    {
        tie->tukko[index].vari_alku=vari_alut[este_pisteet];
        tie->tukko[index].vari_loppu=vari_loppu;
        tie->tukko[index].varialkuleveys=VARIN_ALKU_LEVEYS;
        tie->tukko[index].varileveys=VARIN_LEVEYS;
        tie->tukko[index].merkki='$';
        tie->tukko[index].type=este_pisteet;
    }
        
}
void arvo_esteet(peliinfo *pi,tienpala *tie)
{
    int satunnaisluku;
    int esteet;
    tie->esteamnt=0;
    for(esteet=0;esteet<3;esteet++)
    {
        satunnaisluku=rand();
        if((!pi->huijaukset.estehuijaus && satunnaisluku<RAND_MAX/10) || (pi->huijaukset.estehuijaus && satunnaisluku>RAND_MAX/10))
            uusi_este(pi,tie,esteet);
        else
            break;
    }
}
void arvo_tie(peliinfo *pi)
{
    tienpala tie_uusi[TIEN_PITUUS];
    int satunnaisluku;
    memset(tie_uusi,0,sizeof(tie_uusi));

arvo_uudestaan:
    satunnaisluku= rand();

    memcpy(tie_uusi,&(pi->tie[1]),sizeof(tienpala)*(TIEN_PITUUS-1));

    if(satunnaisluku < RAND_MAX/4)
    {
        /* vasen */
        tie_uusi[TIEN_PITUUS-1].vas=tie_uusi[TIEN_PITUUS-2].vas-1;
        tie_uusi[TIEN_PITUUS-1].oik=tie_uusi[TIEN_PITUUS-2].oik-1;
    }
    else if(satunnaisluku>=(RAND_MAX-RAND_MAX/4))
    {
        /* oikea */
        tie_uusi[TIEN_PITUUS-1].vas=tie_uusi[TIEN_PITUUS-2].vas+1;   
        tie_uusi[TIEN_PITUUS-1].oik=tie_uusi[TIEN_PITUUS-2].oik+1;   
    }
    else
    {
        /* suoraan */
        tie_uusi[TIEN_PITUUS-1]=tie_uusi[TIEN_PITUUS-2];
    }
    if(tie_uusi[TIEN_PITUUS-1].oik>TIEN_MAX_LEVEYS || tie_uusi[TIEN_PITUUS-1].vas<VARIN_ALKU_LEVEYS)
        goto arvo_uudestaan;
    
    memcpy(pi->tie,tie_uusi,sizeof(tienpala)*TIEN_PITUUS);
    arvo_esteet(pi,&(pi->tie[TIEN_PITUUS-1]));
    
}


static struct termio savemodes;
static int havemodes = 0;

int tty_break()
{
    struct termio modmodes;
    if(ioctl(fileno(stdin), TCGETA, &savemodes) < 0)
        return -1;
    havemodes = 1;
    modmodes = savemodes;
    modmodes.c_lflag &= ~(ICANON|ECHO);
    modmodes.c_cc[VMIN] = 1;
    modmodes.c_cc[VTIME] = 0;
    return ioctl(fileno(stdin), TCSETAW, &modmodes);
}

int tty_fix()
{

    if(!havemodes)
        return 0;
    return ioctl(fileno(stdin), TCSETAW, &savemodes);
}
void ajo_pause()
{
    int tmp;
    printf("paina enter jatkaaksesi\n");
    read(0,&tmp,4);
}


void lisaa_pisteet(peliinfo *pi)
{
    unsigned int pistekerroin=(pi->hidastus_alku-pi->hidastus)/200;
    pi->pisteet+=pistekerroin;
    if(pi->speed_kierros)
    {
        pi->speed_kierros--;
        if(!pi->speed_kierros)
            pi->hidastus=pi->hidastus_saved;
    }
}
void laske_nopeus(peliinfo *pi)
{
    pi->nopeus=(pi->hidastus_alku-pi->hidastus)/20;
}
void uusi_autonpaikka(peliinfo *pi)
{
    pi->autonpaikka+=pi->suunta;
}
int main(int argc, char *argv[])
{
    peliinfo pi;
    int ikahyvitys=0;
    int i;
    hiscore fivebests[HISCORE_AMNT];
    
    memset(&pi,0,sizeof(pi));
    pi.huijaukset.hidastuskerroin=1;
    pi.huijaukset.elamat=1;
    pi.huijaukset.huijaus=0;
    pi.huijaukset.lisaleveys=0;
    pi.huijaukset.estehuijaus=0;
    pi.kierros=0;
    pi.pisteet=0;
    pi.nopeus=0;
    pi.hidastus=pi.hidastus_alku=100000;
    pi.suunta=SUUNTA_OIKEA;

    //init_beeb_tty();
    //beebbeeb(0xfff, 250);
    //return 0;
    if(init_sizes())
    {
 //       printf("liian pieni ruutu\n");
 //       return -1;
    }
    if(!(pi.tie=calloc(sizeof(tienpala),TIEN_PITUUS)))
        return -ENOMEM;
    init_extra_array();
    read_scores(fivebests,HISCORE_AMNT);
    print_fivebests(fivebests,HISCORE_AMNT);
    ajo_pause();
 
    if(argc>1)
    {
        if(!strcmp(argv[1],"Siri") || !strcmp(argv[1],"siri"))
        {
            pi.huijaukset.lisaleveys=20;
            pi.huijaukset.hidastuskerroin=0.3;
        }
        if(!strcmp(argv[1],"Iivari") || !strcmp(argv[1],"iivari"))
        {
            pi.huijaukset.hidastuskerroin=5;
            ikahyvitys=4;
            pi.huijaukset.huijaus=1;
        }
        if(!strcmp(argv[1],"autopilotti"))
        {
            pi.huijaukset.autopilotti=1;
        }
        else if( !strcmp(argv[1],"Jasper") || !strcmp(argv[1],"jasper") )
            pi.huijaukset.estehuijaus=1;
        /*
        else if(!strcmp(argv[1],"Joona") || !strcmp(argv[1],"joona") )
            hidastuskerroin=1;
        else if( !strcmp(argv[1],"Jasper") || !strcmp(argv[1],"jasper") )
            hidastuskerroin=1;
        
        else if(!strcmp(argv[1],"Isi") || !strcmp(argv[1],"isi"))
            pistekerroin=100;
            */
        else if(!strcmp(argv[1],"Johanna") || !strcmp(argv[1],"johanna"))
        {
            pi.huijaukset.elamat=6;
        }
    }
    if( pi.huijaukset.estehuijaus || pi.huijaukset.lisaleveys || pi.huijaukset.elamat>1 || (pi.huijaukset.hidastuskerroin-ikahyvitys)>1 || pi.huijaukset.autopilotti)
        pi.huijaukset.huijaus=1;
    srand(time(NULL));
    tty_break();

    pi.autonpaikka=30+pi.huijaukset.lisaleveys/2;

    for(i=0;i<TIEN_PITUUS;i++)
    {
        pi.tie[i].vas=20;
        pi.tie[i].oik=pi.tie[i].vas+RADAN_LEVEYS+pi.huijaukset.lisaleveys;
    }   


    for (pi.kierros=0;1;pi.kierros++)
    {
        lisaa_pisteet(&pi);
        if(pi.ammu == GUN_RELOAD)
            poista_esteet(&pi);
        if(!(pi.kierros%3))
            advance_extratext_ctr(&pi);
        if(piirra_ja_tarkista(&pi /*(100000-hidastus)/20,kierros*pistekerroin*/,(argc>1)?argv[1]:"Tuntematon"))
        {
            pi.huijaukset.elamat--;
            if(!pi.huijaukset.elamat)
            {
                lopeta(&pi,(argc>1)?argv[1]:"Tuntematon",fivebests,HISCORE_AMNT);
                break;
            }
            else
                pi.autonpaikka=pi.tie[0].vas+10;
        }
        if(pi.ammu)
            pi.ammu--;   
        if(ohjaa(&pi))
            break;
        uusi_autonpaikka(&pi);
        arvo_tie(&pi);
        pi.hidastus-=100;
        laske_nopeus(&pi);
        usleep((pi.hidastus<=0)?0:(pi.hidastus*pi.huijaukset.hidastuskerroin));
    }
    tty_fix();
    return 0;
}
