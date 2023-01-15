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

/** 
	The Advisorbot is the main class of the whole application. contains all the functions
	that performes the specific action of the commands.
*/

class Advisorbot
{
	public:
		/** Constructor Function of the Advisorbot */
		Advisorbot();
		/** create instance of the Advisorbot class */ 
		void init();
	
	/** protected is members of the Advisorbot class that can be accessed by its derived classes (CommandHandler),
		also known as the child */ 
	protected:
		/** execute and performe the functionality of the specific command */

		/** prints all the available commands */
		void executePrintHelp();
		/** command that gets the lowest price of the current timestamp */
		void executeMin(std::string userProduct, OrderBookType userOrderType);
		/** command that gets the highest price of the current timestamp */
		void executeMax(std::string userProduct, OrderBookType userOrderType);
		/** command  that gets the avg of specified number of steps */
		void executeAverage(std::string product, OrderBookType orderType, int steps);
		/** lists all available products */
		void executeProduct();
		/** predicts the market using Exponential Moving Average */
		void executePredictMarket(std::string product, OrderBookType orderType, int steps);
		/** gets the current time */
		void executeTime();
		/** moves from the current time by one step */
		void executeNextTimeStep();
		/** compute the standard deviation over specified number of steps. If no number of steps is specified,
		    it will use the current time step.*/
		void executeStandardDeviation(std::string product, OrderBookType orderType, int steps = 0);

		// data members 
		std::vector<std::string> availableProducts;
		std::string currentTime;

		/** get the prices of the order book entries by the N timesteps */ 
		std::vector<double> getOBEPricesByTimestep(const std::string& product, OrderBookType orderType, int n);
		/** get the orders of the order book entries by the N timesteps */
		std::vector<OrderBookEntry> getOrdersByTimestep(const std::string& product, OrderBookType orderType, int n);

	private:
		
		OrderBook orderBook{ "20200601.csv" };



		


		

};

