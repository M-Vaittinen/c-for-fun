#ifndef _COMMON_H_
#define _COMMON_H_

#define WINDOW_X (640*2)
#define WINDOW_Y (480*2)

/* Alusten maksimimäärä */
#define ALUKSET_MAX 255
/* Yhtäaikaisten poweruppien maksimimäärä */
#define MAX_PUPS 5
/* tsäännsi että uus poweruppi tehdään on yksi näin monesta */
#define UUDEN_PUPIN_TSAANNSSI 3

#define MIN_SERVER_UPDATE_TIME_US 5000
//5 ms
#define LOOP_DELAY_US 5000
#define NOP_MAX 36000*2

#endif
