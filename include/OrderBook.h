#pragma once

#include <cstdint>
#include <unordered_map>
#include <map>

enum class Side {
    BUY,
    SELL
};

struct Order {
    uint64_t id;
    Side side;
    uint64_t price;
    uint32_t quantity;
    
    Order* prev = nullptr;
    Order* next = nullptr;
};

struct PriceLevel {
    Order* head = nullptr;
    Order* tail = nullptr;
    uint64_t volume = 0;
};

class OrderBook {
private:
    std::map<uint64_t, PriceLevel, std::greater<uint64_t>> bids;
    std::map<uint64_t, PriceLevel> asks;
    std::unordered_map<uint64_t, Order*> orderMap;

public:
    OrderBook() = default;
    ~OrderBook() = default;

    void addOrder(uint64_t id, Side side, uint64_t price, uint32_t quantity);
    void cancelOrder(uint64_t id);
    void printBook() const;
    
private:
    void removeOrderFromList(Order* order, PriceLevel& level);
    void appendOrderToList(Order* order, PriceLevel& level);
};