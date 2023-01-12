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
		*	if there's no time stamp. wraps around to the start */	
		std::string getNextTime(std::string timestamp);

		// static because it doesn't need to look at any data stored inside class ?????
		/** prints the highest price*/
		static double getHighPrice(std::vector<OrderBookEntry>& orders);
		static double getLowPrice(std::vector<OrderBookEntry>& orders);
		/** gets the mean of orders*/
		static double getMean(std::vector<OrderBookEntry>& orders);
		static double getMovingAverage(const std::string& product, OrderBookType orderType, std::string currentTime);

	private:
		/** stores orders*/
		std::vector<OrderBookEntry> orders;
};

