#include "OrderBook.h"
#include <iostream>
#include <iomanip>
#include <algorithm>



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
    uint32_t remainingQuantity = quantity;

    if (side == Side::BUY) {
        auto it = asks.begin();
        while (it != asks.end() && remainingQuantity > 0 && it->first <= price) {
            PriceLevel& level = it->second;
            Order* currentAsk = level.head;

            while (currentAsk != nullptr && remainingQuantity > 0) {
                uint32_t fillQty = std::min(remainingQuantity, currentAsk->quantity);
                
                std::cout << "[TRADE] ID: " << id << " bought " << fillQty 
                          << " from ID: " << currentAsk->id << " @ " << it->first << "\n";

                remainingQuantity -= fillQty;
                currentAsk->quantity -= fillQty;
                level.volume -= fillQty;

                if (currentAsk->quantity == 0) {
                    Order* toDelete = currentAsk;
                    currentAsk = currentAsk->next;
                    
                    orderMap.erase(toDelete->id);
                    removeOrderFromList(toDelete, level);
                    delete toDelete;
                } else {
                    break; 
                }
            }

            if (level.head == nullptr) {
                it = asks.erase(it);
            } else {
                ++it;
            }
        }
    } else {
        auto it = bids.begin();
        while (it != bids.end() && remainingQuantity > 0 && it->first >= price) {
            PriceLevel& level = it->second;
            Order* currentBid = level.head;

            while (currentBid != nullptr && remainingQuantity > 0) {
                uint32_t fillQty = std::min(remainingQuantity, currentBid->quantity);
                
                std::cout << "[TRADE] ID: " << id << " sold " << fillQty 
                          << " to ID: " << currentBid->id << " @ " << it->first << "\n";

                remainingQuantity -= fillQty;
                currentBid->quantity -= fillQty;
                level.volume -= fillQty;

                if (currentBid->quantity == 0) {
                    Order* toDelete = currentBid;
                    currentBid = currentBid->next;
                    
                    orderMap.erase(toDelete->id);
                    removeOrderFromList(toDelete, level);
                    delete toDelete;
                } else {
                    break;
                }
            }

            if (level.head == nullptr) {
                it = bids.erase(it);
            } else {
                ++it;
            }
        }
    }

    if (remainingQuantity > 0) {
        Order* newOrder = new Order{id, side, price, remainingQuantity, nullptr, nullptr};
        orderMap[id] = newOrder;

        if (side == Side::BUY) {
            appendOrderToList(newOrder, bids[price]);
        } else {
            appendOrderToList(newOrder, asks[price]);
        }
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