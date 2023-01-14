#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "OrderBook.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <time.h>


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
		void getMin(std::string userProduct, OrderBookType userOrderType, std::string userOrderTypeStr);
		void getMax(std::string userProduct, OrderBookType userOrderType, std::string userOrderTypeStr);
		double getAverage(std::string product, OrderBookType orderType, int steps);
		void getProduct();
		double predictMarket(std::string product, OrderBookType orderType, int steps);
		void getTime();
		void moveToNextTimeStep();

		OrderBook orderBook{ "20200601.csv" };
		

		/** type alias that represents a pointer to a member function of Advisorbot that returns void */
		using AdvisorbotFunctionPointer = void(Advisorbot::*)(const std::vector<std::string>&);
		/** Check if the product is in the list of available products */
		bool isValidProduct(const std::string& product);
		/** Check if its a "bid" or "ask */
		bool isValidBidAsk(const std::string& bidAsk);

		bool isValidNumber(std::string parsedArgument);

		void processHelpCommand(const std::vector<std::string>& userCommands);
		void processMinCommand(const std::vector<std::string>& userCommands);
		void processMaxCommand(const std::vector<std::string>& userCommands);
		void processAvgCommand(const std::vector<std::string>& userCommands);
		void processPredictCommand(const std::vector<std::string>& userCommands);
		void processProdCommand(const std::vector<std::string>& userCommands);
		void processTimeCommand(const std::vector<std::string>& userCommands);
		void processStepCommand(const std::vector<std::string>& userCommands);

		// data members 
		std::vector<std::string> availableProducts;
		std::string currentTime;

		std::vector<double> getEntriesByTimestep(const std::string& product, OrderBookType orderType, int n);


		

};

