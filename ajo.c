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

static int TIEN_PITUUS=22;
#define TIEN_MAX_LEVEYS 60
//7 for spaces and "speed", 7 for spaces and"points", 6 for points 7 for speed, one for NULL byte
#define GUESS_SPEED_SIZE  28

typedef enum extratexts
{
    EXTRAT_POINTS = 0,
    EXTRAT_SPEED,
    EXTRA_AMNT
}extratexts;
#define EXTRA_MAX (EXTRA_AMNT-1)

const char *extratext_s[EXTRA_AMNT] =
{
    [EXTRAT_POINTS] = "+10000 Points!!!",
    [EXTRAT_SPEED] = "Super Speed!!!" 
};
//struct extratext;
typedef struct extratext
{
 //   int display_ctr;
    const char *text;
}extratext;

struct extratext extra_array[1024];

typedef enum estetyyppi
{
    este_unused=0,
    este_block,
    este_speed,
    este_pisteet
}estetyyppi;

typedef struct este
{
    int type;
    char merkki;
    int paikka;
}este;

typedef struct tienpala
{
    int vas;
    int oik;
    int esteamnt;
    este tukko[3];
    //struct *extratext;
}tienpala;

typedef struct estestats
{
    int nopeus;
    int piste;
    int este;
}estestats;

typedef struct peliinfo
{
    int kierros;
    unsigned int pisteet;
    unsigned int pistekerroin;
    unsigned int nopeus;
    int suunta;
    int autonpaikka;
    int hidastus;
    int hidastus_alku;
    int speed_kierros;
    int hidastus_saved;
    tienpala *tie;
    int extratext_array_current;
    estestats stats;
}peliinfo;
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

typedef struct hiscore
{
    int used;
    char name[NAME_MAX];
    unsigned int max_speed;
    unsigned int points;
    estestats stats;
}hiscore;

static hiscore fivebests[HISCORE_AMNT];
//static int ttyfd;

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
void print_fivebests()
{
    int i;
    printf("Best Of The Best:\n\n");
    for(i=0;i<5&&fivebests[i].used;i++)
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
        for(i=0;i<5&&6==(rval=fscanf(rf,"%m[^!]!%u!%u!%d!%d!%d\n",&name,&stmp,&ptmp,&st_etmp,&st_ntmp,&st_ptmp));i++)
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
        for(i=0;i<5&&fivebests[i].used;i++)
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

void update_fivebests(peliinfo *pi,char *name)
{
    int i;
//    printf("Vanhat Huippupisteet:\n");
//    print_fivebests();
    for(i=0;i<5&&fivebests[i].used;i++)
    {
        if(fivebests[i].points<pi->pisteet)
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
        fivebests[i].max_speed=pi->nopeus;
        fivebests[i].points=pi->pisteet;
        fivebests[i].used=1;
        fivebests[i].stats.este=pi->stats.este;
        fivebests[i].stats.nopeus=pi->stats.nopeus;
        fivebests[i].stats.piste=pi->stats.piste;
    }
    write_scores();
}

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
void piirra_esteet(char *tie,tienpala *tiep)
{
    int i;
    for(i=0;i<tiep->esteamnt && i<3;i++)
        if(tiep->tukko[i].type==este_block || tiep->tukko[i].type==este_speed || tiep->tukko[i].type==este_pisteet)
            tie[tiep->tukko[i].paikka]=tiep->tukko[i].merkki;
}
void hit_points_handler(peliinfo *pi)
{
    //int i;
    pi->pisteet+=10000;
    pi->stats.piste++;
    /*
    for(i=0;i<TIEN_PITUUS/2;i++)
        printf("\n");
    printf("Jippii!!! +2000 Pistettä!!!\n");
    for(i=0;i<TIEN_PITUUS/2;i++)
        printf("\n");
    sleep(1);
    */
    add_new_extratext(pi,EXTRAT_POINTS);
    //fprintf(stdout, "\7");
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
int piirra_ylarivi(tienpala *tiep, peliinfo *pi, char *kuski)
{
    char tie[TIEN_MAX_LEVEYS+1];
    int rval=0;
   // int i;

    memset(tie,' ',sizeof(tie));

    tie[tiep->vas]=TIEMERKKI_VAS;
    tie[tiep->oik]=TIEMERKKI_OIK;
    tie[pi->autonpaikka]=AUTOMERKKI;
    piirra_esteet(tie,tiep);
        
    tie[TIEN_MAX_LEVEYS]='\0';

    if(pi->autonpaikka<=tiep->vas || pi->autonpaikka>=tiep->oik || tarkista_esteet(pi,tiep))
    {
        tie[pi->autonpaikka]='X';
        rval=-1;
    }
    printf("%s Ajajana %s\n",tie,kuski);
    clear_extratext(pi);
    return rval;
}
void piirra_alarivi(tienpala *tiep,unsigned int nopeus, unsigned int pisteet)
{
    char tie[TIEN_MAX_LEVEYS+1];
    memset(tie,' ',sizeof(tie));
    tie[tiep->vas]=TIEMERKKI_VAS;
    tie[tiep->oik]=TIEMERKKI_OIK;
    piirra_esteet(tie,tiep);
    tie[TIEN_MAX_LEVEYS]='\0';
    printf("%s speed %u points %u\n",tie,nopeus,pisteet);
}
void piirra_rivi(tienpala *tiep, const char *et)
{
    char tie[TIEN_MAX_LEVEYS+1];
    memset(tie,' ',sizeof(tie));

    tie[tiep->vas]=TIEMERKKI_VAS;
    tie[tiep->oik]=TIEMERKKI_OIK;
    piirra_esteet(tie,tiep);
    tie[TIEN_MAX_LEVEYS]='\0';
    if(et)
        printf("%s %s\n",tie,et);
    else
        printf("%s\n",tie);
}

int piirra_ja_tarkista(peliinfo *pi, char *kuski)
{
    int rval=0;
    int i;
 
    if(!(rval=piirra_ylarivi(&(pi->tie[0]),pi,kuski)))
    {
        for(i=1;i<TIEN_PITUUS-1;i++)
            piirra_rivi
            (
                &(pi->tie[i]),
                get_extratext_for_tienpala_index(pi,i)
            );
        piirra_alarivi(&(pi->tie[i]),pi->nopeus,pi->pisteet);
    }
    fflush(stdout);
    return rval;
}

void lopeta(peliinfo *pi, char *nimi,int huijaus)
{
    int i;
    for(i=0;i<TIEN_PITUUS;i++)
        printf("\n");
    if(nimi)
        printf("Olipa hauska ajella vaikka meninkin päin seinää - Pisteesi %s: %u\n",nimi,(unsigned)pi->pisteet);
    else
        printf("Olipa hauska ajella vaikka meninkin päin seinää - pisteet %u\n",(unsigned)pi->pisteet);
    if(!huijaus)
    {
        update_fivebests(pi,nimi);
        print_fivebests();
    }
    printf("Matkalla keräsit %d pistebonusta\nnappasit %d supervauhtia\nja osuit %d esteeseen\n",pi->stats.piste,pi->stats.nopeus,pi->stats.este);
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

int ohjaa(peliinfo *pi)
{
    int nappi=0;
    int arvo=0;

    while(kbhit())
        if( 3==(arvo=read(0,&nappi,sizeof(int))))
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
            //pi->pistekerroin+;
            /* lisää nopeutta ja pistekerrointa */
            }
            else if(YLOS_NUOLI==nappi && !pi->speed_kierros)
            {
            /* vähennä nopeutta ja pistekerrointa */
                pi->hidastus+=10000;
                if(pi->hidastus>pi->hidastus_alku)
                    pi->hidastus=pi->hidastus_alku;

            //pi->pistekerroin-=600/20;
           }
        }

    return 0;
}
void uusi_este(tienpala *tie,int index)
{
    int luku;
    int tyyppi;
uus:
    luku=rand()%10+1;
    if(index==2)
        if(tie->vas+luku==tie->tukko[1].paikka)
            goto uus;
    if(index)
        if(tie->vas+luku==tie->tukko[0].paikka)
            goto uus;

    tie->esteamnt++;
    tie->tukko[index].paikka=tie->vas+luku;
    tyyppi=rand()%10;
    if(tyyppi<5)
    {
        tie->tukko[index].merkki='#';
        tie->tukko[index].type=este_block;
    }
    else if(tyyppi<8)
    {
        tie->tukko[index].merkki='V';
        tie->tukko[index].type=este_speed;
    }
    else
    {
        tie->tukko[index].merkki='$';
        tie->tukko[index].type=este_pisteet;
    }
        
}
void arvo_esteet(tienpala *tie)
{
    int satunnaisluku;
    int esteet;

    for(esteet=0;esteet<3;esteet++)
    {
        satunnaisluku=rand();
        if(satunnaisluku<RAND_MAX/10)
            uusi_este(tie,esteet);
        else
            break;
    }
}
void arvo_tie(tienpala *tie)
{
    tienpala tie_uusi[TIEN_PITUUS];
    int satunnaisluku;
    memset(tie_uusi,0,sizeof(tie_uusi));

arvo_uudestaan:
    satunnaisluku= rand();

    memcpy(tie_uusi,&(tie[1]),sizeof(tienpala)*(TIEN_PITUUS-1));

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
    if(tie_uusi[TIEN_PITUUS-1].oik>TIEN_MAX_LEVEYS || tie_uusi[TIEN_PITUUS-1].vas<0)
        goto arvo_uudestaan;
    
    memcpy(tie,tie_uusi,sizeof(tienpala)*TIEN_PITUUS);
    arvo_esteet(&tie[TIEN_PITUUS-1]);
    
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
    unsigned int pistekerroin=(pi->hidastus_alku-pi->hidastus)/20;
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
//    int autonpaikka=30,i;
    peliinfo pi;
 //   int suunta=SUUNTA_OIKEA;
 //   int kierros;
 //   int hidastus = 100000;
    float hidastuskerroin=1;
    int elamat=1;
    int huijaus=0;
    int ikahyvitys=0;
    int i;
    memset(&pi,0,sizeof(pi));
    //pi.pistekerroin=1;
    pi.kierros=0;
    pi.pisteet=0;
    pi.nopeus=0;
    pi.hidastus=pi.hidastus_alku=100000;
    pi.suunta=SUUNTA_OIKEA;
    pi.autonpaikka=30;

    //init_beeb_tty();
    //beebbeeb(0xfff, 250);
    //sleep(1);
    //return 0;
    if(init_sizes())
    {
        printf("liian pieni ruutu\n");
        return -1;
    }
    if(!(pi.tie=calloc(sizeof(tienpala),TIEN_PITUUS)))
        return -ENOMEM;
    init_extra_array();
    read_scores();
    print_fivebests();
    ajo_pause();
 
    if(argc>1)
    {
        if(!strcmp(argv[1],"Iivari") || !strcmp(argv[1],"iivari"))
        {
            hidastuskerroin=5;
            ikahyvitys=4;
        }
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
            //pi.pistekerroin=10;
            elamat=6;
        }
    }
    if( elamat>1 || (hidastuskerroin-ikahyvitys)>1)
        huijaus=1;
    srand(time(NULL));
    tty_break();

    for(i=0;i<TIEN_PITUUS;i++)
    {
        pi.tie[i].vas=20;
        pi.tie[i].oik=40;
    }   


    for (pi.kierros=0;1;pi.kierros++)
    {
        lisaa_pisteet(&pi);
        if(!(pi.kierros%3))
            advance_extratext_ctr(&pi);
        if(piirra_ja_tarkista(&pi /*(100000-hidastus)/20,kierros*pistekerroin*/,(argc>1)?argv[1]:"Tuntematon"))
        {
            elamat--;
            if(!elamat)
            {
                lopeta(&pi,(argc>1)?argv[1]:"Tuntematon",huijaus);
                break;
            }
            else
                pi.autonpaikka=pi.tie[0].vas+10;
        }
        
        if(ohjaa(&pi))
            break;
        uusi_autonpaikka(&pi);
        arvo_tie(pi.tie);
        pi.hidastus-=100;
        laske_nopeus(&pi);
        usleep((pi.hidastus<=0)?0:(pi.hidastus*hidastuskerroin));
    }
    tty_fix();
    return 0;
}
