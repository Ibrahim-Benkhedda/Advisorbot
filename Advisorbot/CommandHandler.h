#pragma once
#include "Advisorbot.h"

/** 
	The CommandHandler Class is child of the Advisorbot Class, meaning, all the protected and public data members and methods
	of Advisorbot class are accessible to CommandHandler class. 
	The CommandHandler Class takes the user input (commands) and tries to match the parsed commands by validating the input.
	if a command is matched with the possible commands of the bot. execute the commands by calling the function from Advisorbot.
*/

class CommandHandler : protected Advisorbot 
{
	public:
		/** Constructor Function of the Command Handler class */
		CommandHandler();
		/** gets the commands from the user, return it as a vector of tokens of command */
		std::vector<std::string> parseUserCommands();
		/** gets the user input command, and tries to match it with a function based on the input */
		void matchCommands(const std::vector<std::string>& userCommands);

		
	private:
		/** type alias that represents a pointer to a member function of CommandHandler that returns void */
		using CommandFunctionPointer = void(CommandHandler::*)(const std::vector<std::string>&);

		/** the following functions they validate and execute the command by calling function
		that is related to the command from the Advisorbot class */

		/** validate and run the help command */
		void processHelpCommand(const std::vector<std::string>& userCommands);
		/** validate and run the Min command */
		void processMinCommand(const std::vector<std::string>& userCommands);
		/** validate and run the Max command */
		void processMaxCommand(const std::vector<std::string>& userCommands);
		/** validate and run the Average command */
		void processAvgCommand(const std::vector<std::string>& userCommands);
		/** validate and run the Predict command */
		void processPredictCommand(const std::vector<std::string>& userCommands);
		/** validate and run the Products command */
		void processProdCommand(const std::vector<std::string>& userCommands);
		/** validate and run the time command */
		void processTimeCommand(const std::vector<std::string>& userCommands);
		/** validate and run the step command */
		void processStepCommand(const std::vector<std::string>& userCommands);
		/** validate and run the standard deviation command */
		void processStddevCommand(const std::vector<std::string>& userCommands);

		/** Check if the product is in the list of available products */
		bool isValidProduct(const std::string& product);
		/** Check if its a "bid" or "ask */
		bool isValidBidAsk(const std::string& bidAsk);
		/** checks if its a valid number/integer */
		bool isValidNumber(std::string parsedArgument);
};


