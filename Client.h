#ifndef CLIENT_H
#define CLIENT_H

#include "Person.h"

class Client : public Person {
private:
    double balance;

public:
    Client();
    Client(const std::string& name, const std::string& phoneNumber, double balance);

    // ДОБАВЬТЕ ЭТИ МЕТОДЫ:
    double getBalance() const;
    void setBalance(double balance);
    void addBalance(double amount);
    void deductBalance(double amount);

    void display() const override;
};

#endif
