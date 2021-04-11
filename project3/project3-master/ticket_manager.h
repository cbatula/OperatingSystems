#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <array>

class TicketManager
{
private:
	const static int COLS = 10;
	const static int ROWS = 10;

	// Each seat has a lock so it can be sold without being interupted
	std::array<std::mutex, COLS * ROWS> seat_locks;

	// Each seat can be sold by any individual, this tracks the ids of the seller
	std::array<std::string, COLS * ROWS> seat_state;

	// Checks if a seat is available (not thread safe)
	bool isSold(size_t offset) const;

public:
	TicketManager();

	/* 	trySellTicket - Attempt to sell a ticket. This does not lock the seat
	*  			unless it can sell it so it can be called on sold seats 
	*			without issue. This operation is thread safe.
	*
	* 	seller_id - A unique id for the seller selling the seat. Not empty.
	*	col - seat column
	*	row - seat row
	*
	*	returns - true if the seat was sold to `seller_id`, false otherwise.
	*/
	bool trySellTicket(const std::string &seller_id, size_t col, size_t row);

	// print helper
	void print(std::ostream &ostr) const;
};

std::ostream &operator<<(std::ostream &ostr, const TicketManager &manager);
