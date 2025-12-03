#include "Client.h"
#include <iostream>

Client::Client() : Person(), balance(0.0) {}

Client::Client(const std::string& name, const std::string& phoneNumber, double balance)
    : Person(name, phoneNumber), balance(balance) {}

double Client::getBalance() const {
    return balance;
}

void Client::setBalance(double balance) {
    this->balance = balance;
}

void Client::addBalance(double amount) {
    balance += amount;
}

void Client::deductBalance(double amount) {
    if (balance >= amount) {
        balance -= amount;
    }
}

void Client::display() const {
    Person::display();
    std::cout << "Баланс: " << balance << " руб" << std::endl;
}
