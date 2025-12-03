#include "Person.h"
#include <iostream>

// Конструкторы изменены
Person::Person() : name(""), phoneNumber("") {}

Person::Person(const std::string& name, const std::string& phoneNumber)
    : name(name), phoneNumber(phoneNumber) {}

Person::~Person() {}

std::string Person::getName() const {
    return name;
}

void Person::setName(const std::string& name) {
    // --- ИСПРАВЛЕНО ---
    // БЫЛО: this.name = name;
    // НАДО:
    this->name = name;
}

// Реализация методов для phoneNumber
std::string Person::getPhoneNumber() const {
    return phoneNumber;
}

void Person::setPhoneNumber(const std::string& phone) {
    // --- ИСПРАВЛЕНО ---
    // БЫЛО: this.phoneNumber = phone;
    // НАДО:
    this->phoneNumber = phone;
    // (Также можно было написать просто: phoneNumber = phone;
    // так как имена не конфликтуют, но this-> тоже верно)
}

// Имя метода изменено
void Person::display() const {
    // Я также добавил std::endl для корректного переноса строки
    std::cout << "Имя: " << name << ", Телефон: " << phoneNumber << std::endl;
}
