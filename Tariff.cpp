#include "Tariff.h"
#include <iostream>

Tariff::Tariff() : city(""), pricePerMinute(0.0), connectionFee(0.0) {}

Tariff::Tariff(const std::string& city, double pricePerMinute, double connectionFee)
    : city(city), pricePerMinute(pricePerMinute), connectionFee(connectionFee) {}

// ДОБАВЬТЕ ЭТИ МЕТОДЫ:
std::string Tariff::getCity() const {
    return city;
}

double Tariff::getPricePerMinute() const {
    return pricePerMinute;
}

double Tariff::getConnectionFee() const {
    return connectionFee;
}

void Tariff::setCity(const std::string& city) {
    this->city = city;
}

void Tariff::setPricePerMinute(double price) {
    this->pricePerMinute = price;
}

void Tariff::setConnectionFee(double fee) {
    this->connectionFee = fee;
}

void Tariff::display() const {
    std::cout << "Город: " << city << ", Цена: " << pricePerMinute
              << " руб/мин, Подключение: " << connectionFee << " руб" << std::endl;
}
