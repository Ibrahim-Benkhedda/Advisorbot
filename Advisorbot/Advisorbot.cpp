#include "Advisorbot.h"

Advisorbot::Advisorbot() {
    init(); 
}

void Advisorbot::init() {
    std::cout << ">>> Welcome to Advisorbot." << std::endl;
    std::cout << ">>> Use 'help' to list all available commands or 'help [command]' about the specific command." << std::endl;
    currentTime = orderBook.getEarliestTime();
    // temporarly for change 
    userProduct = "ETH/BTC";
    userOrderType = OrderBookType::ask;

    while (true) {
        availableProducts = orderBook.getKnownProducts();
        std::vector<std::string> userCommands = parseUserCommands();

        /** Print the user input command */
        std::string command;
        for (const auto& command : userCommands) {
            std::cout << "commands: " << command << std::endl;
        }

        matchCommands(userCommands);
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
    std::cout << "The available commands are help, help [command]:" << std::endl;
    std::cout << "help min" << std::endl;
    std::cout << "help max" << std::endl;
    std::cout << "help avg" << std::endl;
    std::cout << "help prod" << std::endl;
    std::cout << "help predict" << std::endl;
    std::cout << "help time" << std::endl;
    std::cout << "help step" << std::endl;
}
void Advisorbot::getMin() {
    // get the prodct type, get ordertype and convert it to OrderBookType and process it
    std::cout << "find minimum bid or ask for product in current time step" << std::endl;

    // see whether the user ask or bid 
    std::vector<OrderBookEntry> entries = orderBook.getOrders(
        userOrderType, userProduct, currentTime
    );

    std::cout << "Min " << userOrderTypeStr << " of: " << userProduct << " " << OrderBook::getLowPrice(entries) << std::endl;
}

// void Advisorbot::getMax(std::string product, OrderBookType orderType)
void Advisorbot::getMax() {
    std::cout << "find maximum bid or ask for product in current time step" << std::endl;

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
void Advisorbot::getAverage() {
    std::cout << "compute average ask or bid for the sent product over the sent number of time steps." << std::endl;

    std::vector<OrderBookEntry> entries = orderBook.getOrders(
        userOrderType, userProduct, currentTime
    );

    std::cout << "Average " << userOrderTypeStr << " of: " << userProduct << " " << OrderBook::getHighPrice(entries) << std::endl;
}

void Advisorbot::getTime() {
    std::cout << "state current time in dataset, i.e. which timeframe are we looking at" << std::endl;
    std::cout << "current time: " << currentTime << std::endl;
}

void Advisorbot::moveToNextTimeStep() {
    currentTime = orderBook.getNextTime(currentTime);
    std::cout << "now at: " << currentTime << std::endl;  
}

void Advisorbot::matchCommands(const std::vector<std::string>& userCommands) {
    // inspired from c++ - How to store methods as function pointers in a map container? - Stack Overflow
    // https://stackoverflow.com/questions/53336880/how-to-store-methods-as-function-pointers-in-a-map-container
    // 
    // map that contains the possible commands for advisorbot program
    // AdvisorbotFunctionPointer is pointer  which is used to call the member functions in Advisorbot.
    std::unordered_map<std::string, AdvisorbotFunctionPointer> commandsMap;

    // populate map
    commandsMap["help"] = &Advisorbot::printHelp;
    commandsMap["min"] = &Advisorbot::getMin;
    commandsMap["max"] = &Advisorbot::getMax;
    commandsMap["avg"] = &Advisorbot::getAverage;
    commandsMap["prod"] = &Advisorbot::getProduct;
    commandsMap["predict"];
    commandsMap["time"] = &Advisorbot::getTime;
    commandsMap["step"] = &Advisorbot::moveToNextTimeStep;

    if (userCommands[0] == "help") {
        if (userCommands.size() == 1) {
            // If there's no argument, call the "help" function
            if (commandsMap.count("help")) {
                (this->*commandsMap["help"])();
            }
            else {
                std::cout << "invalid input..." << std::endl;
            }
        }
        else if (userCommands.size() == 2 && userCommands[1] != "help" && commandsMap.count(userCommands[1])) {
            // If there's a second command that is not "help" and it's in the map, call the corresponding function
            (this->*commandsMap[userCommands[1]])();
        }
        else {
            std::cout << "invalid input..." << std::endl;
        }
    }
    else if (userCommands[0] == "prod" && userCommands.size() == 1) {
        (this->*commandsMap["prod"])();
    }
    else if (userCommands[0] == "min" && userCommands.size() == 3 && isValidProduct(userCommands[1]) && isValidBidAsk(userCommands[2])) {
        // If the first command is "min", there are three arguments, and the second and third are valid product and bid/ask, respectively, call the corresponding function
        userProduct = userCommands[1];
        userOrderType = OrderBookEntry::stringToOrderBookType(userCommands[2]);
        userOrderTypeStr = userCommands[2];
        (this->*commandsMap["min"])();
    }
    else if (userCommands[0] == "max" && userCommands.size() == 3 && isValidProduct(userCommands[1]) && isValidBidAsk(userCommands[2])) {
        // If the first command is "min", there are three arguments, and the second and third are valid product and bid/ask, respectively, call the corresponding function
        userProduct = userCommands[1];
        userOrderType = OrderBookEntry::stringToOrderBookType(userCommands[2]);
        userOrderTypeStr = userCommands[2];
        (this->*commandsMap["max"])();
    }
    else if (userCommands[0] == "avg" && userCommands.size() == 3 && isValidProduct(userCommands[1]) && isValidBidAsk(userCommands[2])) {
        userProduct = userCommands[1];
        userOrderType = OrderBookEntry::stringToOrderBookType(userCommands[2]);
        userOrderTypeStr = userCommands[2];
        (this->*commandsMap["avg"])();
    }
    else if (userCommands[0] == "time" && userCommands.size() == 1) {
        // If the first command is "min", there are three arguments, and the second and third are valid product and bid/ask, respectively, call the corresponding function
        (this->*commandsMap["time"])();
    }
    else if (userCommands[0] == "step" && userCommands.size() == 1) {
        // If the first command is "min", there are three arguments, and the second and third are valid product and bid/ask, respectively, call the corresponding function
        (this->*commandsMap["step"])();
    }
    else if (userCommands[0] == "predict" && userCommands.size() == 4 && isValidProduct(userCommands[1]) && isValidBidAsk(userCommands[2])) {
        userProduct = userCommands[1];
        userOrderType = OrderBookEntry::stringToOrderBookType(userCommands[2]);
        std::cout << "The average " << userProduct << " " << userCommands[2] << " price over the last 10 timesteps was " << predictMarket() << std::endl;
    }

    else {
            std::cout << "invalid input..." << std::endl;

    }
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


double Advisorbot::predictMarket() {
    // Set the window size for the moving average
    const int windowSize = 10;

    // Get the ask prices for the product and order type specified by the user
    std::vector<double> askbidPrices = getLastNOrderBookEntries(userProduct, OrderBookType::ask, windowSize);

    // Calculate the sum of the askbid prices
    double sum = 0;
    for (double price : askbidPrices) {
        sum += price;
    }

    // Return the average of the ask prices over the last `windowSize` timesteps
    return sum / windowSize;
}

std::vector<double> Advisorbot::getLastNOrderBookEntries(const std::string& product, OrderBookType orderType, int n) {
    std::vector<double> prices;
    std::vector<OrderBookEntry> entries = orderBook.getOrders(orderType, product, currentTime);

    // Iterate through the entries and add the prices to the `prices` vector
    for (const OrderBookEntry& entry : entries) {
        prices.push_back(entry.price);
    }

    return prices;
}





    // check if the token is a number
    // bool isNumber = true;
    // int number = 0;

    // try {
        // number = std::stoi(cmd);
        // return number;
    // }
    // catch (const std::invalid_argument& e) {
    //    isNumber = false;
    // }