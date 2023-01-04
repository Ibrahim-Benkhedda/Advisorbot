#include "OrderBook.h"

OrderBook::OrderBook(std::string filename) {
	orders = CSVReader::readCSV(filename);
}

/** returns the list of products in the dataset as a vector of strings */
std::vector<std::string> OrderBook::getKnownProducts() {
	std::vector<std::string> products;

	// maps string to boolean
	std::map<std::string, bool> prodMap;

	for (OrderBookEntry& e : orders) {
		prodMap[e.product] = true;
	}

	// flattens the map to a vector of string
	// const because we don't need to change the data
	for (auto const& e : prodMap) {
		// gives us access to the key 
		products.push_back(e.first);
	}

	return products;
}

