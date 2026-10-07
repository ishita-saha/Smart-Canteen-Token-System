#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include <map>
#include <fstream>

using namespace std;


// ============================================================
// FOOD ITEM CLASS
// ============================================================

class FoodItem
{
private:
    int itemId;
    string itemName;
    double price;

public:

    FoodItem(int id = 0, string name = "Unknown", double p = 0.0)
    {
        itemId = id;
        itemName = name;
        price = p;
    }

    int getId() const
    {
        return itemId;
    }

    string getName() const
    {
        return itemName;
    }

    double getPrice() const
    {
        return price;
    }

    void display() const
    {
        cout << left
             << setw(5) << itemId
             << setw(25) << itemName
             << "Rs. "
             << fixed << setprecision(2)
             << price << endl;
    }
};


// ============================================================
// USER BASE CLASS
// ============================================================

class User
{
protected:
    string name;
    string userId;

public:

    User(string n = "", string id = "")
    {
        name = n;
        userId = id;
    }

    virtual void displayRole() const
    {
        cout << "User";
    }

    string getName() const
    {
        return name;
    }

    string getUserId() const
    {
        return userId;
    }

    virtual ~User()
    {
    }
};


// ============================================================
// STUDENT CLASS
// ============================================================

class Student : public User
{
public:

    Student(string n = "", string id = "")
        : User(n, id)
    {
    }

    void displayRole() const override
    {
        cout << "Student";
    }

    void displayStudent() const
    {
        cout << "\n----------------------------------------\n";
        cout << "          STUDENT INFORMATION\n";
        cout << "----------------------------------------\n";

        cout << "Name       : " << name << endl;
        cout << "Student ID : " << userId << endl;
        cout << "Role       : ";

        displayRole();

        cout << endl;
        cout << "----------------------------------------\n";
    }
};


// ============================================================
// ORDER CLASS
// ============================================================

class Order
{
private:

    int tokenNumber;
    Student student;

    vector<FoodItem> items;
    vector<int> quantities;

public:

    Order(int token, Student s)
        : tokenNumber(token), student(s)
    {
    }

    Order(const Order& other)
    {
        tokenNumber = other.tokenNumber;
        student = other.student;
        items = other.items;
        quantities = other.quantities;
    }


    // --------------------------------------------------------
    // FUNCTION OVERLOADING
    // --------------------------------------------------------

    void addItem(FoodItem item)
    {
        addItem(item, 1);
    }

    void addItem(FoodItem item, int quantity)
    {
        if (quantity <= 0)
        {
            throw invalid_argument(
                "Quantity must be greater than zero."
            );
        }

        items.push_back(item);
        quantities.push_back(quantity);
    }


    // --------------------------------------------------------
    // CALCULATE TOTAL
    // --------------------------------------------------------

    double calculateTotal() const
    {
        if (items.empty())
        {
            throw runtime_error(
                "Order cannot be empty."
            );
        }

        double total = 0;

        for (int i = 0; i < items.size(); i++)
        {
            total += items[i].getPrice()
                   * quantities[i];
        }

        return total;
    }


    // --------------------------------------------------------
    // OPERATOR OVERLOADING <<
    // --------------------------------------------------------

    friend ostream& operator<<(
        ostream& out,
        const Order& order)
    {
        out << "\n========================================\n";
        out << "             ORDER DETAILS\n";
        out << "========================================\n";

        out << "Token Number : "
            << order.tokenNumber << endl;

        out << "Student      : "
            << order.student.getName() << endl;

        out << "Student ID   : "
            << order.student.getUserId() << endl;

        out << "\nItems:\n";

        for (int i = 0;
             i < order.items.size();
             i++)
        {
            out << left
                << setw(22)
                << order.items[i].getName()

                << " x "
                << setw(3)
                << order.quantities[i]

                << " = Rs. "

                << fixed
                << setprecision(2)

                << order.items[i].getPrice()
                   * order.quantities[i]

                << endl;
        }

        out << "\nTotal Amount : Rs. "
            << fixed
            << setprecision(2)
            << order.calculateTotal()
            << endl;

        out << "========================================\n";

        return out;
    }


    // --------------------------------------------------------
    // GETTERS
    // --------------------------------------------------------

    int getToken() const
    {
        return tokenNumber;
    }

    string getStudentName() const
    {
        return student.getName();
    }

    string getStudentId() const
    {
        return student.getUserId();
    }
};


// ============================================================
// TOKEN GENERATOR
// ============================================================

int generateToken()
{
    static int token = 1000;

    return ++token;
}


// ============================================================
// DISPLAY MENU
// ============================================================

void displayMenu(const vector<FoodItem>& menu)
{
    cout << "\n========================================\n";
    cout << "              CANTEEN MENU\n";
    cout << "========================================\n";

    cout << left
         << setw(5) << "ID"
         << setw(25) << "Food Item"
         << "Price\n";

    cout << "----------------------------------------\n";

    for (int i = 0; i < menu.size(); i++)
    {
        menu[i].display();
    }

    cout << "========================================\n";
}


// ============================================================
// PLACE ORDER
// ============================================================

Order placeOrder(
    Student& student,
    const vector<FoodItem>& menu)
{
    int token = generateToken();

    Order order(token, student);

    int choice;
    int quantity;

    while (true)
    {
        displayMenu(menu);

        cout << "\nEnter Food ID (0 to finish): ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');

            cout << "\nInvalid input! "
                 << "Please enter a number.\n";

            continue;
        }

        if (choice == 0)
        {
            break;
        }

        if (choice < 1 ||
            choice > static_cast<int>(menu.size()))
        {
            cout << "\nInvalid food choice!\n";
            continue;
        }

        cout << "Enter quantity: ";

        if (!(cin >> quantity))
        {
            cin.clear();
            cin.ignore(10000, '\n');

            cout << "\nInvalid quantity! "
                 << "Please enter a number.\n";

            continue;
        }

        try
        {
            order.addItem(
                menu[choice - 1],
                quantity
            );

            cout << "Item added successfully!\n";
        }
        catch (const exception& e)
        {
            cout << "Error: "
                 << e.what()
                 << endl;
        }
    }

    order.calculateTotal();

    return order;
}


// ============================================================
// TOKEN CLASS
// ============================================================

class Token
{
private:
    int tokenNumber;
    string status;

public:

    Token(int number)
    {
        tokenNumber = number;
        status = "Pending";
    }

    int getTokenNumber() const
    {
        return tokenNumber;
    }

    string getStatus() const
    {
        return status;
    }

    void setStatus(string newStatus)
    {
        if (status == "Pending" &&
            newStatus == "Preparing")
        {
            status = newStatus;
        }
        else if (status == "Preparing" &&
                 newStatus == "Ready")
        {
            status = newStatus;
        }
        else if (status == "Ready" &&
                 newStatus == "Collected")
        {
            status = newStatus;
        }
        else
        {
            throw invalid_argument(
                "Invalid status transition."
            );
        }
    }
};


// ============================================================
// CANTEEN CLASS
// ============================================================

class Canteen
{
private:

    string canteenName;

    vector<Order> orders;

    map<int, Order> orderMap;

    vector<Token> tokens;

public:

    Canteen(string name)
    {
        canteenName = name;
    }

    string getCanteenName() const
    {
        return canteenName;
    }


    // --------------------------------------------------------
    // ADD ORDER
    // --------------------------------------------------------

    void addOrder(const Order& order)
    {
        orders.push_back(order);

        orderMap.insert(
            {order.getToken(), order}
        );

        Token token(order.getToken());

        tokens.push_back(token);
    }


    // --------------------------------------------------------
    // GET ALL ORDERS
    // --------------------------------------------------------

    vector<Order>& getOrders()
    {
        return orders;
    }


    // --------------------------------------------------------
    // FIND TOKEN
    // --------------------------------------------------------

    Token* findToken(int tokenNumber)
    {
        for (int i = 0;
             i < tokens.size();
             i++)
        {
            if (tokens[i].getTokenNumber()
                == tokenNumber)
            {
                return &tokens[i];
            }
        }

        throw invalid_argument(
            "Invalid token number."
        );
    }


    // --------------------------------------------------------
    // FIND ORDER USING TOKEN
    // --------------------------------------------------------

    Order& findOrder(int tokenNumber)
    {
        auto it = orderMap.find(tokenNumber);

        if (it == orderMap.end())
        {
            throw invalid_argument(
                "Invalid token number."
            );
        }

        return it->second;
    }


    // --------------------------------------------------------
    // SEARCH TOKEN
    // --------------------------------------------------------

    void searchToken(int tokenNumber)
    {
        try
        {
            Token* token =
                findToken(tokenNumber);

            cout << "\n========== TOKEN FOUND ==========\n";

            cout << "Token Number : "
                 << token->getTokenNumber()
                 << endl;

            cout << "Status       : "
                 << token->getStatus()
                 << endl;

            cout << "\nOrder Information:\n";

            Order& order =
                findOrder(tokenNumber);

            cout << order;

            cout << "Status       : "
                 << token->getStatus()
                 << endl;

            cout << "========================================\n";
        }
        catch (const exception& e)
        {
            cout << "Error: "
                 << e.what()
                 << endl;
        }
    }


    // --------------------------------------------------------
    // DISPLAY ALL ORDERS
    // --------------------------------------------------------

    void displayOrders() const
    {
        cout << "\n========== ALL ORDERS ==========\n";

        if (orders.empty())
        {
            cout << "No orders available.\n";
            return;
        }

        for (int i = 0;
             i < orders.size();
             i++)
        {
            cout << orders[i];
        }
    }


    // --------------------------------------------------------
    // DISPLAY ALL TOKENS
    // --------------------------------------------------------

    void displayTokens() const
    {
        cout << "\n========== TOKEN STATUS ==========\n";

        if (tokens.empty())
        {
            cout << "No tokens available.\n";
            return;
        }

        for (int i = 0;
             i < tokens.size();
             i++)
        {
            cout << "Token Number: "
                 << tokens[i].getTokenNumber()
                 << " | Status: "
                 << tokens[i].getStatus()
                 << endl;
        }
    }


    // --------------------------------------------------------
    // SAVE ORDER HISTORY
    // --------------------------------------------------------

    void saveOrderHistory() const
    {
        ofstream file("orders.txt");

        if (!file)
        {
            throw runtime_error(
                "Unable to open order history file."
            );
        }

        for (int i = 0;
             i < orders.size();
             i++)
        {
            file << orders[i];
            file << "\n";
        }

        file.close();

        cout << "Order history saved successfully.\n";
    }


    // --------------------------------------------------------
    // LOAD ORDER HISTORY
    // --------------------------------------------------------

    void loadOrderHistory() const
    {
        ifstream file("orders.txt");

        if (!file)
        {
            cout << "No previous order history found.\n";
            return;
        }

        cout << "\n========================================\n";
        cout << "          ORDER HISTORY\n";
        cout << "========================================\n";

        string line;

        while (getline(file, line))
        {
            cout << line << endl;
        }

        file.close();
    }
};


// ============================================================
// ADMIN CLASS
// ============================================================

class Admin : public User
{
public:

    Admin(
        string n = "Canteen Admin",
        string id = "ADMIN")
        : User(n, id)
    {
    }

    void displayRole() const override
    {
        cout << "Admin";
    }


    // --------------------------------------------------------
    // UPDATE TOKEN STATUS
    // --------------------------------------------------------

    void updateTokenStatus(
        Canteen& canteen,
        int tokenNumber,
        string newStatus)
    {
        try
        {
            Token* token =
                canteen.findToken(tokenNumber);

            token->setStatus(newStatus);

            cout << "Token status updated successfully.\n";
        }
        catch (const exception& e)
        {
            cout << "Error: "
                 << e.what() << endl;
        }
    }


    // --------------------------------------------------------
    // MANAGE ORDERS
    // --------------------------------------------------------

    void manageOrders(Canteen& canteen)
    {
        cout << "\n========================================\n";
        cout << "             MANAGE ORDERS\n";
        cout << "========================================\n";

        canteen.displayTokens();

        int tokenNumber;

        cout << "\nEnter Token Number: ";
        cin >> tokenNumber;

        try
        {
            Token* token =
                canteen.findToken(tokenNumber);

            cout << "\nCurrent Status: "
                 << token->getStatus()
                 << endl;

            string newStatus;

            if (token->getStatus() == "Pending")
            {
                newStatus = "Preparing";
            }
            else if (token->getStatus() == "Preparing")
            {
                newStatus = "Ready";
            }
            else if (token->getStatus() == "Ready")
            {
                newStatus = "Collected";
            }
            else
            {
                throw invalid_argument(
                    "Order has already been collected."
                );
            }

            cout << "Changing status to: "
                 << newStatus
                 << endl;

            token->setStatus(newStatus);

            cout << "Order status updated successfully.\n";
        }
        catch (const exception& e)
        {
            cout << "Error: "
                 << e.what()
                 << endl;
        }
    }
};


// ============================================================
// MAIN FUNCTION
// ============================================================

int main()
{
    vector<FoodItem> menu =
    {
        FoodItem(1, "Veg Sandwich", 50),
        FoodItem(2, "Masala Dosa", 70),
        FoodItem(3, "Veg Burger", 80),
        FoodItem(4, "Cold Coffee", 60),
        FoodItem(5, "French Fries", 90)
    };

    Canteen canteen("Smart Canteen");

    Student currentStudent("", "");

    bool studentLoggedIn = false;
    bool adminLoggedIn = false;

    int choice;

    cout << "\n========================================\n";
    cout << "       SMART CANTEEN TOKEN SYSTEM\n";
    cout << "========================================\n";

    do
    {
        cout << "\n--------------- MAIN MENU ---------------\n";
        cout << "1. Student Login\n";
        cout << "2. Place Order\n";
        cout << "3. View Menu\n";
        cout << "4. Track Token\n";
        cout << "5. Admin Login\n";
        cout << "6. Manage Orders\n";
        cout << "7. View Order History\n";
        cout << "8. Exit\n";
        cout << "------------------------------------------\n";

        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');

            cout << "\nInvalid input! "
                 << "Please enter a number from 1 to 8.\n";

            continue;
        }

        try
        {
            switch (choice)
            {
                case 1:
                {
                    string name;
                    string userId;

                    cout << "\nEnter Student Name: ";

                    cin.ignore();
                    getline(cin, name);

                    cout << "Enter Student ID: ";
                    getline(cin, userId);

                    currentStudent =
                        Student(name, userId);

                    studentLoggedIn = true;

                    cout << "\nStudent login successful!\n";

                    currentStudent.displayStudent();

                    break;
                }

                case 2:
                {
                    if (!studentLoggedIn)
                    {
                        throw runtime_error(
                            "Please login as a student first."
                        );
                    }

                    Order order =
                        placeOrder(
                            currentStudent,
                            menu
                        );

                    canteen.addOrder(order);

                    cout << "\nOrder placed successfully!\n";

                    cout << "Your Token Number: "
                         << order.getToken()
                         << endl;

                    cout << "Total Amount: Rs. "
                         << fixed
                         << setprecision(2)
                         << order.calculateTotal()
                         << endl;

                    canteen.saveOrderHistory();

                    break;
                }

                case 3:
                {
                    displayMenu(menu);

                    break;
                }

                case 4:
                {
                    int tokenNumber;

                    cout << "\nEnter Token Number: ";

                    cin >> tokenNumber;

                    canteen.searchToken(tokenNumber);

                    break;
                }

                case 5:
                {
                    string adminId;

                    cout << "\nEnter Admin ID: ";

                    cin >> adminId;

                    if (adminId == "ADMIN")
                    {
                        adminLoggedIn = true;

                        cout << "\nAdmin login successful!\n";
                    }
                    else
                    {
                        throw runtime_error(
                            "Invalid Admin ID."
                        );
                    }

                    break;
                }

                case 6:
                {
                    if (!adminLoggedIn)
                    {
                        throw runtime_error(
                            "Please login as admin first."
                        );
                    }

                    Admin admin(
                        "Canteen Admin",
                        "ADMIN"
                    );

                    admin.manageOrders(canteen);

                    break;
                }

                case 7:
                {
                    canteen.loadOrderHistory();

                    break;
                }

                case 8:
                {
                    cout << "\nThank you for using Smart Canteen!\n";

                    break;
                }

                default:
                {
                    cout << "\nInvalid choice. "
                         << "Please enter a number from 1 to 8.\n";
                }
            }
        }
        catch (const exception& e)
        {
            cout << "\nError: "
                 << e.what()
                 << endl;
        }

    } while (choice != 8);

    return 0;
}