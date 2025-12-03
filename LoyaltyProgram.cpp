#include "LoyaltyProgram.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Конструктор по умолчанию
LoyaltyProgram::LoyaltyProgram() 
    : Person(), vipStatus("Обычный"), discountPercent(0.0) {}

// Параметризованный конструктор
LoyaltyProgram::LoyaltyProgram(const string& status, double discount)
    : Person(), vipStatus(status), discountPercent(discount) {
    
    // Валидация процента скидки
    if (discountPercent < 0) discountPercent = 0;
    if (discountPercent > 50) discountPercent = 50;
}

// Деструктор
LoyaltyProgram::~LoyaltyProgram() {}

// Геттеры
string LoyaltyProgram::getVIPStatus() const {
    return vipStatus;
}

double LoyaltyProgram::getDiscountPercent() const {
    return discountPercent;
}

// Сеттеры
void LoyaltyProgram::setVIPStatus(const string& status) {
    this->vipStatus = status;
}

void LoyaltyProgram::setDiscountPercent(double discount) {
    if (discount < 0) discount = 0;
    if (discount > 50) discount = 50;
    this->discountPercent = discount;
}

double LoyaltyProgram::applyDiscount(double price) const {
    return price * (1.0 - discountPercent / 100.0);
}

void LoyaltyProgram::displayLoyaltyInfo() const {
    cout << fixed << setprecision(1);
    cout << "VIP-статус: " << vipStatus 
         << ", Скидка: " << discountPercent << "%" << endl;
}
