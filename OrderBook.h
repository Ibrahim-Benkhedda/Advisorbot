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
	private:
		/** stores orders*/
		std::vector<OrderBookEntry> orders;
};

