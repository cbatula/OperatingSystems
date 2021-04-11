#ifndef CUSTOMER
#define CUSTOMER


#include <stdlib.h>
#include <string>

using namespace std;

class Customer {
	private:
		// Attributes
		int ID;
		int arrivalTime;
		int type;
		int duration;
		int completionTime;
		string seat;

		// Setter Method
		void setDuration(int);
	
	public:
		// Constructor
		Customer(int ID, int price_level);
		
		// Getter methods
		int getID();
		int getArrivalTime();
		int getType();
		int getDuration();
		string getSeat();
		int getCompletionTime();
		
		// Setter methods
		void setCompletionTime(int);
		void setSeat(string);
};

#endif