#pragma once
#include <string>
#include <iostream>

enum class OrderBookType { bid, ask, unknown };

/** The OrderBookEntry represents a row in the order book data set
    (i.e. a single order in the order book). It can be a bid or an ask order
*/
class OrderBookEntry
{
    public:
        /*constructor*/
        OrderBookEntry(double _price,
            double _amount,
            std::string _timestamp,
            std::string _product,
            OrderBookType _orderType,
            std::string username = "dataset"
        );

        /*converts string into an OrderBookType*/
        static OrderBookType stringToOrderBookType(std::string s);

        /** converts OrderBookType into string */
        static std::string OrderBookTypeToString(const OrderBookType& type);
        
        /*data members */
        double price;
        double amount;
        std::string timestamp;
        std::string product;
        OrderBookType orderType;
};

