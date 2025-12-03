#ifndef LOYALTYPROGRAM_H
#define LOYALTYPROGRAM_H

#include "Person.h"
#include <string>

class LoyaltyProgram : virtual public Person {
protected:
    std::string vipStatus;
    double discountPercent;
    
public:
    // Конструкторы
    LoyaltyProgram();
    LoyaltyProgram(const std::string& status, double discount);
    
    // Виртуальный деструктор
    virtual ~LoyaltyProgram();
    
    // Геттеры и сеттеры
    std::string getVIPStatus() const;
    double getDiscountPercent() const;
    
    void setVIPStatus(const std::string& status);
    void setDiscountPercent(double discount);
    
    // Применение скидки к цене
    double applyDiscount(double price) const;
    
    // Отображение информации о программе лояльности
    void displayLoyaltyInfo() const;
};

#endif
