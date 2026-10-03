#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <stdexcept>

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

    // Constructor
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

    // Constructor
    User(string n = "", string id = "")
    {
        name = n;
        userId = id;
    }

    // Virtual function
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
// Inheritance + Function Overriding
// ============================================================

class Student : public User
{
public:

    Student(string n = "", string id = "")
        : User(n, id)
    {
    }

    // Function overriding
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

    // Constructor
    Order(int token, Student s)
        : tokenNumber(token), student(s)
    {
    }

    // Copy Constructor
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

        out << "Status       : Pending\n";

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
        cin >> choice;

        if (choice == 0)
        {
            break;
        }

        if (choice < 1 ||
            choice > menu.size())
        {
            cout << "\nInvalid food choice!\n";
            continue;
        }

        cout << "Enter quantity: ";
        cin >> quantity;

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

    // Check for empty order
    order.calculateTotal();

    return order;
}


// ============================================================
// MAIN FUNCTION
// ============================================================

int main()
{
    // --------------------------------------------------------
    // CANTEEN MENU
    // --------------------------------------------------------

    vector<FoodItem> menu =
    {
        FoodItem(1, "Veg Sandwich", 50),
        FoodItem(2, "Masala Dosa", 70),
        FoodItem(3, "Veg Burger", 80),
        FoodItem(4, "Cold Coffee", 60),
        FoodItem(5, "French Fries", 90)
    };


    cout << "\n========================================\n";
    cout << "       SMART CANTEEN TOKEN SYSTEM\n";
    cout << "========================================\n";


    // --------------------------------------------------------
    // STUDENT LOGIN
    // --------------------------------------------------------

    string name;
    string id;

    cout << "\n--------- STUDENT LOGIN ---------\n";

    cout << "Enter Student Name: ";
    getline(cin, name);

    cout << "Enter Student ID: ";
    getline(cin, id);

    Student student(name, id);

    student.displayStudent();


    // --------------------------------------------------------
    // RUNTIME POLYMORPHISM
    // --------------------------------------------------------

    User* userPtr = &student;

    cout << "\nRole using base class pointer: ";

    userPtr->displayRole();

    cout << endl;


    // --------------------------------------------------------
    // PLACE ORDER
    // --------------------------------------------------------

    try
    {
        Order order =
            placeOrder(student, menu);

        // Operator << overloaded
        cout << order;

        cout << "\nOrder placed successfully!\n";

        cout << "Your Token Number: "
             << order.getToken()
             << endl;


        // Copy constructor demonstration
        Order copiedOrder(order);

        cout << "\nCopy constructor executed successfully."
             << endl;
    }
    catch (const exception& e)
    {
        cout << "\nOrder could not be placed.\n";

        cout << "Reason: "
             << e.what()
             << endl;
    }


    cout << "\nThank you for using "
         << "Smart Canteen Token System!\n";

    return 0;
}