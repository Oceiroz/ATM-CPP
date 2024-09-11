#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>

using namespace std;
typedef filesystem::directory_iterator fs_dir;

template<typename T>
T GetInput(string inputMessage);
int GetChoice(string inputMessage, vector<string> options);
vector<string> GetAccounts();
void OpenAccount();
int Menu();
void CreateAccount();
bool SignIn();
bool Finish();
int AccountInfo();
void AccountBalance();
void DepositMoney();
void WithdrawMoney();
void OverdraftApply();
void HelpMenu();
bool SignOut();
void SaveFile();

string username, fName, lName;
bool overdraft;
int password;
float money, overdraftAmount = 0.0;
string filePath = "Accounts\\";
vector<string> accountList = GetAccounts();

int main()
{
    bool signOut = false;
    while (signOut == false)
    {
        int area = 0;
        bool verified = false;
        int start;
        start = Menu();
        if (start == 1)
        {
            CreateAccount();
        }
        else
        {
            verified = SignIn();
        }
        while (verified == true)
        {
            area = AccountInfo();
            if (area == 1)
            {
                AccountBalance();
            }
            else if (area == 2)
            {
                DepositMoney();
            }
            else if (area == 3)
            {
                WithdrawMoney();
            }
            else if (area == 4)
            {
                OverdraftApply();
            }
            else if (area == 5)
            {
                HelpMenu();
            }
            else if (area == 6)
            {
                signOut = SignOut();
                verified = false;
            }
        }
    }
}
template<typename T>
T GetInput(string inputMessage)
{
    T userInput;
    while (true)
    {
        cout << inputMessage << endl;
        string rawInput;
        getline(cin, rawInput);
        try
        {
            istringstream ss(rawInput);
            ss >> userInput;
            if (ss.fail() || ss.peek() != EOF)
            {
                throw new exception;
            }
            break;
        }
        catch (...)
        {
            cout << "This input is of the wrong data type" << endl;
        }
    }
    return userInput;
}
int GetChoice(string inputMessage, vector<string> options)
{
    int userInput = 0;
    while (true)
    {
        for (int x = 0; x < options.size(); x++)
        {
            cout << endl << x + 1 << " ---> " << options[x] << endl;
        }
        cout << endl << inputMessage << endl;
        string rawInput;
        getline(cin, rawInput);
        try
        {
            userInput = stoi(rawInput);
            if (userInput < 1 || userInput > options.size())
            {
                throw new exception;
            }
            break;
        }
        catch (...)
        {
            cout << endl << "this is either the wrong data type or out of the range of your given options" << endl;
        }
    }
    return userInput;
}
vector<string> GetAccounts()
{
    vector<string> accounts;
    for (const auto& account : fs_dir(filePath))
    {
        string newAccount;
        string tempAccount = account.path().filename().string();
        newAccount = tempAccount.erase(8, 4);
        accounts.push_back(newAccount);
    }
    return accounts;
    //may not worky bim bam bim
}
void OpenAccount()
{
    ifstream accountFile(filePath + username + ".txt");
    vector<string> lines;
    string line;
    while (getline(accountFile, line))
    {
        lines.push_back(line); 
    }
    accountFile.close();
    fName = lines[0];
    lName = lines[1];
    money = stof(lines[2]);
    password = stoi(lines[3]);
    istringstream ss(lines[4]);
    ss >> overdraft;
    overdraftAmount = stof(lines[5]);
}
int Menu()
{
    vector<string> options = { "Create Account", "Sign-In" };
    int choice = GetChoice("what would you like to do?", options);
    return choice;
}
void CreateAccount()
{
    int rawCharacter;
    string usernameTemp;
    bool x = true;
    while (x == true)
    {
        srand((unsigned)time(NULL));
        int y = 0;
        for (; y < 8; y++)
        {
            rawCharacter = rand() % 9;
            string character = to_string(rawCharacter);
            usernameTemp.append(character);
        }
        if (accountList.size() == 0)
        {
            username = usernameTemp;
            x = false;
        }
        else
        {
            for (string name : accountList)
            {
                if (name != usernameTemp)
                {
                    username = usernameTemp;
                    x = false;
                }
            }
        }
    }
    fName = GetInput<string>("What is your first name?");
    lName = GetInput<string>("What is your last name?");
    money = GetInput<float>("How much money do you currently have?");
    while (true)
    {
        int passwordTemp = GetInput<int>("Please enter a 4 digit password");
        string strPassword = to_string(passwordTemp);
        if (strPassword.length() != 4)
        {
            cout << "your password is not 4 digits" << endl;
        }
        else
        {
            password = passwordTemp;
            break;
        }
    }
    while (true)
    {
        int passwordReenter = GetInput<int>("Please re-enter your password");
        if (password != passwordReenter)
        {
            cout << "Your password does not match the initial password" << endl;
        }
        else
        {
            break;
        }
    }
    overdraft = false;
    overdraftAmount = 0.00;
    SaveFile();
}
bool SignIn()
{
    bool verified;
    bool x = true;
    while (x == true)
    {
        string tempUsername = GetInput<string>("What is your username?");
        for (string account : accountList)
        {
            if (account == tempUsername)
            {
                x = false;
                username = tempUsername;
                OpenAccount();
                break;
            }
        }
        if (x == true)
        {
            cout << "This ID could not be found, please try again:" << endl;
        }
    }
    while (true)
    {
        int passTemp = GetInput<int>("Please enter your 4 digit pin");
        if (passTemp = password)
        {
            break;
        }
        else
        {
            cout << "Password is incorrect" << endl;
        }
    }
    verified = true;
    return verified;
}
bool Finish()
{
    bool x = true;
    vector<string> options = { "Yes", "No" };
    int choice = GetChoice("are you finished in this section?", options);
    if (choice == 1)
    {
        x = false;
    }
    return x;
}
int AccountInfo()
{
    cout << "Welcome " << fName<<":" << endl;
    vector<string> options = { "Account Balance", "Deposit Money", "Withdraw Money", "Apply Overdraft", "Help", "Sign-Out" };
    int area = GetChoice("Where woudld you like to go?", options);
    return area;
}
void AccountBalance()
{
    bool x = true;
    while (x == true)
    {
        cout << "Your account balance is: " << money << endl;
        x = Finish();
    }
}
void DepositMoney()
{
    bool x = true;
    while (x == true)
    {
        double amountEnter = GetInput<float>("How much money would you like to deposit?");
        money += amountEnter;
        cout << "Your new total is: " << money << endl;
        x = Finish();
    }
}
void WithdrawMoney()
{
    bool x = true;
    while (x == true)
    {
        double amountRemove = GetInput<float>("How much money would you like to withdraw?");
        if (amountRemove > money && overdraft == false)
        {
            cout << "You cannot withdraw that amount" << endl;
        }
        else if (amountRemove > money && overdraft == true)
        {
            double totalTemp = money + overdraftAmount;
            if (amountRemove > totalTemp)
            {
                cout << "You cannot withdraw that amount as it goes beyond your overdraft" << endl;
            }
        }
        else
        {
            money -= amountRemove;
            cout << "Your new total is:" << money << endl;
        }
        x = Finish();
    }
}
void OverdraftApply()
{
    bool x = true;
    while (x == true)
    {
        if (overdraft == true)
        {
            vector<string> options = { "Yes", "No" };
            int choice = GetChoice("It looks like you already have an overdraft... would you like to change it?", options);
            if (choice == 1)
            {
                cout << "If you set it to 0, we will remove the overdraft" << endl;
                overdraftAmount = GetInput<float>("What would you like your overdraft to be?");
            }
        }
        else
        {
            vector<string> options = { "Yes", "No" };
            int choice = GetChoice("It looks like you do not have an overdraft... would you like one?", options);
            if (choice == 1)
            {
                overdraftAmount = GetInput<float>("What would you like your overdraft to be?");
            }
        }
        if (overdraftAmount == 0)
        {
            overdraft = false;
        }
        else
        {
            overdraft = true;
        }
        x = Finish();
    }
}
void HelpMenu()
{
    while (true)
    {
        cout << "NO" << endl;
        break;
    }
}
bool SignOut()
{
    bool signOut = true;
    while (true)
    {
        vector<string> options = { "Yes", "No" };
        int confirmation = GetChoice("Are you sure you wish to sign out?", options);
        if (confirmation == 1)
        {
            SaveFile();
            break;
        }
        else
        {
            AccountInfo();
        }
    }
    return signOut;
}
void SaveFile()
{
    ofstream outFile (filePath + username + ".txt");
    vector<string> credentials = { fName, lName, to_string(money), to_string(password), to_string(overdraft), to_string(overdraftAmount) };
    for (string cred : credentials)
    {
        outFile << cred << endl;
    }
    outFile.close();
}