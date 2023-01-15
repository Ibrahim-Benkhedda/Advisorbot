#include "Advisorbot.h"
#include "CommandHandler.h"

CommandHandler cmdHandler;

Advisorbot::Advisorbot() {
    init(); 
}

void Advisorbot::init() {
    std::cout << ">>> Welcome to Advisorbot." << std::endl;
    std::cout << ">>> Use 'help' to list all available commands or 'help [command]' about the specific command." << std::endl;
    currentTime = orderBook.getEarliestTime();

    // getOBEPricesByTimestep("ETH/BTC", OrderBookType::bid, 10);

    while (true) {
        availableProducts = orderBook.getKnownProducts();
        std::vector<std::string> parsedCommands = cmdHandler.parseUserCommands();


        /** Print the user input command */
        std::string command;
        for (const auto& command : parsedCommands) {
            std::cout << "commands: " << command << std::endl;
        }

       cmdHandler.matchCommands(parsedCommands);
    }
    
}

void Advisorbot::executePrintHelp() {
    std::cout << "The available commands are help, help [command] and the following commands:" << std::endl;
    std::cout << "prod" << std::endl;
    std::cout << "min [product] bid/ask" << std::endl;
    std::cout << "max [product] bid/ask " << std::endl;
    std::cout << "avg [product] bid/ask timesteps" << std::endl; 
    std::cout << "predict [product] bid/ask timesteps " << std::endl;
    std::cout << "time" << std::endl;
    std::cout << "step" << std::endl;
    std::cout << "stddev [product] bid/ask (optional: timesteps)" << std::endl;
}
void Advisorbot::executeMin(std::string userProduct, OrderBookType userOrderType, std::string userOrderTypeStr) {   
    std::vector<OrderBookEntry> entries = orderBook.getOrders(
        userOrderType, userProduct, currentTime
    );

    std::cout << "Min " << userOrderTypeStr << " of: " << userProduct << " " << OrderBook::computeLowPrice(entries) << std::endl;
}

// void Advisorbot::getMax(std::string product, OrderBookType orderType)
void Advisorbot::executeMax(std::string userProduct, OrderBookType userOrderType, std::string userOrderTypeStr) {

    std::vector<OrderBookEntry> entries = orderBook.getOrders(
        userOrderType, userProduct, currentTime
    );

    std::cout << "Max " << userOrderTypeStr << " of: " << userProduct << " " << OrderBook::computeHighPrice(entries) << std::endl;

}
void Advisorbot::executeProduct() {
    std::cout << " list of available products: " << std::endl;
    availableProducts = orderBook.getKnownProducts();
    // prints all available products
    for (std::string const& product : availableProducts) {
        std::cout << product << ", ";
    }
    std::cout << std::endl;
}

void Advisorbot::executeTime() {
    // gives the current time using the currentTime variable.
    std::cout << "current time: " << currentTime << std::endl;
}

void Advisorbot::executeNextTimeStep() {
    // moves to the next time
    currentTime = orderBook.getNextTime(currentTime);
    std::cout << "now at: " << currentTime << std::endl;  
}

void Advisorbot::executeAverage(std::string product, OrderBookType orderType, int steps) {
    
    // Get the ask prices for the product and order type specified by the user
    std::vector<double> entries = getOBEPricesByTimestep(product, orderType, steps);

    // Calculate the sum of the entries prices
    double sum = 0;
    for (double price : entries) {
        sum += price;
    }

    // Return the average of the ask prices over the last N timesteps
    double average =  sum / steps;
    std::cout << "The Average" << product << " " << "ask/bid" << " over the last " << steps << " : " << average << std::endl;
}

void Advisorbot::executePredictMarket(std::string product, OrderBookType orderType, int steps) {
    // https://nullbeans.com/how-to-calculate-the-exponential-moving-average-ema/
    // following the article, here's how we are going to compute the Exponential Moving Average (EMA)
    // First, calculate the smoothing factor by using the formula : Smoothing Factor = 2 / (number of time periods + 1)
    // Next, calculate the first intermediate EMA value by using the first data point in the set
    // We use the following formula to calculate the EMA : EMA(current) = (current value x smoothing factor) + (previous EMA x(1 - smoothing factor))
    // Continue this process for the entire set of data points
    // The final EMA value is the last value calculated in this process, which represents the moving average of the data set over the specified number of time periods.
   
    // gets the prices of the specific product, order type (bid/ask) by the timesteps that the user provided.
    std::vector<double> entries = getOBEPricesByTimestep(product, orderType, steps);

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
    // the current EMA, which represents the moving average of entries
    std::cout << "The Exponetial moving Average" << product << " " << "ask/bid" << " over the last " << steps << " : " << currentEMA << std::endl;
}

void Advisorbot::executeStandardDeviation(std::string product, OrderBookType orderType, int steps) {
    // if steps is provided, get orders by N steps, else get orders for the current time
    if (steps) {
        std::vector<OrderBookEntry> entries = getOrdersByTimestep(product, orderType, steps);

        std::cout << OrderBook::computeStandardDeviation(entries) << std::endl;
    }
    else {
        std::vector<OrderBookEntry> entries = orderBook.getOrders(
            orderType, product, currentTime
        );
        std::cout << OrderBook::computeStandardDeviation(entries) << std::endl;
    }

    
}

std::vector<double> Advisorbot::getOBEPricesByTimestep(const std::string& product, OrderBookType orderType, int n) {
    std::vector<double> prices;
    std::vector<OrderBookEntry> entries = getOrdersByTimestep(product, orderType, n);
        
    // Iterate through the entries and add the prices to the `prices` vector
    for (const OrderBookEntry& entry : entries) {
        prices.push_back(entry.price);  
    }

    return prices;
}

std::vector<OrderBookEntry> Advisorbot::getOrdersByTimestep(const std::string& product, OrderBookType orderType, int n) {
    std::vector<OrderBookEntry> entries;
    std::string time = currentTime;
    for (int i = 0; i < n; i++) {
        std::cout << "its working" << std::endl;
        std::vector<OrderBookEntry> currentEntries = orderBook.getOrders(orderType, product, time);
        time = orderBook.getNextTime(time);
        // adding entries from the current timestep in every iteration of the loop
        entries.insert(entries.end(), currentEntries.begin(), currentEntries.end());
    }

    return entries;
}


    