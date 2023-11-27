#include <iostream>
#include <string>
#include <iomanip>

class Account {
protected:
    std::string accountNumber;
    std::string accountHolder;
    double balance;

public:
    Account(const std::string& number, const std::string& holder, double initialBalance)
        : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

    virtual void displayDetails() const {
        std::cout << "Account Details for " << accountHolder << " (ID: " << accountNumber << "):\n";
        std::cout << "   Holder: " << accountHolder << "\n";
        std::cout << "   Balance: $" << std::fixed << std::setprecision(2) << balance << "\n";
    }

    void deposit(double amount) {
        balance += amount;
    }

    virtual void withdraw(double amount) = 0;

    double getBalance() const {
        return balance;
    }

    void transfer(Account& receiver, double amount) {
        if (balance >= amount) {
            withdraw(amount);
            receiver.deposit(amount);
            std::cout << "Transfer successful.\n";
        } else {
            std::cout << "Insufficient balance for transfer.\n";
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Account& account);
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
        : Account(number, holder, initialBalance), interestRate(rate) {}

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2) << (interestRate * 100) << "%\n";
    }

    void withdraw(double amount) override {
        const double minBalance = 100; // Minimum balance required for savings account
        if (balance - amount >= minBalance) {
            balance -= amount;
        } else {
            std::cout << "Insufficient balance for withdrawal.\n";
        }
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double limit)
        : Account(number, holder, initialBalance), overdraftLimit(limit) {}

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit << "\n";
    }

    void withdraw(double amount) override {
        if (balance + overdraftLimit >= amount) {
            balance -= amount;
        } else {
            std::cout << "Withdrawal amount exceeds overdraft limit.\n";
        }
    }
};

std::ostream& operator<<(std::ostream& os, const Account& account) {
    account.displayDetails();
    return os;
}

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    std::cout << savings;
    std::cout << current;

    savings.deposit(500);
    current.withdraw(1000);

    std::cout << savings;
    std::cout << current;

    savings.transfer(current, 300);

    std::cout << savings;
    std::cout << current;

    return 0;
}
