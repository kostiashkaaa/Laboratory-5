#ifndef TARIFF_H
#define TARIFF_H

#include <string>

class Tariff {
private:
    std::string city;
    double pricePerMinute;
    double connectionFee;

public:
    // Конструкторы
    Tariff();
    Tariff(const std::string& city, double pricePerMinute, double connectionFee);

    // Геттеры
    std::string getCity() const;
    double getPricePerMinute() const;
    double getConnectionFee() const;

    // Сеттеры
    void setCity(const std::string& city);
    void setPricePerMinute(double price);
    void setConnectionFee(double fee);

    void display() const;


    bool operator<(const Tariff& other) const {
        return this->pricePerMinute < other.pricePerMinute;
    }

    bool operator==(const Tariff& other) const {
        return this->city == other.city &&
               this->pricePerMinute == other.pricePerMinute &&
               this->connectionFee == other.connectionFee;
    }
};

#endif
