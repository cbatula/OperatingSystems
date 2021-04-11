#include "ticket_manager.h"

bool TicketManager::isSold(size_t offset) const
{
	return !seat_state[offset].empty();
}

TicketManager::TicketManager()
{
}

bool TicketManager::trySellTicket(const std::string &seller_id, size_t col, size_t row)
{
	size_t offset = row * ROWS + col;
	if (isSold(offset))
	{
		return false;
	}

	// if this seat is already locked, it either is sold or will be sold
	bool couldLock = seat_locks[offset].try_lock();

	if (!couldLock)
	{	
		// Lock was not obtained here so no need to unlock
		return false;
	}

	// we now have the lock
	// check if the seat is sold now
	if (isSold(offset))
	{
		seat_locks[offset].unlock();
		return false;
	}

	// sell seat
	seat_state[offset] = seller_id;

	// release lock
	seat_locks[offset].unlock();
	return true;
}

static void print_string_padded(std::ostream &ostr, std::string str, size_t len)
{
	ostr << str;
	size_t remaining = len - str.length();
	for (size_t i = 0; i < remaining; i++)
	{
		ostr << ' ';
	}
}

static void print_divider(std::ostream &ostr, size_t len)
{
	for (size_t i = 0; i < len; i++)
	{
		ostr << '-';
	}
	ostr << std::endl;
}

void TicketManager::print(std::ostream &ostr) const
{
	// Figure out how much space to leave in each cell
	size_t max_len = 0;
	for (auto &&id : seat_state)
	{
		size_t len = id.length();
		if (len > max_len)
		{
			max_len = len;
		}
	}

	// Print each cell with padding as needed
	size_t line_len = (max_len + 3) * ROWS + 1;
	for (size_t row = 0; row < ROWS; row++)
	{
		print_divider(ostr, line_len);
		for (size_t col = 0; col < COLS; col++)
		{
			size_t offset = row * ROWS + col;
			ostr << "| ";
			print_string_padded(ostr, seat_state[offset], max_len);
			ostr << " ";
		}
		ostr << "|" << std::endl;
	}
	print_divider(ostr, line_len);
}

// We need private access so just pass to print method
std::ostream &operator<<(std::ostream &ostr, const TicketManager &manager)
{
	manager.print(ostr);
	return ostr;
}
