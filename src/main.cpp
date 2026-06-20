#include "OrderBook.h"
#include <iostream>

int main() {
    OrderBook book;
    
    book.addOrder(1, Side::BUY, 15023, 400);
    book.addOrder(2, Side::BUY, 15024, 500);
    book.addOrder(3, Side::BUY, 15024, 300);
    
    book.addOrder(4, Side::SELL, 15026, 500);
    book.addOrder(5, Side::SELL, 15025, 1200);

    std::cout << "Initial Book State:\n";
    book.printBook();

    std::cout << "Canceling Order #2 (Buy 500 @ 15024)...\n";
    book.cancelOrder(2);
    
    book.printBook();

    return 0;
}