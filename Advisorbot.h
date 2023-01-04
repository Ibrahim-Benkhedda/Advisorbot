#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "OrderBook.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>


class Advisorbot
{
	public:
		Advisorbot();
		void init();

	private:
		/** gets the commands from the user, return it as a vector of tokens of command*/
		std::vector<std::string> parseUserCommands();
		/** gets the user input command, and tries to match it with a function based on the input */
		void matchCommands(const std::vector<std::string>& userCommands);
		/** deals with orders */
		void printHelp();
		void getMin();
		void getMax();
		void getAverage();
		void getProduct();
		void predictMarket();
		void getTime();

		OrderBook orderBook{ "20200601.csv" };

		/** type alias that represents a pointer to a member function of Advisorbot that returns void */
		using AdvisorbotFunctionPointer = void (Advisorbot::*)();

		

};

