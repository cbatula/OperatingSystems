#include "main.h"

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
  std::array<Seller*, 10> ticket_sellers;

  //Array for threads
  std::array<std::thread, 10> seller_threads;

  //Seller Class should allow to pass in TicketManager by reference and implement = operator
  //Create 1 High Priority Seller
  ticket_sellers[0] = new Seller("H1",HIGHPRICE, &ticket_manager);

  //Create 3 Medium Priority Sellers
  j = 0;
  for(i = 1; i < 4; ++i)
  {
    id = "M";
    id += ('1' + j++);
    //std::cout << id << std::endl;
    ticket_sellers[i] = new Seller(id,MIDPRICE, &ticket_manager);
  }
  j=0;
  //Create 6 Low Priority Sellers
  for(i = 4; i < 10; ++i)
  {
    id = "L";
    id += ('1' + j++);
    //std::cout << id << std::endl;
    ticket_sellers[i] = new Seller(id,LOWPRICE, &ticket_manager);
  }
  //Generate Customers
  for(i = 0; i < 10; ++i)
  {
    //seller_threads[i] = std::thread(&Seller::run, ticket_sellers[i], n, 60);
    ticket_sellers[i]->generateCustomers(n);
  }

  bool isActiveCustomer = false;
  for (int tick = 0; tick < 60 || isActiveCustomer; tick++) {
	  for (i= 0; i < 10; ++i) { // ticks each seller
	  	seller_threads[i] = std::thread(&Seller::tick, ticket_sellers[i], 60, tick);
	  }
	  isActiveCustomer = false;
	  for(i = 0; i < 10; ++i)  // oins threads and checks for active customers
	  {
	    //std::cout << "Waiting to terminate threads" << std::endl;
	    seller_threads[i].join();
	    isActiveCustomer = isActiveCustomer || ticket_sellers[i]->has_active_customer();
	  }
   }

  //Print Results
  int high_seated=0;
  int high_rejected=0;
  int high_turnabout=0;
  int high_response=0;
  int high_served = 0;
  int high_served_2 =0;
  int med_seated=0;
  int med_rejected=0;
  int med_turnabout=0;
  int med_response=0;
  int med_served = 0;
  int med_served_2 =0;
  int low_seated=0;
  int low_rejected=0;
  int low_turnabout=0;
  int low_response=0;
  int low_served = 0;
  int low_served_2 =0;
  for (i = 0; i < 10; ++i) {
  	vector<int> temp = ticket_sellers[i]->print_event_log();
	cout << "price: " << temp[0] <<" seated: " << temp[1]
    << " turnedaway: "<< temp[2] << " turnabout: "
     << temp[3] << " response: " << temp[4] << "\n";
	if (temp[0] == 0) {
		high_seated+=temp[1];
		high_rejected+=temp[2];
		high_served = temp[5];
                high_served_2 = temp[6];
    high_turnabout+=temp[3];
    high_response+=temp[4];
	}
	else if (temp[0] == 1){
		med_seated+=temp[1];
		med_rejected+=temp[2];
    med_turnabout+=temp[3];
    med_response+=temp[4];
    med_served+= temp[5];
    med_served_2 += temp[6];
	}
	else {
		low_seated+=temp[1];
		low_rejected+=temp[2];
    low_turnabout+=temp[3];
    low_response+=temp[4];
    low_served+= temp[5];
    low_served_2 += temp[6];
	}
  }

  for(i=0; i < 10; ++i)
  {
    delete ticket_sellers[i];
  }

  std::cout << ticket_manager << std::endl;
  cout << "H sold: "<<high_seated<<" H rejected: "<<high_rejected<<"\n";
  cout << "M sold: "<<med_seated<<" M rejected: "<<med_rejected<<"\n";
  cout << "L sold: "<<low_seated<<" L rejected: "<<low_rejected<<"\n";
  float high_avg_turnabout = high_turnabout / (high_served_2 * 1.0);
  float med_avg_turnabout = med_turnabout / (med_served_2 * 1.0);
  float low_avg_turnabout = low_turnabout / (low_served_2 * 1.0);
  cout << "H turnabout: " << high_avg_turnabout << "\n";
  cout << "M turnabout: " << med_avg_turnabout << "\n";
  cout << "L turnabout: " << low_avg_turnabout << "\n";
  float high_avg_response = high_response / (high_served * 1.0);
  float med_avg_response = med_response / (med_served * 1.0);
  float low_avg_response = low_response / (low_served * 1.0);
  cout << "H response: " << high_avg_response << "\n";
  cout << "M response: " << med_avg_response << "\n";
  cout << "L response: " << low_avg_response << "\n";
  float high_throughput = high_served;
  float med_throughput = med_served/3.0;
  float low_throughput = low_served/6.0;
  cout << "H throughput: " << high_throughput << "\n";
  cout << "M throughput: " << med_throughput << "\n";
  cout << "L throughput: " << low_throughput << "\n";

}
