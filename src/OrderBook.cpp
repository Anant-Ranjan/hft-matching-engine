#include "OrderBook.h"
#include <iostream>
#include <iomanip>


void OrderBook::appendOrderToList(Order* order, PriceLevel& level) {
    if (level.head == nullptr) {
        level.head = order;
        level.tail = order;
    } else {
        order->prev = level.tail;
        level.tail->next = order;
        level.tail = order;
    }
    level.volume += order->quantity;
}

void OrderBook::removeOrderFromList(Order* order, PriceLevel& level) {
    if (order->prev) {
        order->prev->next = order->next;
    } else {
        level.head = order->next;
    }

    if (order->next) {
        order->next->prev = order->prev;
    } else {
        level.tail = order->prev;
    }

    level.volume -= order->quantity;
}

void OrderBook::addOrder(uint64_t id, Side side, uint64_t price, uint32_t quantity) {
    Order* newOrder = new Order{id, side, price, quantity, nullptr, nullptr};
    orderMap[id] = newOrder;

    if (side == Side::BUY) {
        appendOrderToList(newOrder, bids[price]);
    } else {
        appendOrderToList(newOrder, asks[price]);
    }
}
void OrderBook::cancelOrder(uint64_t id) {
    auto it = orderMap.find(id);
    if (it == orderMap.end()) {
        return;
    }

    Order* order = it->second;
    
    if (order->side == Side::BUY) {
        removeOrderFromList(order, bids[order->price]);
        if (bids[order->price].head == nullptr) {
            bids.erase(order->price);
        }
    } else {
        removeOrderFromList(order, asks[order->price]);
        if (asks[order->price].head == nullptr) {
            asks.erase(order->price);
        }
    }

    orderMap.erase(it);
    delete order;
}

void OrderBook::printBook() const {
    std::cout << "========================================\n";
    std::cout << "              ORDER BOOK                \n";
    std::cout << "========================================\n";
    
    std::cout << "[ASKS]\n";
    for (auto it = asks.rbegin(); it != asks.rend(); ++it) {
        std::cout << " Price: " << std::setw(8) << it->first 
                  << " | Vol: " << std::setw(8) << it->second.volume << "\n";
    }

    std::cout << "----------------------------------------\n";
    std::cout << "[BIDS]\n";
    for (auto it = bids.begin(); it != bids.end(); ++it) {
        std::cout << " Price: " << std::setw(8) << it->first 
                  << " | Vol: " << std::setw(8) << it->second.volume << "\n";
    }
    std::cout << "========================================\n\n";
}