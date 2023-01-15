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

// we don't need the static keyword when implementing the function
double OrderBook::computeHighPrice(std::vector<OrderBookEntry>& orders) {
	double max = orders[0].price;
	for (OrderBookEntry& e : orders) {
		if (e.price > max) {
			// update highest price
			max = e.price;
		}
	}

	return max;
}

double OrderBook::computeLowPrice(std::vector<OrderBookEntry>& orders) {
	double min = orders[0].price;
	for (OrderBookEntry& e : orders) {
		if (e.price < min) {
			min = e.price;
		}
	}

	return min;
}

double OrderBook::computeMean(std::vector<OrderBookEntry>& orders) {
	double sum = 0;
	for (const OrderBookEntry& e : orders) {
		sum = sum + e.price;
	}

	return sum / orders.size();
}

double OrderBook::computeStandardDeviation(std::vector<OrderBookEntry>& orders) {
	// The formula for standard deviation is:
	// sigma = sqrt( sum((x_i - mean)^2) / n )

	double mean = computeMean(orders);
	double sum = 0;

	// calculate the variance
	for (const OrderBookEntry& e : orders) {
		sum += pow(e.price - mean, 2);
	}

	//calculate the standard deviation
	double standardDeviation = sqrt(sum / orders.size());
	return standardDeviation;
}



/** returns vector of Orders according to the sent filters */
std::vector<OrderBookEntry> OrderBook::getOrders(
	OrderBookType type,
	std::string product,
	std::string timestamp
) {
	// use the arguments to select which orders from the orderBook to send back
	std::vector<OrderBookEntry> orders_sub;
	for (OrderBookEntry& e : orders) {
		if (e.orderType == type && e.product == product && e.timestamp == timestamp) {
			orders_sub.push_back(e);
		}
	}

	return orders_sub;
}

std::string OrderBook::getEarliestTime() {
	return orders[0].timestamp;
}

std::string OrderBook::getNextTime(std::string timestamp) {
	std::string next_timestamp;
	for (OrderBookEntry& e : orders) {
		if (e.timestamp > timestamp) {
			next_timestamp = e.timestamp;
			break;
		}
	}

	if (next_timestamp == "") {
		next_timestamp = orders[0].timestamp;
	}

	return next_timestamp;
}
