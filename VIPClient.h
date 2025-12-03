#ifndef VIPCLIENT_H
#define VIPCLIENT_H

#include "Client.h"
#include "LoyaltyProgram.h"
#include <string>

class VIPClient : public Client {
private:
    double discount;
    std::string personalManager;
    LoyaltyProgram loyaltyProgram;

public:
    VIPClient();
    VIPClient(const std::string& name, const std::string& phoneNumber,
              double balance, double discount, const std::string& manager);

    double getDiscount() const;
    void setDiscount(double discount);

    std::string getPersonalManager() const;
    void setPersonalManager(const std::string& manager);

    LoyaltyProgram& getLoyaltyProgram();

    void display() const override;
};

#endif
