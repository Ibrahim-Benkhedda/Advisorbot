#include "Advisorbot.h"

Advisorbot::Advisorbot() {
    init(); 
}

void Advisorbot::init() {
    std::cout << ">>> Welcome to Advisorbot." << std::endl;
    std::cout << ">>> Use 'help' to list all available commands or 'help [command]' about the specific command." << std::endl;
    currentTime = orderBook.getEarliestTime();

    while (true) {
        availableProducts = orderBook.getKnownProducts();
        std::vector<std::string> parsedCommands = parseUserCommands();


        /** Print the user input command */
        std::string command;
        for (const auto& command : parsedCommands) {
            std::cout << "commands: " << command << std::endl;
        }

        matchCommands(parsedCommands);
    }
    
}

std::vector<std::string> Advisorbot::parseUserCommands() {
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

void Advisorbot::printHelp() {
    std::cout << "The available commands are help, help [command] and the following commands:" << std::endl;
    std::cout << "prod" << std::endl;
    std::cout << "min [product] bid/ask" << std::endl;
    std::cout << "max [product] bid/ask" << std::endl;
    std::cout << "avg [product] bid/ask" << std::endl; 
    std::cout << "predict [product] bid/ask " << std::endl;
    std::cout << "time" << std::endl;
    std::cout << "step" << std::endl;
}
void Advisorbot::getMin(std::string userProduct, OrderBookType userOrderType, std::string userOrderTypeStr) {
    // get the prodct type, get ordertype and convert it to OrderBookType and process it

    // see whether the user ask or bid 
    std::vector<OrderBookEntry> entries = orderBook.getOrders(
        userOrderType, userProduct, currentTime
    );

    std::cout << "Min " << userOrderTypeStr << " of: " << userProduct << " " << OrderBook::getLowPrice(entries) << std::endl;
}

// void Advisorbot::getMax(std::string product, OrderBookType orderType)
void Advisorbot::getMax(std::string userProduct, OrderBookType userOrderType, std::string userOrderTypeStr) {

    // see whether the user ask or bid 
    std::vector<OrderBookEntry> entries = orderBook.getOrders(
        userOrderType, userProduct, currentTime
    );

    std::cout << "Max " << userOrderTypeStr << " of: " << userProduct << " " << OrderBook::getHighPrice(entries) << std::endl;

}
void Advisorbot::getProduct() {
    std::cout << " list of available products: " << std::endl;
    availableProducts = orderBook.getKnownProducts();
    // prints all available products
    for (std::string const& product : availableProducts) {
        std::cout << product << ", ";
    }
    std::cout << std::endl;
}

void Advisorbot::getTime() {
    std::cout << "current time: " << currentTime << std::endl;
}

void Advisorbot::moveToNextTimeStep() {
    currentTime = orderBook.getNextTime(currentTime);
    std::cout << "now at: " << currentTime << std::endl;  
}

double Advisorbot::getAverage(std::string product, OrderBookType orderType, int steps) {
    
    // Get the ask prices for the product and order type specified by the user
    std::vector<double> entries = getEntriesByTimestep(product, orderType, steps);

    // Calculate the sum of the entries prices
    double sum = 0;
    for (double price : entries) {
        sum += price;
    }

    // Return the average of the ask prices over the last N timesteps
    return sum / steps;
}

double Advisorbot::predictMarket(std::string product, OrderBookType orderType, int steps) {
    // gets the prices of the specific product, order type (bid/ask) by the timesteps that the user provided.
    std::vector<double> entries = getEntriesByTimestep(product, orderType, steps);
    // https://nullbeans.com/how-to-calculate-the-exponential-moving-average-ema/
    // following the article, here's  how we are going to approach the Exponential Moving Average (EMA)
    // First, calculate the smoothing factor by using the formula : Smoothing Factor = 2 / (number of time periods + 1)
    // Next, calculate the first intermediate EMA value by using the first data point in the set
    // We use the following formula to calculate the EMA : EMA(current) = (current value x smoothing factor) + (previous EMA x(1 - smoothing factor))
    // Continue this process for the entire set of data points
    // The final EMA value is the last value calculated in this process, which represents the moving average of the data set over the specified number of time periods.
    
    double currentEMA = 0;
    double previousEMA = entries[0];
    // calculate the smoothing factor by using the formula : Smoothing Factor = 2 / (number of time periods + 1)
    double smoothingFactor = 2.0 / (steps + 1);
    // For each subsequent data point
    for (double price : entries) {
        // We use the following formula to calculate the EMA : EMA(current) = (current value x smoothing factor) + (previous EMA x(1 - smoothing factor))
        currentEMA = (price * smoothingFactor) + (previousEMA * (1 - smoothingFactor));
        previousEMA = currentEMA;
    }
    // returns the current EMA, which represents the moving average of entries
    return currentEMA;
}

std::vector<double> Advisorbot::getEntriesByTimestep(const std::string& product, OrderBookType orderType, int n) {
    std::vector<double> prices;
    std::vector<OrderBookEntry> entries = orderBook.getOrders(orderType, product, currentTime);

    // Iterate through the entries and add the prices to the `prices` vector
    for (const OrderBookEntry& entry : entries) {
        prices.push_back(entry.price);
    }

    return prices;
}

void Advisorbot::matchCommands(const std::vector<std::string>& userCommands) {
    // inspired from c++ - How to store methods as function pointers in a map container? - Stack Overflow
    // https://stackoverflow.com/questions/53336880/how-to-store-methods-as-function-pointers-in-a-map-container
    // https://stackoverflow.com/questions/2136998/using-a-stl-map-of-function-pointers
 
    // hash map that contains all the possible commands for advisorbot
    // AdvisorbotFunctionPointer is pointer  which is used to call the member functions in Advisorbot.
    std::unordered_map<std::string, AdvisorbotFunctionPointer> commandsMap = {
        {"help", &Advisorbot::processHelpCommand},
        {"prod", &Advisorbot::processProdCommand},
        {"min", &Advisorbot::processMinCommand},
        {"max", &Advisorbot::processMaxCommand},
        {"avg", &Advisorbot::processAvgCommand},
        {"time", &Advisorbot::processTimeCommand},
        {"step", &Advisorbot::processStepCommand},
        {"predict", &Advisorbot::processPredictCommand}
    };
    
    std::string command = userCommands[0];
    
    AdvisorbotFunctionPointer processCommand = commandsMap[command];
    if (processCommand)
        (this->*processCommand)(userCommands);
    else
        std::cout << "Invalid command..." << std::endl;
}

bool Advisorbot::isValidProduct(const std::string& product) {
    // Check if the product is in the list of available products
    for (auto const& p : availableProducts) {
        if (p == product) {
            return true;
        }
    }
    std::cout << "The product does not exist" << std::endl;
    return false;
}

bool Advisorbot::isValidBidAsk(const std::string& bidAsk) {
    // Check if the bidAsk is "bid" or "ask"
    return (bidAsk == "bid" || bidAsk == "ask");
}



void Advisorbot::processHelpCommand(const std::vector<std::string>& userCommands) {
    // stores all the commands help text
    std::map<std::string, std::string> commandsHelpText = {
        {"min", "The min command finds the minimum bid or ask for a product in the current time step."},
        {"max", "The max command finds the maximum bid or ask for a product in the current time step."},
        {"avg", "The avg command finds the average bid or ask for a product in the current time step."},
        {"prod", "The prod command lists all available products."},
        {"predict", "The predict command predicts the price of a product at a specific time."},
        {"time", "The time command sets the current time for the program."},
        {"step", "The step command advances the current time by a specific number of steps."}
    };

    // validates and execute the help command
    if (userCommands[0] == "help") {
        if (userCommands.size() == 1) {
            printHelp();
        }
        else if (userCommands.size() == 2 && userCommands[1] != "help" && commandsHelpText.count(userCommands[1])) {
            // If there's a second command that is not "help" and it's in the map
            std::cout << commandsHelpText[userCommands[1]] << std::endl;
        }
        else {
            std::cout << "Invalid Argument..." << std::endl;
        }
    }
}


void Advisorbot::processProdCommand(const std::vector<std::string>& userCommands) {
    if (userCommands[0] == "prod" && userCommands.size() == 1) {
        getProduct();
    }
}


void Advisorbot::processMinCommand(const std::vector<std::string>& userCommands) {
    if (userCommands[0] == "min" && userCommands.size() == 3 && isValidProduct(userCommands[1]) && isValidBidAsk(userCommands[2])) {
        std::string parsedProduct = userCommands[1];
        std::string parsedOrderTypeStr = userCommands[2];
        OrderBookType parsedOrderType = OrderBookEntry::stringToOrderBookType(userCommands[2]);
        getMin(parsedProduct, parsedOrderType, parsedOrderTypeStr);
    }
}

void Advisorbot::processMaxCommand(const std::vector<std::string>& userCommands) {
    if (userCommands[0] == "max" && userCommands.size() == 3 && isValidProduct(userCommands[1]) && isValidBidAsk(userCommands[2])) {
        // If the first command is "min", there are three arguments, and the second and third are valid product and bid/ask, respectively, call the corresponding function
        std::string parsedProduct = userCommands[1];
        std::string parsedOrderTypeStr = userCommands[2];
        OrderBookType parsedOrderType = OrderBookEntry::stringToOrderBookType(userCommands[2]);
        getMax(parsedProduct, parsedOrderType, parsedOrderTypeStr);
    }
}

void Advisorbot::processAvgCommand(const std::vector<std::string>& userCommands) {
    if (userCommands[0] == "avg" && userCommands.size() == 4 && isValidProduct(userCommands[1]) && isValidBidAsk(userCommands[2]) && isValidNumber(userCommands[3])) {
        std::string parsedProduct = userCommands[1];
        std::string parsedOrderTypeStr = userCommands[2];
        OrderBookType parsedOrderType = OrderBookEntry::stringToOrderBookType(userCommands[2]);
        int steps = std::stoi(userCommands[3]); // convert the third argument of the avg command into a integer
        std::cout << "The average " << parsedProduct << " " << parsedOrderTypeStr << " price over the last " << steps << " timesteps was " << getAverage(parsedProduct, parsedOrderType, steps) << std::endl;
    }
}

void Advisorbot::processTimeCommand(const std::vector<std::string>& userCommands) {
    if (userCommands[0] == "time" && userCommands.size() == 1) {
        getTime();
    }
}

void Advisorbot::processStepCommand(const std::vector<std::string>& userCommands) {
    if (userCommands[0] == "step" && userCommands.size() == 1) {
        moveToNextTimeStep();
    }
}
    

void Advisorbot::processPredictCommand(const std::vector<std::string>& userCommands) {
    if (userCommands[0] == "predict" && userCommands.size() == 4 && isValidProduct(userCommands[1]) && isValidBidAsk(userCommands[2]) && isValidNumber(userCommands[3])) {
        std::string parsedProduct = userCommands[1];
        std::string parsedOrderTypeStr = userCommands[2];
        OrderBookType parsedOrderType = OrderBookEntry::stringToOrderBookType(userCommands[2]);
        int steps = std::stoi(userCommands[3]); // convert the third argument of the avg command into a integer
        predictMarket(parsedProduct, parsedOrderType, steps);
    }
}


bool Advisorbot::isValidNumber(std::string parsedArgument) {
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

    