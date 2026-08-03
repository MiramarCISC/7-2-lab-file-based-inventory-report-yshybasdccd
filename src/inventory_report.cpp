#include "inventory_report.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

bool isValidQuantity(int quantity) {
    if (quantity >= 0) {
        return true;
    }
    return false;
}

bool isValidPrice(double price) {
    if (price >= 0.0) {
        return true;
    }
    return false;
}

double calculateItemValue(const InventoryItem& item) {
    if (!isValidQuantity(item.quantity)) {
        return 0.0;
    }
    if (!isValidPrice(item.price)) {
        return 0.0;
    }
    return item.quantity * item.price;
}

int readInventoryFile(string filename, InventoryItem items[], int maxItems) {
    if (items == nullptr || maxItems <= 0) {
        return 0;
    }
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        return 0;
    }

    int count = 0;
    InventoryItem item;
    while (
        count < maxItems &&
        inputFile >> item.sku
                  >> item.name
                  >> item.quantity
                  >> item.price
    ) {
        if (
            isValidQuantity(item.quantity) &&
            isValidPrice(item.price)
        ) {
            items[count] = item;
            count = count + 1;
        }
    }
    inputFile.close();
    return count;
}

bool writeInventoryReport(string filename, const InventoryItem items[], int count) {
    if (items == nullptr || count < 0) {
        return false;
    }
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        return false;
    }

    outputFile << fixed << setprecision(2);
    outputFile << "Inventory Report";
    for (int i = 0; i < count; i++) {
        outputFile << endl << items[i].sku << " "
                    << items[i].name << " "
                    << items[i].quantity << " "
                    << items[i].price << " "
                    << calculateItemValue(items[i]);
                   
    }
    outputFile << endl
                << "Total inventory value: "
                << calculateTotalInventoryValue(items, count);
    outputFile.close();

    return true;
}

double calculateTotalInventoryValue(const InventoryItem items[], int count) {
    if (items == nullptr || count <= 0) {
        return 0.0;
    }

    double total = 0.0;
    for (int i = 0; i < count; i++) {
        total = total + calculateItemValue(items[i]);
    }
    return total;
}

int findItemBySku(const InventoryItem items[],int count,string sku) {
    if (items == nullptr || count <= 0) {
        return -1;
    }
    for (int i = 0; i < count; i++) {
        if (items[i].sku == sku) {
            return i;
        }
    }
    return -1;
}

int findHighestValueItemIndex(const InventoryItem items[],int count) {
    if (items == nullptr || count <= 0) {
        return -1;
    }
    int highestIndex = 0;
    for (int i = 1; i < count; i++) {
        double currentValue = calculateItemValue(items[i]);
        double highestValue =
            calculateItemValue(items[highestIndex]);

        if (currentValue > highestValue) {
            highestIndex = i;
        }
    }
    return highestIndex;
}
