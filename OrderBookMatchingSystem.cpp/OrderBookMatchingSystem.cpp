#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

// Represents an order in the order book
struct Order {
    int id;        // Unique identifier for the order
    int price;     // Price of the order
    int quantity;  // Quantity of the order
    bool isBuy;    // True if the order is a buy order, false for sell orders

    // Constructor to initialize an order
    Order(int id, int price, int quantity, bool isBuy)
        : id(id), price(price), quantity(quantity), isBuy(isBuy) {}
};

// Comparator for sorting orders by price
bool compareByPrice(const Order& a, const Order& b) {
    return a.price < b.price; // Sort in ascending order for sell orders
}

// Function to display the main menu
void displayMenu() {
    cout << "\nOrder Book Matching System Menu:\n";
    cout << "1. Add Buy Order\n";
    cout << "2. Add Sell Order\n";
    cout << "3. Execute Trades\n";
    cout << "4. View Orders\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

// Function to add an order to the appropriate list
void addOrder(vector<Order>& buyOrders, vector<Order>& sellOrders, bool isBuy) {
    int id, price, quantity;
    cout << "Enter Order ID: ";
    cin >> id;
    cout << "Enter Price: ";
    cin >> price;
    cout << "Enter Quantity: ";
    cin >> quantity;

    Order order(id, price, quantity, isBuy);
    if (isBuy) {
        buyOrders.push_back(order);
    } else {
        sellOrders.push_back(order);
    }
}

// Function to execute trades between buy and sell orders
void executeTrades(vector<Order>& buyOrders, vector<Order>& sellOrders) {
    // Sort orders: Buy orders in descending order, Sell orders in ascending order
    sort(buyOrders.begin(), buyOrders.end(), [](const Order& a, const Order& b) {
        return a.price > b.price; // Higher price first for buy orders
    });
    sort(sellOrders.begin(), sellOrders.end(), compareByPrice);

    // Match orders
    while (!buyOrders.empty() && !sellOrders.empty()) {
        Order& buy = buyOrders.front();
        Order& sell = sellOrders.front();

        // Determine the quantity to trade
        int tradeQuantity = min(buy.quantity, sell.quantity);

        // Print the trade details
        cout << "Trade executed: Buy Order ID " << buy.id
             << " with Sell Order ID " << sell.id
             << " for " << tradeQuantity << " units at price " << sell.price << endl;

        // Update quantities
        buy.quantity -= tradeQuantity;
        sell.quantity -= tradeQuantity;

        // Remove orders if their quantity reaches zero
        if (buy.quantity == 0) {
            buyOrders.erase(buyOrders.begin());
        }
        if (sell.quantity == 0) {
            sellOrders.erase(sellOrders.begin());
        }
    }
}

// Function to view remaining orders
void viewOrders(const vector<Order>& buyOrders, const vector<Order>& sellOrders) {
    cout << "\nRemaining Buy Orders:\n";
    for (const auto& order : buyOrders) {
        cout << "Order ID: " << order.id << ", Price: " << order.price
             << ", Quantity: " << order.quantity << endl;
    }

    cout << "\nRemaining Sell Orders:\n";
    for (const auto& order : sellOrders) {
        cout << "Order ID: " << order.id << ", Price: " << order.price
             << ", Quantity: " << order.quantity << endl;
    }
}

int main() {
    vector<Order> buyOrders;
    vector<Order> sellOrders;
    int choice;

    while (true) {
        displayMenu();
        cin >> choice;

        // Clear input buffer in case of invalid input
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                addOrder(buyOrders, sellOrders, true);
                break;
            case 2:
                addOrder(buyOrders, sellOrders, false);
                break;
            case 3:
                executeTrades(buyOrders, sellOrders);
                break;
            case 4:
                viewOrders(buyOrders, sellOrders);
                break;
            case 5:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}

