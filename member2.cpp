#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

using namespace std;

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

    int getTokenNumber()
    {
        return tokenNumber;
    }

    string getStatus()
    {
        return status;
    }

    void setStatus(string newStatus)
    {
        if (status == "Pending" && newStatus == "Preparing")
        {
            status = newStatus;
        }
        else if (status == "Preparing" && newStatus == "Ready")
        {
            status = newStatus;
        }
        else if (status == "Ready" && newStatus == "Collected")
        {
            status = newStatus;
        }
        else
        {
            throw invalid_argument("Invalid status transition.");
        }
    }
};

class Canteen
{
private:
    string canteenName;
    vector<Token> tokens;

public:

    Canteen(string name)
    {
        canteenName = name;
    }

    string getCanteenName()
    {
        return canteenName;
    }

    void addToken(Token token)
    {
        tokens.push_back(token);
    }

    void displayTokens()
    {
        cout << "\n========== TOKEN STATUS ==========\n";

        for (int i = 0; i < tokens.size(); i++)
        {
            cout << "Token Number: "
                 << tokens[i].getTokenNumber()
                 << " | Status: "
                 << tokens[i].getStatus()
                 << endl;
        }
    }

    Token* findToken(int tokenNumber)
    {
        for (int i = 0; i < tokens.size(); i++)
        {
            if (tokens[i].getTokenNumber() == tokenNumber)
            {
                return &tokens[i];
            }
        }

        throw invalid_argument("Invalid token number.");
    }

    void searchToken(int tokenNumber)
    {
        try
        {
            Token* token = findToken(tokenNumber);

            cout << "\n========== TOKEN FOUND ==========\n";

            cout << "Token Number : "
                 << token->getTokenNumber()
                 << endl;

            cout << "Status       : "
                 << token->getStatus()
                 << endl;
        }
        catch (const exception& e)
        {
            cout << "Error: "
                 << e.what()
                 << endl;
        }
    }
};

class Admin
{
private:
    string adminName;

public:

    Admin(string name)
    {
        adminName = name;
    }

    string getAdminName()
    {
        return adminName;
    }

    void updateTokenStatus(
        Canteen& canteen,
        int tokenNumber,
        string newStatus)
    {
        try
        {
            Token* token = canteen.findToken(tokenNumber);

            token->setStatus(newStatus);

            cout << "Token status updated successfully.\n";
        }
        catch (const exception& e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }
};

int main()
{
    Canteen canteen("Smart Canteen");

    Token token1(1001);
    Token token2(1002);

    canteen.addToken(token1);
    canteen.addToken(token2);

    cout << "\nBefore update:\n";
    canteen.displayTokens();

    cout << "\nSearching for Token 1001...\n";
    canteen.searchToken(1001);

    Admin admin("Canteen Admin");

    cout << "\nUpdating Token 1001...\n";

    admin.updateTokenStatus(
        canteen,
        1001,
        "Preparing"
    );

    cout << "\nAfter update:\n";
    canteen.displayTokens();

    return 0;
}