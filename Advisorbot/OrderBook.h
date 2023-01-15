#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"

#include <string>
#include <vector>
#include <map>

/** The OrderBook class presents a high level interface on all
	the orders in the order book data set, including query functions to find out
	which products are in the data set, and to retrieve a subset of the orders
	using filters.
*/
class OrderBook
{
	public:
		/** construct of OrderBook class, takes argument as a filename as a string, reaing a csv data file */
		OrderBook(std::string filename);
		/** returns the list of products in the dataset as a vector of strings */
		std::vector<std::string> getKnownProducts();

		/** returns vector of Orders according to the sent filters */
		std::vector<OrderBookEntry> getOrders(
			OrderBookType type,
			std::string product,
			std::string timestamp
		);

		/** returns the earliest time in the orderbook*/
		std::string getEarliestTime();

		/** returns the next time after the sent time in the orderbook.
			if there's no time stamp. wraps around to the start */	
		std::string getNextTime(std::string timestamp);

		/** calculates the highest price of given orders */
		static double computeHighPrice(std::vector<OrderBookEntry>& orders);
		/** calculates the lowest price of given orders  */
		static double computeLowPrice(std::vector<OrderBookEntry>& orders);
		/** calculates the mean of orders of given orders */
		static double computeMean(std::vector<OrderBookEntry>& orders);
		/** calculates the standard deviation of given orders */
		static double computeStandardDeviation(std::vector<OrderBookEntry>& orders);

	private:
		/** stores orders*/
		std::vector<OrderBookEntry> orders;
};

