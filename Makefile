CC=gcc
CFLAGS=-Wall -ggdb
TGT=autopelihieno
SRC=ajo.c hiscore.c 
LDFLAGS=

$(TGT): ajo.c hiscore.c
		$(CC) $(CFLAGS) -o $(TGT) $(SRC) $(LDFLAGS)

clean:
	rm -rf $(TGT)
