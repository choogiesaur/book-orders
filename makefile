COMPILER = gcc
CCFLAGS = -g -Wall
all: book-orders

debug:
	make DEBUG=TRUE

#index: tokenizer.o index.o sorted-list.o
#	$(COMPILER) $(CCFLAGS) -o index tokenizer.o index.o sorted-list.o
	
#tokenizer.o: tokenizer.c
#	$(COMPILER) $(CCFLAGS) -c tokenizer.c
	
#index.o: index.c index.h
#	$(COMPILER) $(CCFLAGS) -c index.c

#sorted-list.o: sorted-list.c sorted-list.h
#	$(COMPILER) $(CCFLAGS) -c sorted-list.c

book-orders: book-orders.o customer-database.o
	$(COMPILER) $(CCFLAGS) -o book-orders book-orders.o customer-database.o

book-orders.o: book-orders.c book-orders.h
	$(COMPILER) $(CCFLAGS) -c book-orders.c
	
customer-database.o: customer-database.c customer-database.h
	$(COMPILER) $(CCFLAGS) -c customer-database.c

ifeq ($(DEBUG), TRUE)
 CCFLAGS += -g
endif

clean:
	rm -f book-orders
	rm -f *.o
