#include "Customer.h"
#include <iostream>

using namespace std;

// for testing purposes
int main2() {
	Customer c1 = Customer(0,0);

	cout << c1.getArrivalTime() << endl;
	cout << c1.getType() << endl;
	cout << c1.getDuration() << endl;

	c1.setSeat("H001");

	cout << c1.getSeat() << endl;
	return 0;
}



// Constructor
// Randomly generates the arival time (0-59)
// Randomly generates the type (0-2)
// 	0 = high priced buyer
// 	1 = medium priced buyer
// 	2 = low priced buyer
// Sets the duration time
Customer::Customer(int ID, int price_level) {
	this->ID = ID;
	arrivalTime = rand()%60;
	type = price_level;
	setDuration(type);
}

// Randomly generates a duration time based on
// the type
void Customer::setDuration(int t) {
	if (t == 0) {
		// High priced buyer
		duration = (rand() % 2) + 1;
	}
	else if (t == 1) {
		// Medium priced buyer
		duration = (rand() % 3) + 2;
	}
	else {
		// Low priced buyer
		duration = (rand() % 4) + 4;
	}
}

int Customer::getID() {
	return ID;
}

int Customer::getArrivalTime() {
	return arrivalTime;
}

int Customer::getType() {
	return type;
}

int Customer::getDuration() {
	return duration;
}

int Customer::getCompletionTime() {
	return completionTime;
}

string Customer::getSeat() {
	return seat;
}

void Customer::setCompletionTime(int time) {
	completionTime = time;
}

void Customer::setSeat(string s) {
	seat = s;
}

