CC=gcc
CFLAGS=-Wall -ggdb
TGT=autopelihieno
SRC=ajo.c 
LDFLAGS=

$(TGT): ajo.c
		$(CC) $(CFLAGS) -o $(TGT) $(SRC) $(LDFLAGS)

clean:
	rm -rf $(TGT)
