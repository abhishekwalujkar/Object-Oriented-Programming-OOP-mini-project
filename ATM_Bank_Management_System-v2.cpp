#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Base Account Class
class Account {
protected:
    int accNo;
    string name;
    int pin;
    double balance;

public:
    Account(int n, string nm, int p, double b)
        : accNo(n), name(nm), pin(p), balance(b) {}

    virtual ~Account() {}

    int getAccNo() { return accNo; }
    string getName() { return name; }
    double getBalance() { return balance; }
    bool validatePin(int p) { return pin == p; }
    void setPin(int p) { pin = p; }

    void deposit(double amt) {
        balance += amt;
        cout << "Successfully deposited Rs." << amt << endl;
    }

    virtual void withdraw(double amt) {
        if (amt > balance) {
            cout << "Insufficient Balance!" << endl;
        } else {
            balance -= amt;
            cout << "Successfully withdrawn Rs." << amt << endl;
        }
    }

    void display() {
        cout << "Acc No: " << accNo << " | Name: " << name
             << " | Balance: Rs." << balance << endl;
    }
};

// Savings Account (Inheritance)
class SavingsAccount : public Account {
public:
    SavingsAccount(int n, string nm, int p, double b)
        : Account(n, nm, p, b) {}

    void withdraw(double amt) override {
        if (amt > 20000) {
            cout << "Limit exceeded! Maximum withdrawal for Savings is Rs. 20,000" << endl;
        } else {
            Account::withdraw(amt);
        }
    }
};

// Current Account (Inheritance)
class CurrentAccount : public Account {
public:
    CurrentAccount(int n, string nm, int p, double b)
        : Account(n, nm, p, b) {}
};

// ATM System Class
class ATMSystem {
    vector<Account*> accounts;
    int nextAccNo = 1001;

public:
    ATMSystem() { loadFromFile(); }
    ~ATMSystem() {
        saveToFile();
        for (auto acc : accounts) {
            delete acc;
        }
    }

    // --- Admin Functions ---
    void createAccount() {
        string name;
        int pin;
        double balance;
        int type;

        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Set 4-digit PIN: ";
        cin >> pin;
        cout << "Initial Balance: ";
        cin >> balance;
        cout << "Account Type (1. Savings, 2. Current): ";
        cin >> type;

        if (type == 1)
            accounts.push_back(new SavingsAccount(nextAccNo++, name, pin, balance));
        else
            accounts.push_back(new CurrentAccount(nextAccNo++, name, pin, balance));

        cout << "Account created successfully! Account Number: " << (nextAccNo - 1) << endl;
    }

    void viewAllAccounts() {
        if (accounts.empty()) {
            cout << "No accounts found." << endl;
            return;
        }
        for (auto acc : accounts)
            acc->display();
    }

    void deleteAccount() {
        int no;
        cout << "Enter Account Number to delete: ";
        cin >> no;
        for (size_t i = 0; i < accounts.size(); i++) {
            if (accounts[i]->getAccNo() == no) {
                delete accounts[i];
                accounts.erase(accounts.begin() + i);
                cout << "Account deleted." << endl;
                return;
            }
        }
        cout << "Account not found!" << endl;
    }

    // --- Customer Functions ---
    Account* login() {
        int no, pin;
        cout << "Enter Account Number: ";
        cin >> no;
        cout << "Enter PIN: ";
        cin >> pin;
        for (auto acc : accounts) {
            if (acc->getAccNo() == no && acc->validatePin(pin))
                return acc;
        }
        return nullptr;
    }

    void customerMenu(Account* user) {
        int choice;
        do {
            cout << "\n--- Customer Menu ---\n"
                 << "1. Deposit\n"
                 << "2. Withdraw\n"
                 << "3. Balance Inquiry\n"
                 << "4. Change PIN\n"
                 << "5. Logout\n"
                 << "Choice: ";
            cin >> choice;

            if (choice == 1) {
                double amt;
                cout << "Enter amount: ";
                cin >> amt;
                user->deposit(amt);
            } else if (choice == 2) {
                double amt;
                cout << "Enter amount: ";
                cin >> amt;
                user->withdraw(amt);
            } else if (choice == 3) {
                user->display();
            } else if (choice == 4) {
                int newPin;
                cout << "Enter new 4-digit PIN: ";
                cin >> newPin;
                user->setPin(newPin);
                cout << "PIN updated successfully!" << endl;
            }
        } while (choice != 5);
    }

    void adminMenu() {
        string id, pass;
        cout << "Admin ID: ";
        cin >> id;
        cout << "Password: ";
        cin >> pass;

        if (id == "admin" && pass == "1234") {
            int choice;
            do {
                cout << "\n--- Admin Menu ---\n"
                     << "1. Create Account\n"
                     << "2. View All\n"
                     << "3. Delete Account\n"
                     << "4. Logout\n"
                     << "Choice: ";
                cin >> choice;

                if (choice == 1)
                    createAccount();
                else if (choice == 2)
                    viewAllAccounts();
                else if (choice == 3)
                    deleteAccount();
            } while (choice != 4);
        } else {
            cout << "Invalid Admin Credentials!" << endl;
        }
    }

    // --- File Handling ---
    void saveToFile() {
        ofstream fout("accounts.txt");
        for (auto acc : accounts) {
            fout << acc->getAccNo() << " " << acc->getName() << " " << acc->getBalance() << endl;
        }
        fout.close();
    }

    void loadFromFile() {
        ifstream fin("accounts.txt");
        if (!fin) return;
        int no;
        string name;
        double bal;
        while (fin >> no >> name >> bal) {
            accounts.push_back(new Account(no, name, 1234, bal));
            if (no >= nextAccNo)
                nextAccNo = no + 1;
        }
        fin.close();
    }

    void run() {
        int choice;
        do {
            cout << "\n=== WELCOME TO ATM SYSTEM ===\n"
                 << "1. Customer Login\n"
                 << "2. Admin Login\n"
                 << "3. Exit\n"
                 << "Choice: ";
            cin >> choice;

            if (choice == 1) {
                Account* user = login();
                if (user)
                    customerMenu(user);
                else
                    cout << "Invalid Account No or PIN!" << endl;
            } else if (choice == 2) {
                adminMenu();
            }
        } while (choice != 3);
    }
};

int main() {
    ATMSystem atm;
    atm.run();
    return 0;
}
