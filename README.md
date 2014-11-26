book-orders
===========

super secret project with omar

*Note: when running, you may adjust the buffer size for the orders at the top of queue.h

Data Structures

Customer Struct - Each customer struct holds customer name, numerical customer id, balance, address, state, zip, as well as two lists: one for Successful Orders and one for Rejected Orders (along with pointers to the back of the list, to optimize list insertion). Lastly it has a mutex that locks itself to access by the Consumer threads. 

Customer Database (CDB) – This is a wrapper for an array of Customer structs. It holds 1) the array of customer structs, 2) a double for the total revenue gained from orders, 3) the number of customers in the array, and 4) the total size of the array. It also holds a mutex to lock access to its revenue field, so only one consumer can update it at a time.

Consumer Struct – Holds the information that each consumer thread is responsible for. Holds 1) a string for the book category name, 2) a queue of orders to be processed, 3) a mutex to control access to this queue (so the Producer thread and Consumer thread are not modifying it at the same time),  4) and two condition variables: one for the producer to shout at the consumer that there is new order data available in the queue, and one for the consumer to shout at the producer that there is space available in the queue.

Consumer Struct Array (CSA) – Holds 1) an integer flag to signal to the consumers that the producer is done with the orders, 2) the number of consumers structs so far, 3) the actual size of the array 4) the actual array of Consumer Structs.

Design

The program starts by reading in the data from various text files. 

	- read_customers() parses the database.txt, creates customer structs, and calls CDInsert() to 	toss these into the customer database. 
	- read_categories() then parses the categories text file and creates Consumer Structs, callling 	CSAInsert() to toss these into the Consumer Struct Array.

-The main function now creates a thread which runs the producer() function, and threads running the consumer() function for each category.

The producer thread will read the orders file and create orders. It tosses it into the proper consumer's queue, while respecting the mutex. If the max buffer size has been reached, it will wait to insert into the queue until the consumer shouts spaceAvailable. Also, the producer will shout dataAvailable to the consumer once it has inserted new orders into the queue, as the consumer may be waiting on new data.

The consumer threads will pop orders off its queue one by one and call CDUpdate() (this will perform the balance adjustments and update the customer's successful orders list, or just update the rejected orders list.) The consumer will wait to do this if its queue is empty, until the producer shouts dataAvailable.

This continues till all the threads have finished; Producer waits for the consumers to be done, then ceases. The program prints the final database and exits. ZE END!
