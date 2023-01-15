#include "CSVReader.h"
#include <chrono>



CSVReader::CSVReader() {

}

std::vector<OrderBookEntry> CSVReader::readCSV(const std::string& csvFilename) {
	auto start = std::chrono::high_resolution_clock::now();
	std::vector<OrderBookEntry> entries;

	std::ifstream csvFile{ csvFilename }; // open the file
	std::string line;

	while (std::getline(csvFile, line)) {
		try {
			OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
			entries.push_back(obe);
		}
		catch (const std::exception& e) {
			std::cout << "CSVreader::readCSV bad data" << std::endl;
		}
	}

	// prints how much we read in
	std::cout << "Total entries read: " << entries.size() << "entries" << std::endl;

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Time taken: " << duration.count() << " ms" << std::endl;

	return entries;
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator) {

	// stores tokens of strings in a vector
	std::vector<std::string> tokens;
	signed int start, end; // variable to store the position of the token
	std::string token; // temp variable to store current token
	start = csvLine.find_first_not_of(separator, 0); // starts at 0

	do {
		end = csvLine.find_first_of(separator, start);
		if (start == csvLine.length() || start == end) break;
		if (end >= 0) {
			// we found the separator
			token = csvLine.substr(start, end - start);
		}
		else {
			// end is invalid
			token = csvLine.substr(start, csvLine.length() - start);
		}

		tokens.push_back(token);
		start = end + 1;
	} while (end != std::string::npos); //  find_first_not_of returns std::string::npos if it does not find the search term


	return tokens;
}

 OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens) {

	double price, amount;

	if (tokens.size() != 5) {
		std::cout << "error" << std::endl;
		throw std::exception{};
	}
	// we have 5 tokens 
	try {
		price = std::stod(tokens[3]);  // convert string to double
		amount = std::stod(tokens[4]); // convert string to double
	}
	catch (const std::exception& e) {
		std::cout << "bad float number!" << tokens[3] << std::endl;
		std::cout << "bad float number!" << tokens[4] << std::endl;
		throw;
	}

	OrderBookEntry obe{
		price,
		amount,
		tokens[0],
		tokens[1],
		OrderBookEntry::stringToOrderBookType(tokens[2])
	};

	return obe;
}
