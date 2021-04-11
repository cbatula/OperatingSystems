#include "Seller.h"
#include "Customer.h"
#include <iostream>
#include <string>
#include <math.h>
#include <vector>

using namespace std;

int low_sel_ind=99; //indexes for the low, med, and high sellers for seats
int med_sel_ind=50;
int high_sel_ind=0;
int total_seats_sold=0;
int seated=0;
int turned_away=0;
int response_time=0;
int served = 0;
int turnaround_time=0;
pthread_mutex_t mtex = PTHREAD_MUTEX_INITIALIZER;


Seller::Seller(string sellerID, int priceLevel, TicketManager* tm) {
	this->priceLevel = priceLevel;
	this->currentCustomer = NULL;
	this->sellerID = sellerID;
	this->tickman = tm;
	this->currentCustomer = NULL;
}


void Seller::generateCustomers(int n) {
	for (int i = 0; i < n; i++) {
		// adds a customer that matches the seller's price level
		this->unarrivedCustomers.push_back(Customer(i, this->priceLevel));
 	}
}

void Seller::tick(int duration, int tick) {

	// (1) check if any customers are freshly arrived in the queue
	// adds arrived customers to arrivedCustomers
	int tick_time = tick;
	for (list<Customer>::iterator it = this->unarrivedCustomers.begin(); it != this->unarrivedCustomers.end();) {
		if (it->getArrivalTime() <= tick) {
			this->arrivedCustomers.push(*it); // adds customer to arrivedCustomers
			//printEvent(i, "Customer " + to_string(it->getID()) + " arrived.");
			this->logEvent(tick, "Customer " + to_string(it->getID()) + " arrived.");
			it = this->unarrivedCustomers.erase(it); // removes customer from unarrivedCustomers
		}
		else
			++it;
	}

	// (2) check if previously started customer is finished
	if (currentCustomer != NULL) {
		// printf("customer completion: %i\n", currentCustomer->getCompletionTime());
		// printf("customer arrival: %i\n", currentCustomer->getArrivalTime());
		// printf("customer id: %i\n", currentCustomer->getID());
		if (currentCustomer->getCompletionTime() <= tick) { // if customer is now finished
			this->servicedCustomers.push(*(currentCustomer));  // adds customer to finished queue
			// printEvent(i, "Customer " + to_string(currentCustomer->getID()) + " completed transaction!");
			this->logEvent(tick, "Customer " + to_string(currentCustomer->getID()) + " completed transaction!");
			this->turnaround_times.push_back(currentCustomer->getCompletionTime() - currentCustomer->getArrivalTime());
			currentCustomer = NULL;  // now there is no active customer
		}
	}

	// (3) if need to start a new customer transaction at this tick
	// only starts a transaction if under the time limit
	if (currentCustomer == NULL && tick < duration && !this->arrivedCustomers.empty()) {
		// TODO
		// 1. pull customer off arrivedCustomers
		// 2. try to purchase ticket
		// 3. do stuff depending if ticket purchase failed or succeeded
		// need print statements for:
		// 		customer is assigned a seat
		// 		customer is rejected

		// calculate completion time when customer starts being served

		currentCustomer = &(this->arrivedCustomers.front()); //pull next customer off queue
		this->arrivedCustomers.pop(); //remove customer from queue
		response_time+=tick_time-currentCustomer->getArrivalTime();
		served += 1;
		printf("response time: %i\n", tick - currentCustomer->getArrivalTime());
		this->response_times.push_back(tick - currentCustomer->getArrivalTime());

		/*printf("customer id: %i\n", currentCustomer->getID());
		cout << "seller id: " << this->sellerID << endl;
		printf("tick: %i\n", tick);
		printf("customer duration: %i\n", currentCustomer->getDuration());
		printf("customer end time: %i\n", tick + currentCustomer->getDuration());*/

		string ticket_id = this->sellerID + "_" + to_string(currentCustomer->getID());

		// pthread_mutex_lock(&mtex);

		if(priceLevel == LOWPRICE) //low price 10 -> 1
		{
			if(total_seats_sold==100) {
				this->logEvent(tick, "Customer " + to_string(currentCustomer->getID()) + " rejected transaction!");
				turned_away+=1;
				currentCustomer = NULL;
			}
			else if(!tickman->trySellTicket(ticket_id,low_sel_ind%10,floor(low_sel_ind/10)))
			{
				while(!tickman->trySellTicket(ticket_id,low_sel_ind%10,floor(low_sel_ind/10)))
				{
					low_sel_ind-=1;
				}
				this->logEvent(tick, "Customer " + to_string(currentCustomer->getID()) + " assigned a seat!");
				currentCustomer->setCompletionTime(tick+currentCustomer->getDuration());
				turnaround_time+=tick_time-currentCustomer->getArrivalTime();
				total_seats_sold+=1;
				seated+=1;
			}
			else
			{
				this->logEvent(tick, "Customer " + to_string(currentCustomer->getID()) + " assigned a seat!");
				currentCustomer->setCompletionTime(tick+currentCustomer->getDuration());
				turnaround_time+=tick_time-currentCustomer->getArrivalTime();
				low_sel_ind-=1; //decrement seats from last row last seat
				total_seats_sold+=1;
				seated+=1;
			}
		}
		else if(priceLevel == MIDPRICE) //medium price 5 -> 6 -> 4 -> 7
		{
			if(total_seats_sold==100) {
				this->logEvent(tick, "Customer " + to_string(currentCustomer->getID()) + " rejected transaction!");
				turned_away+=1;
				currentCustomer = NULL;
			}
			else if(!tickman->trySellTicket(ticket_id,med_sel_ind%10,floor(med_sel_ind/10))) //if ticket is not available
			{
				while(!tickman->trySellTicket(ticket_id,med_sel_ind%10,floor(med_sel_ind/10)))
				{
					if(med_sel_ind==69) //seat rotation as specified in lab doc (row 5, row 6, row 4, etc)
						med_sel_ind=40;
					else if(med_sel_ind==49)
						med_sel_ind=70;
					else if(med_sel_ind==79)
						med_sel_ind=30;
					else if(med_sel_ind==39)
						med_sel_ind=80;
					else if(med_sel_ind==89)
						med_sel_ind=20;
					else if(med_sel_ind==29)
						med_sel_ind=90;
					else if(med_sel_ind==99)
						med_sel_ind=10;
					else if(med_sel_ind==19)
						med_sel_ind=0;
					else
						med_sel_ind+=1; //increment seat within a row and use above conditions to switch rows accordingly
				}
				this->logEvent(tick, "Customer " + to_string(currentCustomer->getID()) + " assigned a seat!");
				currentCustomer->setCompletionTime(tick+currentCustomer->getDuration());
				turnaround_time+=tick_time-currentCustomer->getArrivalTime();
				total_seats_sold+=1;
				seated+=1;
			}
			else
			{
				this->logEvent(tick, "Customer " + to_string(currentCustomer->getID()) + " assigned a seat!");
				currentCustomer->setCompletionTime(tick+currentCustomer->getDuration());
				if(med_sel_ind==69) //seat rotation as specified in lab doc (row 5, row 6, row 4, etc)
					med_sel_ind=40;
				else if(med_sel_ind==49)
					med_sel_ind=70;
				else if(med_sel_ind==79)
					med_sel_ind=30;
				else if(med_sel_ind==39)
					med_sel_ind=80;
				else if(med_sel_ind==89)
					med_sel_ind=20;
				else if(med_sel_ind==29)
					med_sel_ind=90;
				else if(med_sel_ind==99)
					med_sel_ind=10;
				else if(med_sel_ind==19)
					med_sel_ind=0;
				else
					med_sel_ind+=1; //increment seat within a row and use above conditions to switch rows accordingly
				total_seats_sold+=1;
				seated+=1;
				turnaround_time+=tick_time-currentCustomer->getArrivalTime();
			}
		}
		else if(priceLevel == HIGHPRICE) //high price 1 -> 10
		{
			if(total_seats_sold==100){
				this->logEvent(tick, "Customer " + to_string(currentCustomer->getID()) + " rejected transaction!");
				turned_away+=1;
				currentCustomer = NULL;
			}
			else if(!tickman->trySellTicket(ticket_id,high_sel_ind%10,floor(high_sel_ind/10))) //if ticket is not available
			{
				while(!tickman->trySellTicket(ticket_id,high_sel_ind%10,floor(high_sel_ind/10)))
				{
					high_sel_ind+=1;
				}
				this->logEvent(tick, "Customer " + to_string(currentCustomer->getID()) + " assigned a seat!");
				currentCustomer->setCompletionTime(tick+currentCustomer->getDuration());
				turnaround_time+=tick_time-currentCustomer->getArrivalTime();
				total_seats_sold+=1;
				seated+=1;
			}
			else
			{
				this->logEvent(tick, "Customer " + to_string(currentCustomer->getID()) + " assigned a seat!");
				currentCustomer->setCompletionTime(tick+currentCustomer->getDuration());
				high_sel_ind+=1; //increment from row 0 seat 0 further and further back from stage
				total_seats_sold+=1;
				seated+=1;
				turnaround_time+=tick_time-currentCustomer->getArrivalTime();
			}
		}
		// pthread_mutex_unlock(&mtex);
	}
}

int Seller::run(int numCustomers, int duration) {
	this->generateCustomers(numCustomers);  // generate customers for this run

	// for each tick in the run
	// completes last transaction even after duration ended
	for (int i = 0; i < duration || currentCustomer != NULL; i++) {

		// (1) check if any customers are freshly arrived in the queue
		// adds arrived customers to arrivedCustomers
		for (list<Customer>::iterator it = this->unarrivedCustomers.begin(); it != this->unarrivedCustomers.end();) {
			if (it->getArrivalTime() <= i) {
				this->arrivedCustomers.push(*it); // adds customer to arrivedCustomers
				//printEvent(i, "Customer " + to_string(it->getID()) + " arrived.");
				this->logEvent(i, "Customer " + to_string(it->getID()) + " arrived.");
				it = this->unarrivedCustomers.erase(it); // removes customer from unarrivedCustomers
			}
			else
				++it;
		}

		// (2) check if previously started customer is finished
		if (currentCustomer != NULL) {
			if (currentCustomer->getCompletionTime() <= i) { // if customer is now finished
				this->servicedCustomers.push(*currentCustomer);  // adds customer to finished queue
				// printEvent(i, "Customer " + to_string(currentCustomer->getID()) + " completed transaction!");
				this->logEvent(i, "Customer " + to_string(currentCustomer->getID()) + " completed transaction!");
				currentCustomer = NULL;  // now there is no active customer
			}
		}

		// (3) if need to start a new customer transaction at this tick
		// only starts a transaction if under the time limit
		if (currentCustomer == NULL && i < duration && !this->arrivedCustomers.empty()) {
			// TODO
			// 1. pull customer off arrivedCustomers
			// 2. try to purchase ticket
			// 3. do stuff depending if ticket purchase failed or succeeded
			// need print statements for:
			// 		customer is assigned a seat
			// 		customer is rejected

			// calculate completion time when customer starts being served

			currentCustomer = &(this->arrivedCustomers.front()); //pull next customer off queue
			this->arrivedCustomers.pop(); //remove customer from queue
			response_time+=i-currentCustomer->getArrivalTime();

			string ticket_id = this->sellerID + "_" + to_string(currentCustomer->getID());

			pthread_mutex_lock(&mtex);

			if(priceLevel == LOWPRICE) //low price 10 -> 1
			{
				if(total_seats_sold==100) {
					this->logEvent(i, "Customer " + to_string(currentCustomer->getID()) + " rejected transaction!");
					turned_away+=1;
				}
				else if(!tickman->trySellTicket(ticket_id,low_sel_ind%10,floor(low_sel_ind/10)))
				{
					while(!tickman->trySellTicket(ticket_id,low_sel_ind%10,floor(low_sel_ind/10)))
					{
						low_sel_ind-=1;
					}
					this->logEvent(i, "Customer " + to_string(currentCustomer->getID()) + " assigned a seat!");
					currentCustomer->setCompletionTime(i+currentCustomer->getDuration());
					total_seats_sold+=1;
					seated+=1;
					turnaround_time+=i-currentCustomer->getArrivalTime();
				}
				else
				{
					this->logEvent(i, "Customer " + to_string(currentCustomer->getID()) + " assigned a seat!");
					currentCustomer->setCompletionTime(i+currentCustomer->getDuration());
					low_sel_ind-=1; //decrement seats from last row last seat
					total_seats_sold+=1;
					seated+=1;
					turnaround_time+=i-currentCustomer->getArrivalTime();
				}
			}
			else if(priceLevel == MIDPRICE) //medium price 5 -> 6 -> 4 -> 7
			{
				if(total_seats_sold==100) {
					this->logEvent(i, "Customer " + to_string(currentCustomer->getID()) + " rejected transaction!");
					turned_away+=1;
				}
				else if(!tickman->trySellTicket(ticket_id,med_sel_ind%10,floor(med_sel_ind/10))) //if ticket is not available
				{
					while(!tickman->trySellTicket(ticket_id,med_sel_ind%10,floor(med_sel_ind/10)))
					{
						if(med_sel_ind==69) //seat rotation as specified in lab doc (row 5, row 6, row 4, etc)
							med_sel_ind=40;
						else if(med_sel_ind==49)
							med_sel_ind=70;
						else if(med_sel_ind==79)
							med_sel_ind=30;
						else if(med_sel_ind==39)
							med_sel_ind=80;
						else if(med_sel_ind==89)
							med_sel_ind=20;
						else if(med_sel_ind==29)
							med_sel_ind=90;
						else if(med_sel_ind==99)
							med_sel_ind=10;
						else if(med_sel_ind==19)
							med_sel_ind=0;
						else
							med_sel_ind+=1; //increment seat within a row and use above conditions to switch rows accordingly
					}
					this->logEvent(i, "Customer " + to_string(currentCustomer->getID()) + " assigned a seat!");
					currentCustomer->setCompletionTime(i+currentCustomer->getDuration());
					total_seats_sold+=1;
					seated+=1;
					turnaround_time+=i-currentCustomer->getArrivalTime();
				}
				else
				{
					this->logEvent(i, "Customer " + to_string(currentCustomer->getID()) + " assigned a seat!");
					currentCustomer->setCompletionTime(i+currentCustomer->getDuration());
					if(med_sel_ind==69) //seat rotation as specified in lab doc (row 5, row 6, row 4, etc)
						med_sel_ind=40;
					else if(med_sel_ind==49)
						med_sel_ind=70;
					else if(med_sel_ind==79)
						med_sel_ind=30;
					else if(med_sel_ind==39)
						med_sel_ind=80;
					else if(med_sel_ind==89)
						med_sel_ind=20;
					else if(med_sel_ind==29)
						med_sel_ind=90;
					else if(med_sel_ind==99)
						med_sel_ind=10;
					else if(med_sel_ind==19)
						med_sel_ind=0;
					else
						med_sel_ind+=1; //increment seat within a row and use above conditions to switch rows accordingly
					total_seats_sold+=1;
					seated+=1;
					turnaround_time+=i-currentCustomer->getArrivalTime();
				}
			}
			else if(priceLevel == HIGHPRICE) //high price 1 -> 10
			{
				if(total_seats_sold==100) {
					this->logEvent(i, "Customer " + to_string(currentCustomer->getID()) + " rejected transaction!");
					turned_away+=1;
				}
				else if(!tickman->trySellTicket(ticket_id,high_sel_ind%10,floor(high_sel_ind/10))) //if ticket is not available
				{
					while(!tickman->trySellTicket(ticket_id,high_sel_ind%10,floor(high_sel_ind/10)))
					{
						high_sel_ind+=1;
					}
					this->logEvent(i, "Customer " + to_string(currentCustomer->getID()) + " assigned a seat!");
					currentCustomer->setCompletionTime(i+currentCustomer->getDuration());
					total_seats_sold+=1;
					seated+=1;
					turnaround_time+=i-currentCustomer->getArrivalTime();
				}
				else
				{
					this->logEvent(i, "Customer " + to_string(currentCustomer->getID()) + " assigned a seat!");
					currentCustomer->setCompletionTime(i+currentCustomer->getDuration());
					high_sel_ind+=1; //increment from row 0 seat 0 further and further back from stage
					total_seats_sold+=1;
					seated+=1;
					turnaround_time+=i-currentCustomer->getArrivalTime();
				}
			}
			pthread_mutex_unlock(&mtex);
		}
	}

	return 0;
}


vector<int> Seller::print_event_log() {
	cout << "------------------------------------------" << endl;
	cout << "Seller " << this->sellerID << " event log: " << endl;
	cout << "------------------------------------------" << endl;
	for (list<string>::iterator it = this->eventLog.begin(); it != this->eventLog.end(); it++) {
		cout << *it;
	}
	vector<int> ret;
	ret.push_back(priceLevel);
	ret.push_back(seated);
	ret.push_back(turned_away);

	int total = 0;
	int length = response_times.size();
        int length_2 = turnaround_times.size();
	for (list<int>::iterator it = this->response_times.begin(); it != this->response_times.end(); it++) {
		total += *it;
	}
	response_time = total;
	printf("response time: %d", response_time);
	printf("average_response_time: %f\n", 1.0 * total/length);

	total = 0;
	for (list<int>::iterator it = this->turnaround_times.begin(); it != this->turnaround_times.end(); it++) {
		total += *it;
	}
	turnaround_time = total;
	printf("turnaround time: %d", turnaround_time);
	printf("average turnaround time: %f\n", 1.0 * total/length_2);
	ret.push_back(turnaround_time);
	ret.push_back(response_time);
	ret.push_back(length);
        ret.push_back(length_2);

	return ret;
}



void printEvent(int time, string message) {
	int hours = time / 60;
	int minutes = time % 60;

	cout << hours << ":" << minutes << " - " << message << endl;
}
void Seller::logEvent(int time, string message) {
	int hours = time / 60;
	int minutes = time % 60;

	string event = to_string(hours) + ":" + to_string(minutes) + " - " + message + "\n";
	this->eventLog.push_back(event);
}

bool Seller::has_active_customer() {
	if (this->currentCustomer == NULL)
		return false;
	return true;
}
