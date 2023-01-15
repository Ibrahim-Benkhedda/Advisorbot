#include "CommandHandler.h"


CommandHandler::CommandHandler() {

}

std::vector<std::string> CommandHandler::parseUserCommands() {
    std::string userInput; // stores the user input 
    std::vector<std::string> commands; // stores the tokenised commands from the userInput

    std::getline(std::cin, userInput);
    // istringstream is better for handling user Input then the our tokenise function.
    // we use it to extract tokens from the string userInput using the getline function
    std::istringstream userInputStream(userInput);

    std::string cmd;
    // get the tokens from the userInputStream until there are no more to extract
    while (std::getline(userInputStream, cmd, ' '))
    {
        // checks if the command is not a space, if not, append the command to the commands vector
        if (!cmd.empty()) commands.push_back(cmd);
    }

    // returns the tokenised commands of the user input
    return commands;
}

void CommandHandler::matchCommands(const std::vector<std::string>& userCommands) {
    // inspired from c++ - How to store methods as function pointers in a map container? - Stack Overflow
    // https://stackoverflow.com/questions/53336880/how-to-store-methods-as-function-pointers-in-a-map-container
    // https://stackoverflow.com/questions/2136998/using-a-stl-map-of-function-pointers

    // The following code is a hash map (unordered_map) that contains all the possible commands for advisorbot.
    // CommandFunctionPointer is pointer to a member function of CommandHandler that returns void.
    std::unordered_map<std::string, CommandFunctionPointer> commandsMap = {
        {"help", &CommandHandler::processHelpCommand},
        {"prod", &CommandHandler::processProdCommand},
        {"min", &CommandHandler::processMinCommand},
        {"max", &CommandHandler::processMaxCommand},
        {"avg", &CommandHandler::processAvgCommand},
        {"time", &CommandHandler::processTimeCommand},
        {"step", &CommandHandler::processStepCommand},
        {"predict", &CommandHandler::processPredictCommand},
        {"stddev", &CommandHandler::processStddevCommand}
    };

    // stores the first parsed input as a command.
    std::string command = userCommands[0];

    // pointer to the member function to execute
    CommandFunctionPointer processCommand = commandsMap[command];
    // looks for the command in commandsMap. if the command is found, call the corresponding member function of the commandsMap.
    // else return print that tells the user its an invalid command.
    if (processCommand)
        (this->*processCommand)(userCommands);
    else
        std::cout << "Invalid command..." << std::endl;
}

void CommandHandler::processHelpCommand(const std::vector<std::string>& userCommands) {
    // stores all the help text for all available commands
    std::map<std::string, std::string> commandsHelpText = {
        {"min", "The min command finds the minimum bid or ask for a product in the current time step."},
        {"max", "The max command finds the maximum bid or ask for a product in the current time step."},
        {"avg", "The avg command finds the average bid or ask for a product in the current time step."},
        {"prod", "The prod command lists all available products."},
        {"predict", "The predict command predicts the price of a product at a specific time."},
        {"time", "The time command sets the current time for the program."},
        {"step", "The step command advances the current time by a specific number of steps."},
        {"stddev", "The stddev command computes the standard deviation of a product price at the current time step or over steps"}
    };

    // validates and execute the help command
    if (userCommands[0] == "help") {
        // if the user enters help only with no extra argument
        if (userCommands.size() == 1) {
            Advisorbot::executePrintHelp();
        }
        // if there's a second command (argument) that is not "help" and it's in the map.
        else if (userCommands.size() == 2 && userCommands[1] != "help" && commandsHelpText.count(userCommands[1])) {
            
            std::cout << commandsHelpText[userCommands[1]] << std::endl;
        }
        // if its not the in format of help [commands], return invalid argument since the user entered the correct first command.
        else {
            std::cout << "Invalid Argument..." << std::endl;
        }
    }
}


void CommandHandler::processProdCommand(const std::vector<std::string>& userCommands) {
    // validate if the first command is prod and the size of the commands is 1. execute the product command.
    if (userCommands[0] == "prod" && userCommands.size() == 1) {
        Advisorbot::executeProduct();
    }
}


void CommandHandler::processMinCommand(const std::vector<std::string>& userCommands) {
    /** validates and execute the min command.
        - checks if the first command is min.
        - checks if the size of the commands is 3
        - checks if the second command is a valid product.
        - checks if the third command is a valid order type.
    */

    if (userCommands[0] == "min" && userCommands.size() == 3 && isValidProduct(userCommands[1]) && isValidBidAsk(userCommands[2])) {
        std::string parsedProduct = userCommands[1];
        std::string parsedOrderTypeStr = userCommands[2];
        OrderBookType parsedOrderType = OrderBookEntry::stringToOrderBookType(userCommands[2]);
        Advisorbot::executeMin(parsedProduct, parsedOrderType, parsedOrderTypeStr);
    }
}

void CommandHandler::processMaxCommand(const std::vector<std::string>& userCommands) {
    /** validates and execute the max command.
        - checks if the first command is max.
        - checks if the size of the commands is 3
        - checks if the second command is a valid product.
        - checks if the third command is a valid order type.
    */
    if (userCommands[0] == "max" && userCommands.size() == 3 && isValidProduct(userCommands[1]) && isValidBidAsk(userCommands[2])) {
        // If the first command is "min", there are three arguments, and the second and third are valid product and bid/ask, respectively, call the corresponding function
        std::string parsedProduct = userCommands[1];
        std::string parsedOrderTypeStr = userCommands[2];
        OrderBookType parsedOrderType = OrderBookEntry::stringToOrderBookType(userCommands[2]);
        Advisorbot::executeMax(parsedProduct, parsedOrderType, parsedOrderTypeStr);
    }
}

void CommandHandler::processAvgCommand(const std::vector<std::string>& userCommands) {
    /** validates and execute the avg command.
        - checks if the first command is avg.
        - checks if the size of the commands is 4
        - checks if the second command is a valid product.
        - checks if the third command is a valid order type.
        - checks if the fourth command is a valid positive number.
    */

    if (userCommands[0] == "avg" && userCommands.size() == 4 && isValidProduct(userCommands[1]) && isValidBidAsk(userCommands[2]) && isValidNumber(userCommands[3])) {
        std::string parsedProduct = userCommands[1];
        std::string parsedOrderTypeStr = userCommands[2];
        OrderBookType parsedOrderType = OrderBookEntry::stringToOrderBookType(userCommands[2]);
        int steps = std::stoi(userCommands[3]); // convert the third argument of the avg command into a integer
        executeAverage(parsedProduct, parsedOrderType, steps);
    }
}

void CommandHandler::processTimeCommand(const std::vector<std::string>& userCommands) {
    // validate if the first command is time and the size of the commands is 1. execute the time command.
    if (userCommands[0] == "time" && userCommands.size() == 1) {
        Advisorbot::executeTime();
    }
}

void CommandHandler::processStepCommand(const std::vector<std::string>& userCommands) {
    // validate if the first command is 'step' and the size of the commands is 1. execute the 'step' command.
    if (userCommands[0] == "step" && userCommands.size() == 1) {
        Advisorbot::executeNextTimeStep();
    }
}


void CommandHandler::processPredictCommand(const std::vector<std::string>& userCommands) {
    /** validates and execute the predict command.
        - checks if the first command is predict.
        - checks if the size of the commands is 4
        - checks if the second command is a valid product.
        - checks if the third command is a valid order type.
        - checks if the fourth command is a valid positive number.
    */

    if (userCommands[0] == "predict" && userCommands.size() == 4 && isValidProduct(userCommands[1]) && isValidBidAsk(userCommands[2]) && isValidNumber(userCommands[3])) {
        std::string parsedProduct = userCommands[1];
        std::string parsedOrderTypeStr = userCommands[2];
        OrderBookType parsedOrderType = OrderBookEntry::stringToOrderBookType(userCommands[2]);
        int steps = std::stoi(userCommands[3]); // convert the third argument of the avg command into a integer
        executePredictMarket(parsedProduct, parsedOrderType, steps);
    }
}

void CommandHandler::processStddevCommand(const std::vector<std::string>& userCommands) {
    /** validate and execute the 'stddev' command
        - checks if the size of the commands is either 4 or 3.          
        - if the size is 4, checks if the second argument is valid product,
          third argument is order type and fourth argument valid positive number.
        - if the size is 3. checks the same paramaters as the previous size but not include the steps as argument.
    */
    if (userCommands.size() == 4) {
        if (userCommands[0] == "stddev" && isValidProduct(userCommands[1]) && isValidBidAsk(userCommands[2]) && isValidNumber(userCommands[3])) {
            std::string parsedProduct = userCommands[1];
            std::string parsedOrderTypeStr = userCommands[2];
            OrderBookType parsedOrderType = OrderBookEntry::stringToOrderBookType(userCommands[2]);
            int steps = std::stoi(userCommands[3]); // convert the third argument of the avg command into a integer
            Advisorbot::executeStandardDeviation(parsedProduct, parsedOrderType, steps);

        }
    }
    else if (userCommands.size() == 3) {
        if (userCommands[0] == "stddev" && isValidProduct(userCommands[1]) && isValidBidAsk(userCommands[2])) {
            std::string parsedProduct = userCommands[1];
            std::string parsedOrderTypeStr = userCommands[2];
            OrderBookType parsedOrderType = OrderBookEntry::stringToOrderBookType(userCommands[2]);
            Advisorbot::executeStandardDeviation(parsedProduct, parsedOrderType);
        }
    }
    else {
        std::cout << "Invalid input..." << std::endl;
    }
}

bool CommandHandler::isValidNumber(std::string parsedArgument) {
    // check if the token is a number
    int number = 0;

    try {
        number = std::stoi(parsedArgument);
        std::cout << number << std::endl;
        return true;
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Invalid Argument..." << std::endl;
        std::cout << "The timestep that you provided as an argument is not an integer..." << std::endl;
        return false;
    }
}

bool CommandHandler::isValidProduct(const std::string& product) {
    // Check if the product is in the list of available products
    for (auto const& p : Advisorbot::availableProducts) {
        if (p == product) {
            return true;
        }
    }
    std::cout << "The product does not exist" << std::endl;
    return false;
}

bool CommandHandler::isValidBidAsk(const std::string& bidAsk) {
    // Check if the bidAsk is "bid" or "ask"
    return (bidAsk == "bid" || bidAsk == "ask");
}
