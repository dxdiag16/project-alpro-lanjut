#include <iostream>
using namespace std;

struct items {
    string name;
    string category;
    string type;
    int price;
    int quantity;
};

items shopItems[] = {
    {"Black Sword", "Weapon", "Long Sword", 500, 7},
    {"Blood Reaper", "Weapon", "Scythe", 1000, 5},
    {"Green Herb", "Consumable", "Heal", 100, 10},
};

int num_shopItems = sizeof(shopItems)/sizeof(shopItems[0]);
items inventoryItems[10];
int inventoryCount = 0;

void viewItems() {
    for(int i = 0; i < num_shopItems ; i++) {
        cout << i + 1 << ". "
        << shopItems[i].name << " - "
        << shopItems[i].category << " - "
        << shopItems[i].type << " - "
        << shopItems[i].price << " Gold - Stock: "
        << shopItems[i].quantity << endl;
    }
}

void searchItems(){

}

void buyItems(int *inventoryCount) {
    int choice, buyQuantity;

    viewItems();
    cout << "Choose item number: "; cin >> choice;

    if(choice < 1 || choice > num_shopItems) {
        cout << "Invalid item number!" << endl;
        return;
    }

    int index = choice - 1;

    cout << "Enter quantity: "; cin >> buyQuantity;

    if(shopItems[index].quantity < buyQuantity) {
        cout << "Not enough stock!" << endl;
        return;
    }

    if(*inventoryCount >= 10) {
        cout << "Inventory full!" << endl;
        return;
    }

    inventoryItems[*inventoryCount] = shopItems[index];
    inventoryItems[*inventoryCount].quantity = buyQuantity;

    shopItems[index].quantity -= buyQuantity;
    (*inventoryCount)++;

    cout << "Item purchased!" << endl;
}

void viewInventory() {
    for(int i = 0; i < inventoryCount; i++) {
        cout << i + 1 << ". "
             << inventoryItems[i].name << " x"
             << inventoryItems[i].quantity << endl;
    }
}

void shop() {
    int option;

    do{
        cout << "=== SHOP ==="    << endl;
        cout << "1. View Items"   << endl;
        cout << "2. Search Items" << endl;
        cout << "3. Buy Items"    << endl;
        cout << "4. Back"         << endl;
        cout << "Enter Options: "; cin >> option;

        switch(option) {
            case 1: 
                viewItems();             
                break;
            case 2:
                searchItems();
                break;
            case 3:
                buyItems(&inventoryCount);
                break;
            default:
                break;      
        }
    }while(option != 4);

}

void inventory() {
    int option;

    do {
        cout << "=== INVENTORY ===" << endl;
        cout << "1. View Inventory" << endl;
        cout << "2. Sort Items"     << endl;
        cout << "3. Back"           << endl;
        cout << "Enter Options: "; cin >> option;

        switch(option) {
            case 1:    
                viewInventory();          
                break;
            case 2:
                break;
            default:
                break;      
        }
    }while(option != 3);
}

int main() {
    int option;

    do {
        cout << "=== MANAGEMENT INVENTORY ERPEGE GAME === "  << endl;
        cout << "1. Shop"      << endl;
        cout << "2. Inventory" << endl;
        cout << "3. Save Game" << endl;
        cout << "4. Load Game" << endl;
        cout << "5. Exit"      << endl;
        cout << "Enter Options: "; cin >> option;

        switch(option) {
            case 1:
                shop();
                break;
            case 2:
                inventory();
                break;
            case 3:
                break;
            case 4:
                break;
            default:
                break;      
        }
    }while(option != 5);
}
