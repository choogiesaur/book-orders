COMPILER = gcc
CCFLAGS = -g -Wall
all: book-orders

debug:
	make DEBUG=TRUE

book-orders: book-orders.o customer-database.o consumer-database.o
	$(COMPILER) $(CCFLAGS) -o book-orders book-orders.o customer-database.o

book-orders.o: book-orders.c book-orders.h
	$(COMPILER) $(CCFLAGS) -c book-orders.c
	
customer-database.o: customer-database.c customer-database.h
	$(COMPILER) $(CCFLAGS) -c customer-database.c

consumer-database.o: consumer-database.c consumer-database.h
	$(COMPILER) $(CCFLAGS) -c consumer-database.c

ifeq ($(DEBUG), TRUE)
 CCFLAGS += -g
endif

clean:
	rm -f book-orders
	rm -f *.o
