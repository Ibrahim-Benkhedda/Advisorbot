#pragma once
#include "OrderBookEntry.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class CSVReader
{
	public:
		CSVReader();
		/**reads CSV file and returns a vector of OrderBookEntry objects */
		static std::vector<OrderBookEntry> readCSV(const std::string& csvFile);
		/** returns a vector of tokens */ 
		static std::vector<std::string> tokenise(std::string csvLine, char separator);
		/** converts a set of strings to an OrderBookEntry object */
		static OrderBookEntry stringsToOBE(
			std::string price,
			std::string amount,
			std::string timestamp,
			std::string product,
			OrderBookType OrderBookType
		);

	private:

		/** converts a vector of strings to an OrderBookEntry object */
		static OrderBookEntry stringsToOBE(std::vector<std::string> tokens);
};

