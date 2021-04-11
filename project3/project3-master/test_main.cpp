#include "main.h"


#define NUM_THREADS 1
#define DURATION NUM_THREADS00

// for testing purposes
int main(int argc, char *argv[])
{
  //Pass in how many customers per seller (
  assert(argc == 2);
  int n, j;
  char i;
  std::string id;
  n = atoi(argv[1]);

  //Create Ticket Manager
  TicketManager ticket_manager;
  
  //Create Ticket Sellers
  std::array<Seller*, NUM_THREADS> ticket_sellers;

  //Array for threads 
  std::array<std::thread, NUM_THREADS> seller_threads;

  //Seller Class should allow to pass in TicketManager by reference and implement = operator
  //Create 1 High Priority Seller
  ticket_sellers[0] = new Seller("H1",HIGHPRICE, &ticket_manager);

  /*//Create 3 Medium Priority Sellers
  j = 0;
  for(i = 1; i < 4; ++i)
  {
    id = "M";
    id += ('1' + j++);
    //std::cout << id << std::endl;
    ticket_sellers[i] = new Seller(id,MIDPRICE);
  }
  j=0;
  //Create 6 Low Priority Sellers
  for(i = 4; i < NUM_THREADS; ++i)
  {
    id = "L";
    id += ('1' + j++);
    //std::cout << id << std::endl;
    ticket_sellers[i] = new Seller(id,LOWPRICE);
  } */
  //Generate Threads
  for(i = 0; i < NUM_THREADS; ++i)
  {
    //seller_threads[i] = std::thread(&Seller::run, ticket_sellers[i], n, 60);
    ticket_sellers[i]->generateCustomers(n);
  }

  bool isActiveCustomer = false;
  for (int tick = 0; tick < 60 || isActiveCustomer; tick++) {
	  for (i= 0; i < NUM_THREADS; ++i) { // ticks each seller
	  	seller_threads[i] = std::thread(&Seller::tick, ticket_sellers[i], 60, tick);
	  }
	  isActiveCustomer = false;
	  for(i = 0; i < NUM_THREADS; ++i)  // oins threads and checks for active customers
	  {
	    //std::cout << "Waiting to terminate threads" << std::endl;
	    seller_threads[i].join();
	    isActiveCustomer = isActiveCustomer || ticket_sellers[i]->has_active_customer();
	    if(tick >= 60 && ticket_sellers[i]->has_active_customer()) {}
	    	//printf("%i active\n", i);
	    }
	  }
	  //printf("tick: %i\n", tick);
   }

  for (i = 0; i < NUM_THREADS; ++i) {
  	ticket_sellers[i]->print_event_log();
  }

  
  for(i=0; i < NUM_THREADS; ++i)
  {
    delete ticket_sellers[i];
  }

  std::cout << ticket_manager << std::endl;  
}
