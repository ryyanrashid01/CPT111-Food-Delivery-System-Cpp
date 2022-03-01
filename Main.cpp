#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <time.h>
#include <ctime>
#include <sstream>
#include <cmath>
using namespace std;

// Global variables
#define N 20

// Structures
// Structure for menu items
typedef struct menuItem
{
    int itemID;
    string itemName;
    float itemPrice;
    int itemStock;
    // Functions
    int buy(int quantity);
    int price(float newPrice);
    int addStock(int quantity);
} menuItem;
// Structure for customer users
typedef struct customer
{
    int customerID;
    float accountBalance;
    string username;
    string passcode;
    // Functions
    void password(string newPass);
} customer;

// Function Prototypes
int displayUserMenu();
int displayCustomerMenu();
char displayManagerMenu();
void printLines();
void orderOnline(menuItem *P, int size, customer *C, int size_c);
void create_update_Menu(menuItem *P, int size);
void makePayment();
void update_Prices(menuItem *P, int size);
bool is_accepting_orders();
void accept_orders();
void tally(menuItem *P, int index, int quantitySelect);
void calculateSales();
void update_stock(menuItem *P, int size);
void displayMenu(menuItem *P, int size);
void displayMenuPrice(menuItem *P, int size);
int login(customer *C, int size_c);
void changePassword(customer *C, int size_c);
void registerUser(customer *C, int size_c);
void check_balance(customer *C, int size_c);
void add_money_to_wallet(customer *C, int size_c);
int checksum(long long int num);
int digits(long long int num);
int first_dig(long long int num);

int main()
{   
    int userSelect, optionSelect;

    // Create an array to store the menu items.
    menuItem menuList[N] = {0};
    // Create an array to store the customer list.
    customer customerList[N] = {0};
    // Open files to read and write.
    fstream menuFile, customerFile;
    menuFile.open("MenuFile.txt", ios::in | ios::out);
    customerFile.open("CustomerList.txt", ios::in | ios::out);
    // Error check. If the file doesn't open.
    if (!menuFile)
    {
        cout << "\n Couldn't find the files. Please check the file names and the location." << endl;
        return -1;
    }
    if (!customerFile)
    {
        cout << "\n Couldn't find the files. Please check the file names and the location." << endl;
        return -1;
    }
    // Read till the end of file and store the data in the menuItem array.
    int i = 0;
    while (!menuFile.eof())
    {
        menuFile >> menuList[i].itemID >> menuList[i].itemName >> menuList[i].itemPrice >> menuList[i].itemStock;
        i++;
    }
    // Read till the end of file and store the data in the customerList array.
    int j = 0;
    while (!customerFile.eof())
    {
        customerFile >> customerList[j].customerID >> customerList[j].username >> customerList[j].passcode >>customerList[j].accountBalance;
        j++;
    }
    // Ask the user to classify himself/herself
    userSelect = displayUserMenu();
    if (userSelect == 1)
    {
        optionSelect = displayCustomerMenu();
        switch (optionSelect)
        {
            case 1:
                orderOnline(menuList, i, customerList, j);
                break;
            case 2: 
                changePassword(customerList, j);
                break;
            case 3:
                check_balance(customerList, j);
                break;
            case 4:
                add_money_to_wallet(customerList, j);
                break;
            case 5:
                registerUser(customerList, j);
                break;
        }
    }
    else if (userSelect == 2)
    {
        optionSelect = (int)displayManagerMenu();
        switch ((char)optionSelect)
        {
        case 'a':
            create_update_Menu(menuList, i);
            i++;
            break;
        case 'b':
            update_Prices(menuList, i);
            break;
        case 'c':
            update_stock(menuList, i);
            break;
        case 'd':
            accept_orders();
            break;
        case 'e':
            calculateSales();
            break;
        }
    }
    else
    {
        cout << "\n Error: INVALID input!" << endl;
        printLines();
    }
    //Update the files before closing
    menuFile.seekg(0, ios::beg); // Move cursor to the beginning of the file
    for (int k = 0; k < N; k++)
    {
        // Since the data of the array elements could have been edited, overwrite sich changes in the original file
        if (menuList[k].itemID == 0)
        {
            continue;
        }
        menuFile << menuList[k].itemID << "\t" 
                 << menuList[k].itemName << "\t" 
                 << menuList[k].itemPrice <<  "\t" 
                 << menuList[k].itemStock;

        if (menuList[k + 1].itemID != 0)
        {
            menuFile << "\n";
        } 
    }
    customerFile.seekg(0); // Move cursor to the beginning of the file
    for (int k = 0; k < N; k++)
    {
        // Since the data of the array elements could have been edited, overwrite sich changes in the original file
        if (customerList[k].customerID == 0)
        {
            continue;
        }
        customerFile   << customerList[k].customerID << "\t" 
                        << customerList[k].username << "\t" 
                        << customerList[k].passcode <<  "\t" 
                        << customerList[k].accountBalance;
        if (customerList[k + 1].customerID != 0)
        {
            customerFile << "\n";
        } 
    }
    // Close the files.
    menuFile.close();
    customerFile.close();
    return 0;
}

// Structure Menu Item Functions
int menuItem::buy(int quantity)
{
    if (quantity > itemStock)
    {
        return -1;
    }
    itemStock -= quantity;
    return 0;
}
int menuItem::price(float newPrice)
{
    if (newPrice <= 0)
    {
        return -1;
    }
    itemPrice = newPrice;
    return 0;
}
int menuItem::addStock(int quantity)
{
    if (quantity < 0)
    {
        return -1;
    }
    itemStock += quantity;
    return 0;
}
// Structure Customer Functions
void customer::password(string newPass)
{
    if (newPass.length() > 5)
    {
        passcode = newPass;
    }
    else
    {
        do
        {
            cout << "\n Error: Password too short!" << endl;
            cin >> newPass;
        } while (newPass.length() < 5);
    }
}
// Function to print options for the user and get the input
int displayUserMenu()
{
    int choice;
    cout << "\n I am a: \n\n 1. Customer \n\n 2. Manager  \n\n Enter '1' for Customer and '2' for Manager: ";
    cin >> choice;
    printLines();
    return choice;
}
// Function to print options for the customer and get the input
int displayCustomerMenu()
{
    int choice = 0;
    cout << "\n Customer Menu: " << endl;
    cout << "\n 1. Order Online " << endl;
    cout << "\n 2. Change Password " << endl;
    cout << "\n 3. Check Account Balance" << endl;
    cout << "\n 4. Add Money to Wallet " << endl;
    cout << "\n 5. Register" << endl;
    do
    {
        cout << "\n Enter option: ";
        cin >> choice;
        if (choice < 1 || choice > 5)
        {
            cout << "\n Invalid Input! Try Again!" << endl;
        }
    } while (!(choice >= 1 && choice <= 5));
    printLines();
    return choice;
}
// Function to print options for the manager and get the input
char displayManagerMenu()
{
    char choice = '0';
    cout << "\n Manager Menu: " << endl;
    cout << "\n a. Create/Update Menu" << endl;
    cout << "\n b. Update Prices" << endl;
    cout << "\n c. Update Stock" << endl;
    cout << "\n d. Accept Orders" << endl;
    cout << "\n e. Calculate total sales for a day" << endl;
    do
    {
        cout << "\n Enter option: ";
        cin >> choice;
        if ((int)choice < 97 || (int)choice > 102)
        {
            cout << "\n Invalid Input! Try Again!" << endl;
        }
    } while (!((int)choice >= 97 && (int)choice <= 102));
    printLines();

    // return the option selected to the main funciton
    return choice;
}
// Function to print lines for beautification
void printLines()
{
    cout << "\n\n =============================================== \n\n";
}
// Funcition to present the user with the buy menu once the option is selected
void orderOnline(menuItem *P, int size, customer *C, int size_c)
{
    // Check if the restaurant is open or not 
    if (!is_accepting_orders())
    {
        cout << "\n The restaurant is not accepting orders right now! Sorry for the inconvenience." << endl;
        return;
    }

    // Let the user login
    int userID = login(C, size_c);
    if (userID == -1)
    {
        return;
    }
    
    // display the menu
    int menuSelect, quantitySelect;
    cout << "\n " << setw(12) << left << "Item ID"
         << setw(20) << left << "Item Name"
         << setw(10) << right << "Item Price"
         << endl;
    for (int i = 0; i < size; i++)
    {
        cout << "\n " << setw(12) << left << P[i].itemID
             << setw(20) << left << P[i].itemName
             << "$" << setw(9) << left  << P[i].itemPrice
             << endl;
    }
    do
    {
        cout << "\n Enter the ID of the food item that you want to buy: ";
        cin >> menuSelect;
        if (menuSelect < 1 || menuSelect > size)
        {
            cout << "\n Invalid Input!" << endl;
        }
    } while (!(menuSelect >= 1 && menuSelect <= size));
    do
    {
        cout << "\n Enter Quantity: ";
        cin >> quantitySelect;
        if (quantitySelect < 1)
        {
            cout << "\n Error: Quantity must be 1 or more." << endl;
        }
    } while (quantitySelect < 1);
    // Check stock
    if (P[menuSelect - 1].itemStock < quantitySelect)
    {
        cout << "\n We are sorry. We do not have enough " << P[menuSelect - 1].itemName << " in the stock. Stock: " << P[menuSelect - 1].itemStock << endl;
    }
    else
    {
        float total = P[menuSelect - 1].itemPrice * quantitySelect * 1.0;
        char payMoney;
        P[menuSelect - 1].buy(quantitySelect);
        cout << "\n Order Placed!" << endl;
        cout << "\n Proceeding to checkout..." << endl;
        cout << "\n Make Payment: " << endl;
        cout << "\n Order Total: $" << total << endl;
        cout << "\n Do you want to proceed with the payment? Account Balance: $" << C[userID].accountBalance << " (Y/N): ";
        cin >> payMoney;
        if (payMoney == 'Y' || payMoney == 'y')
        {
            if (total > C[userID].accountBalance)
            {
                cout << "\n Not enough money. Add money to wallet and try again!" << endl;
                return;
            }
            C[userID].accountBalance -= total;
            cout << "\n Purchase Succesful!" << endl;
            srand(time(NULL));
            cout << "\n Your estimated delivery time is: " << rand() % 30 << " minutes. Hang tight!" << endl;
            tally(P, menuSelect - 1, quantitySelect);
        }
        else
        {
            cout << "\n Payment Declined! Logging out ... " << endl;
            cout << "\n Session Ended!" << endl;
        }
    }
}
// Add items to menu or update existing items
void create_update_Menu(menuItem *P, int size)
{
    string name;
    int index = size;
    if (index < N)
    {
        cout << "\n Enter the name of the item: ";
        cin >> name;
        
        for (int i = 0; i < size; i++)
        {
            // check if item already in the menu list
            if (P[i].itemName == name)
            {
                index = i;
            }
        }
        P[index].itemID = index + 1;
        P[index].itemName = name;
        cout << "\n Enter the price of the item: ";
        cin >> P[index].itemPrice;
        cout << "\n Enter the stock of the item: ";
        cin >> P[index].itemStock;
        cout << "\n Item Added successfully!" << endl;
    }
}
// Update prices of the items
void update_Prices(menuItem *P, int size)
{
    int id, i;
    float updatePrice;
    displayMenuPrice(P, size);
    cout << "\n What is the ID of the item whose price you want to update? : ";
    cin >> id;

    for (i = 0; i < size; i++)
    {
        if (P[i].itemID == id)
        {
            do
            {
                cout << "\n What do you want to update it to? : ";
                cin >> updatePrice;
                if (updatePrice <= 0)
                {
                    cout << "\n INVALID Price!" << endl;
                }
                
            } while (updatePrice <= 0);
            cout << "\n Price of " << P[i].itemName << " changed from $" << P[i].itemPrice << " to $" << updatePrice << "." << endl;
            P[i].price(updatePrice);
            return;
        }
    }
    if (i == size)
    {
        cout << "\n INVALID ID. Please try again!" << endl;
        return;
    }
}
// Check if the restaurant is accepting orders or not
bool is_accepting_orders()
{
    string read;
    fstream orderStatus;
    orderStatus.open("OrderStatus.txt", ios::in | ios::out);
    if (!orderStatus)
    {
        cout << "\n Couldn't access OrderStatus file. Please check the file location and try again." << endl;
        return false;
    }
    orderStatus >> read;
    if (read == "truee")
    {
        return true;
    }
    else
    {
        return false;
    }
    orderStatus.close();
}
// Make the restaurant open or closed for orders
void accept_orders()
{
    char input;
    fstream orderStatus;
    orderStatus.open("OrderStatus.txt", ios::in | ios::out);
    if (!orderStatus)
    {
        cout << "\n Couldn't access OrderStatus file. Please check the file location and try again." << endl;
        return;
    } 
    if (is_accepting_orders())
    {
        do
        {
            cout << "\n You are currently accepting orders. Do you want to change that? (Y/N): ";
            cin >> input;
        } while (!(input == 'Y' || input == 'N'));  
        if (input == 'Y')
        {
            orderStatus.seekg(0, ios::beg);
            orderStatus << "false";
            cout << "\n Status Changed!" << endl;
            return;
            }
        else
        {
            cout << "\n Okay!" << endl;
            return;
        }
    } 
    else
    {
        do {
            cout << "\n You are currently NOT accepting orders. Do you want to change that? (Y/N): ";
            cin >> input;
        } while (!(input == 'Y' || input == 'N'));
        if (input == 'Y')
        {
            orderStatus.seekg(0, ios::beg);
            orderStatus << "true";
            cout << "\n Status Changed!" << endl;
            return;
        }
        else
        {
            cout << "\n Okay!" << endl;
            return;
        }
    }
    orderStatus.close();
}
// Tally the order details into the ledger
void tally(menuItem *P, int index, int quantitySelect)
{
    time_t now;
    struct tm nowLocal;
    now = time(NULL);
    nowLocal = *localtime(&now);
    ostringstream day, month, year;
    day << nowLocal.tm_mday;
    month << nowLocal.tm_mon;
    year << nowLocal.tm_year;
    string fileName = day.str() + month.str() + year.str();
    fstream ledger;
    ledger.open(("Ledger\\" + fileName + ".txt").c_str(), ios::in | ios::out | ios::app);
    ledger << P[index].itemName << "\t" << quantitySelect << "\t" << P[index].itemPrice * quantitySelect << endl;
    return;
    ledger.close();
}
// Calculate total sales of the day by accessing the ledger created by the tally() function
void calculateSales()
{
    time_t now;
    struct tm nowLocal;
    now = time(NULL);
    nowLocal = *localtime(&now);
    ostringstream day, month, year;
    day << nowLocal.tm_mday;
    month << nowLocal.tm_mon;
    year << nowLocal.tm_year;
    string fileName = day.str() + month.str() + year.str();
    fstream ledger;
    ledger.open(("Ledger\\" + fileName + ".txt").c_str(), ios::in | ios::out | ios::app);
    if (!ledger)
    {
        cout << "No record found for today. Make some sales and try again!" << endl;
        return;
    }
    string name[N];
    int quantity[N], i = 0;
    float price[N], total = 0;
    while (!ledger.eof())
    {
        ledger >> name[i] >> quantity[i] >> price[i];
        i++;
    }
    for (int j = 0; j < i - 1; j++)
    {
        total += price[j];
    }
    for (int j = 0; j < i - 1; j++)
    {
        cout << "\n " << setw(20) << left << name[j] << setw(10) << left << quantity[j] << "$" << setw(6) << left << price[j] << endl; 
    }
    cout << "\n ======================================== \n";
    cout << "\n\n " << setw(30) <<left << "Total:" << "$" << setw(6) << left << total << endl;
    return; 
}
// Display menu for updateStock
void displayMenu(menuItem *P, int size)
{
    cout << "\n " << setw(12) << left << "Item ID"
         << setw(20) << left << "Item Name"
         << setw(10) << right << "Item Stock"
         << endl;
    for (int i = 0; i < size; i++)
    {
        cout << "\n " << setw(12) << left << P[i].itemID
             << setw(20) << left << P[i].itemName
             << setw(9) << left  << P[i].itemStock
             << endl;
    }
}
// Display menu for updatePrice
void displayMenuPrice(menuItem *P, int size)
{
    cout << "\n " << setw(12) << left << "Item ID"
         << setw(20) << left << "Item Name"
         << setw(10) << right << "Item Price"
         << endl;
    for (int i = 0; i < size; i++)
    {
        cout << "\n " << setw(12) << left << P[i].itemID
             << setw(20) << left << P[i].itemName
             << "$" << setw(9) << left  << P[i].itemPrice
             << endl;
    }
}
// Update stock
void update_stock(menuItem *P, int size)
{
    int input, updateStock, i;
    displayMenu(P, size);
    cout << "\n Select the ID of the item whose stock you want to update: ";
    cin >> input;
    for (i = 0; i < size; i++)
    {
        if (P[i].itemID == input)
        {
            cout << "\n How much new stock to be added: ";
            cin >> updateStock;
            cout << "\n Stock of " << P[i].itemName << " changed from " << P[i].itemStock << " to " << P[i].itemStock + updateStock << "." << endl;
            P[i].addStock(updateStock);
            return;
        }
    }
    if (i == size)
    {
        cout << "\n INVALID ID. Please try again!" << endl;
        return;
    }
}
// Login user
int login(customer *C, int size_c)
{
    int i;
    string username, password;
    cout << "\n ===== LOG IN =====\n";
    cout << "\n Enter username: ";
    cin >> username;
    cout << "\n Enter password: ";
    cin >> password;

    for (i = 0; i < size_c; i++)
    {
        if (C[i].username == username)
        {
            if (C[i].passcode == password)
            {
                cout << "\n Log In Successful!" << endl;
                return i;
            }
            else
            {
                cout << "\n Incorrect password" << endl;
            }
        }
    }
    if (i == size_c)
    {
        cout << "\n No such user found. Please check your username or REGISTER!" << endl;
    }
    return -1;
}
// Password changer
void changePassword(customer *C, int size_c)
{
    int i;
    string username, password, newPass;
    cout << "\n ===== Change Password ===== \n";
    cout << "\n Username: ";
    cin >> username;
    cout << "\n Old Password: ";
    cin >> password;
    for (i = 0; i < size_c; i++)
    {
        if (C[i].username == username)
        {
            if (C[i].passcode == password)
            {
                cout << "\n New Password: ";
                cin >> newPass;
                C[i].password(newPass);
                cout << "\n Password changed successfully!" << endl;
                return;
            }
            else
            {
                cout << "\n Incorrect password" << endl;
                return;
            }
        }
    }
    if (i == size_c)
    {
        cout << "\n No such user found. Please check your username or REGISTER!" << endl;
        return;
    }
}
// Register new user
void registerUser(customer *C, int size_c)
{
    string username, password;

    cout << "\n ===== Register ===== \n";
    cout << "\n Username: ";
    cin >> username;
    for (int i = 0; i < size_c; i++)
    {
        if (C[i].username == username)
        {
            cout << "\n Username taken! Try another!" << endl;
            return;
        }
    } 

    int length;
    do
    {
        cout << "\n Password: ";
        cin >> password;
        length = password.length();
        if (length < 6)
        {
            cout << "\n Password too short!" << endl;
        }
        
    } while (length < 6);

    C[size_c].customerID = size_c + 1;
    C[size_c].username = username;
    C[size_c].passcode = password;
    C[size_c].accountBalance = 0.0;

    cout << "\n Registration successful!" << endl;
}
// Balance check
void check_balance(customer *C, int size_c)
{
    int userIndex = login(C, size_c);

    if (userIndex == -1)
    {
        return;
    }
    cout << "\n Account balance = $" << C[userIndex].accountBalance << endl;
    return;
}   
// Add money to wallet
void add_money_to_wallet(customer *C, int size_c)
{
    int userIndex = login(C, size_c), cvv;
    long long int creditnumber;
    float amount;
    string type;

    if (userIndex == -1)
    {
        return;
    }
    cout << "\n ===== Add Money ===== \n";
    cout << "\n Enter your credit card number (MASTERCARD/VISA/AMEX only): ";
    cin >> creditnumber;

    int x = digits(creditnumber), y = first_dig(creditnumber), z = checksum(creditnumber);

    if ((x == 15 || x == 16 || x == 13) && z == 0)
    {
        if(x == 15 && (y == 34 || y == 37))
                type = "AMERICAN EXPRESS";
        else if(x == 16 && y >= 51 && y <= 55)
                type = "MASTER";
        else if((x == 16 || x == 13) && y >= 40 && y <= 49)
                type = "VISA";
    }
    else 
    {
        cout << "\n INVALID CARD NUMBER!" << endl;
        return; 
    }
    cout << "\n Enter your " << type << " CARD CVV: ";
    cin >> cvv;

    cout << "\n Enter amount to be added: ";
    cin >> amount;

    C[userIndex].accountBalance += amount;
    cout << "\n Amount added to your wallet. Thank you!";
}
// Helper funcitons for add_money_to_wallet() to check if the credit card number is valid or not
int checksum(long long int num)
{
    // Luhn's algorithm to check the credit card number
    int sum_mult = 0, sum = 0, a, b, dig_1, dig_2, c; 
    for (; num > 0; num /= 100)
    {
        a = num % 10;
        b = ((num / 10) % 10) * 2;
        dig_1 = b % 10;
        dig_2 = (b / 10) % 10;
        c = dig_1 + dig_2;
        sum_mult += c;
        sum += a;      
    }
    int checksum = (sum + sum_mult) % 10;
    return (checksum);
}
int digits(long long int num)
{
    int digit = 0;
    do
    {
        digit++;
        num /= 10;
    } while (num > 0);
    return (digit);
}
int first_dig(long long int num)
{
    int ini_dig = num / pow(10,(digits(num)-2));
    return (ini_dig);
}