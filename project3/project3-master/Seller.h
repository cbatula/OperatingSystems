#ifndef SELLER
#define SELLER

#include "ticket_manager.h"
#include "Customer.h"
#include <string>
#include <queue>
#include <list>
#include <vector>

using namespace std;

#define HIGHPRICE 0
#define MIDPRICE 1
#define LOWPRICE 2

class Seller {
	private:
		// attributes
		int priceLevel; //2 == low, 1 == medium, 0 == high
		list <Customer> unarrivedCustomers;  // customers for this run
		queue <Customer> arrivedCustomers;  // customers in queue
		queue <Customer> servicedCustomers; // customers that received a ticket

		Customer* currentCustomer;
		string sellerID;

		TicketManager* tickman;

		list <string> eventLog; // stores strings

		list <int> response_times;
		list <int> turnaround_times;

		int seated;
		int turned_away;

		// private functions

		void logEvent(int, string);

	public:
		int turnaround_time;
		int response_time;
		Seller(string sellerID, int priceLevel, TicketManager* tm);

		void generateCustomers(int); // generates n customers and adds them to custQueue
		int run(int, int);
		void tick(int, int);

		vector<int> print_event_log();
		bool has_active_customer();
};

void printEvent(int time, string message);

#endif
