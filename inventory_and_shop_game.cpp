#include <iostream>
#include <termios.h>
#include <unistd.h>
using namespace std;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #elif defined(__linux__) || defined(__APPLE__)
        system("clear");
    #else
        std::cout << "\n" << std::endl; // Fallback for unknown systems
    #endif
}

void waitAnyKey() {
    struct termios oldt, newt;
    std::cout << "\n[ Tekan tombol apa saja untuk lanjut ]" << std::flush;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    getchar(); // Menunggu satu input karakter
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << std::endl;
    clearScreen();
}

struct items {
    string name;
    string category;
    string type;
    int price;
    int quantity;
    int maxStack;
};
        
items shopItems[] = {
    {"Black Sword", "Weapon", "Long Sword", 500, 7, 1},
    {"Blood Reaper", "Weapon", "Scythe", 1000, 5, 1},
    {"Green Herb", "Consumable", "Heal", 100, 10, 5},
};

int num_shopItems = sizeof(shopItems)/sizeof(shopItems[0]);
items inventoryItems[10];
int inventoryCount = 0;
int playerGold = 2000;

int inputInt(string prompt) {
    string input;
    int value;

    while(true) {
        cout << prompt;
        getline(cin, input);

        bool valid = true;

        for(char c : input) {
            if(!isdigit(c)) {
                valid = false;
                break;
            }
        }

        if(valid && !input.empty()) {
            value = stoi(input);
            return value;
        } else {
            cout << "Input must be a number only!" << endl;
        }
    }
}

void viewItems(bool isBuy = false) {
    clearScreen();

    for(int i = 0; i < num_shopItems ; i++) {
        cout << i + 1 << ". "
        << shopItems[i].name << " - "
        << shopItems[i].category << " - "
        << shopItems[i].type << " - "
        << shopItems[i].price << " Gold - Stock: "
        << shopItems[i].quantity << endl;
    }

    if (!isBuy){
        waitAnyKey();
    }
}

void searchItems(){

}

bool addToInventory(items inventoryItems[], int &inventoryCount, int maxSize, items item, int amount) {
   
    int remaining = amount;
    
    for(int i = 0; i < inventoryCount; i++) {
        if(inventoryItems[i].name == item.name && inventoryItems[i].quantity < item.maxStack) {
            int space = item.maxStack - inventoryItems[i].quantity;
            int add = min(space, remaining);

            inventoryItems[i].quantity += add;
            remaining -= add;

            if(remaining == 0) return true;
        }
    }

    while(remaining > 0) {
        if(inventoryCount >= maxSize) {
            return false;  
        }

        int add = min(item.maxStack, remaining);

        inventoryItems[inventoryCount] = item;
        inventoryItems[inventoryCount].quantity = add;

        remaining -= add;
        inventoryCount++;
    }

    return true;
}   

void buyItems(int *inventoryCount) {
    clearScreen();
    int choice, buyQuantity;
    bool isBuy = true;

    cout << "Gold: " << playerGold << "G" << endl;

    viewItems(isBuy);
    choice = inputInt("Choose item number (0 to cancel): ");

    if(choice == 0) {
        return;
    }

    if(choice < 1 || choice > num_shopItems) {
        cout << "Invalid item number!" << endl;
        return;
    }

    int index = choice - 1;

    buyQuantity = inputInt("Enter quantity (0 to cancel): "); 

    if( buyQuantity == 0) {
        return;
    }

    if(shopItems[index].quantity < buyQuantity) {
        cout << "Not enough stock!" << endl;
        return;
    }

    int totalPrice = shopItems[index].price * buyQuantity;

    if(playerGold < totalPrice) {
        cout << "Not enough gold!" << endl;
        return;
    }
    
    bool success =  addToInventory(inventoryItems, *inventoryCount, 10, shopItems[index], buyQuantity);

    if(!success) {
        cout << "Inventory full!" << endl;
        return;
    }

    playerGold -= totalPrice;
    shopItems[index].quantity -= buyQuantity;
    cout << "Item purchased!" << endl;
}

void viewInventory(int *inventoryCount) {
    clearScreen();
    if(*inventoryCount == 0) {
        cout << "Inventory empty" << endl;
        waitAnyKey();
    } else {
        for(int i = 0; i < *inventoryCount; i++) {
            cout << i + 1 << ". "
                << inventoryItems[i].name << " x"
                << inventoryItems[i].quantity << endl;
        }
        waitAnyKey();
    }
}

void removeEmptySlot(int &inventoryCount) {
    for(int i = 0; i < inventoryCount; i++) {
        if(inventoryItems[i].quantity == 0) {
            for(int j = i; j < inventoryCount - 1; j++) {
                inventoryItems[j] = inventoryItems[j + 1];
            }
            inventoryCount--;
            i--;
        }
    }
}

void sellItems(int *inventoryCount) {
    if(*inventoryCount == 0) {
        cout << "Inventory empty" << endl;
        return;
    }

    viewInventory(inventoryCount);

    int choice = inputInt("Choose item number to sell (0 to cancel): ");

    if(choice == 0) {
        return;
    }

    if(choice < 1 || choice > *inventoryCount) {
        cout << "Invalid item number!" << endl;
        return;
    }

    int index = choice - 1;

    int sellQuantity = inputInt("Enter quantity to sell (0 to cancel): ");

    if(sellQuantity <= 0 || sellQuantity > inventoryItems[index].quantity) {
        cout << "Invalid quantity!" << endl;
        return;
    }

    int sellPrice = inventoryItems[index].price * 0.7;
    int totalSell = sellPrice * sellQuantity;
    playerGold += totalSell;
    inventoryItems[index].quantity -= sellQuantity;

    removeEmptySlot(*inventoryCount);

    cout << "Item sold!" << endl;
    cout << "Gold earned: " << totalSell << "G" << endl;
}

void shop(int *inventoryCount) {
    clearScreen();
    int option;

    do{
        cout << "=== SHOP ==="    << endl;
        cout << "1. View Items"   << endl;
        cout << "2. Search Items" << endl;
        cout << "3. Buy Items"    << endl;
        cout << "4. Back"         << endl;
        option = inputInt("Enter option: ");

        switch(option) {
            case 1: 
                viewItems();             
                break;
            case 2:
                searchItems();
                break;
            case 3:
                buyItems(inventoryCount);
                break;
            default:
                break;      
        }
    }while(option != 4);
    clearScreen();
}

void inventory(int *inventoryCount) {
    clearScreen();
    int option;

    do {
        cout << "=== INVENTORY ===" << endl;
        cout << "1. View Inventory" << endl;
        cout << "2. Sort Items"     << endl;
        cout << "3. Sell Items"     << endl;
        cout << "4. Back"           << endl;
        option = inputInt("Enter option: ");

        switch(option) {
            case 1:    
                viewInventory(inventoryCount);          
                break;
            case 2:
                break;
            case 3:
                sellItems(inventoryCount);
                break;
            default:
                break;      
        }
    }while(option != 4);
    clearScreen();
}

int main() {
    int option;
    int inventoryCount = 0;

    for(int i = 0; i < 10; i++) {
        inventoryItems[i].quantity = 0;
    }

    do {
        cout << "=== MANAGEMENT INVENTORY ERPEGE GAME === "  << endl;
        cout << "1. Shop"      << endl;
        cout << "2. Inventory" << endl;
        cout << "3. Save Game" << endl;
        cout << "4. Load Game" << endl;
        cout << "5. Exit"      << endl;
        option = inputInt("Enter option: ");

        switch(option) {
            case 1:
                shop(&inventoryCount);
                break;
            case 2:
                inventory(&inventoryCount);
                break;
            case 3:
                break;
            case 4:
                break;
            default:
                break;      
        }
    }while(option != 5);
    clearScreen();
}