#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

// Simulated market data structure
struct MarketData {
    std::string symbol;
    double price;
    int volume;
};

// Simulated order structure
struct Order {
    std::string symbol;
    int quantity;
    double price;
};

class StockExchange {
public:
    void subscribe(const std::string& symbol, std::function<void(const MarketData&)> callback) {
        // Simulated market data feed
        std::thread([this, symbol, callback]() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(1));

                // Generate random market data
                MarketData data;
                data.symbol = symbol;
                data.price = 100.0 + (rand() % 100) / 10.0;
                data.volume = rand() % 1000;

                // Notify subscribers
                callback(data);
            }
        }).detach();
    }

    void placeOrder(const Order& order) {
        // Simulated order execution
        std::lock_guard<std::mutex> lock(orderMutex);
        std::cout << "Executing order - Symbol: " << order.symbol
                  << ", Quantity: " << order.quantity
                  << ", Price: " << order.price << std::endl;
    }

private:
    std::mutex orderMutex;
};

class TradingSystem {
public:
    void handleMarketData(const MarketData& data) {
        // Simulated trading strategy
        if (data.price < 105.0) {
            Order buyOrder{"AAPL", 10, data.price};
            exchange.placeOrder(buyOrder);
        } else if (data.price > 110.0) {
            Order sellOrder{"AAPL", 5, data.price};
            exchange.placeOrder(sellOrder);
        }
    }

private:
    StockExchange exchange;
};

int main() {
    TradingSystem tradingSystem;

    // Subscribe to market data
    tradingSystem.exchange.subscribe("AAPL", [&](const MarketData& data) {
        tradingSystem.handleMarketData(data);
    });

    // Keep the program running
    std::this_thread::sleep_for(std::chrono::minutes(5));

    return 0;
}
