all: 
	#g++ -pthread *.cpp -o ticketSales
	g++ -pthread Seller.cpp main.cpp Customer.cpp ticket_manager.cpp -o ticketSales

test:
	g++ -pthread Seller.cpp test_main.cpp Customer.cpp ticket_manager.cpp -o test_main

clean:
	rm ticketSales
