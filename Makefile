CFLAGS=-Wall -g
CC=clang

all: tim_coin

tim_coin: tim_coin.o parser.o addr_bal.o dbl_spends.o hmap.o deanon.o
	$(CC) $(CFLAGS) -o $@ $^
		
.PHONY clean:
	rm -f *.o
	rm -Rf *.dSYM
	rm -f tim_coin