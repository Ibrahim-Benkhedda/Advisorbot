#include "OrderBookEntry.h"

OrderBookEntry::OrderBookEntry(double _price,
    double _amount,
    std::string _timestamp,
    std::string _product,
    OrderBookType _orderType,
    std::string _username
)
    : price(_price),
    amount(_amount),
    timestamp(_timestamp),
    product(_product),
    orderType(_orderType)
{

}

OrderBookType OrderBookEntry::stringToOrderBookType(std::string s) {
    if (s == "ask") {
        return OrderBookType::ask;
    }
    if (s == "bid") {
        return OrderBookType::bid;
    }

    return OrderBookType::unknown;

}

std::string OrderBookEntry::OrderBookTypeToString(const OrderBookType& ordertype){
    if (ordertype == OrderBookType::ask) {
        return "ask";
    }
    if (ordertype == OrderBookType::bid) {
        return "bid";
    }

    return "unknown";
}
