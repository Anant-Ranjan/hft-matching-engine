#include "OrderBook.h"
#include <iostream>

int main() {
    OrderBook book;
    
    book.addOrder(1, Side::SELL, 15025, 500);
    book.addOrder(2, Side::SELL, 15026, 1000);
    book.addOrder(3, Side::SELL, 15027, 800);
    
    book.addOrder(4, Side::BUY, 15020, 400);
    book.addOrder(5, Side::BUY, 15019, 300);

    std::cout << "Initial Book State:\n";
    book.printBook();

    std::cout << "Incoming aggressively priced BUY order (ID: 6, Buy 1200 @ 15026)...\n\n";
    book.addOrder(6, Side::BUY, 15026, 1200);
    
    std::cout << "\nPost-Trade Book State:\n";
    book.printBook();

    return 0;
}