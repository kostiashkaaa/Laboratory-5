// Person.h
#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>

class Person {
protected:
    std::string name;
    std::string phoneNumber;

public:
    Person();
    Person(const std::string& name, const std::string& phoneNumber); // <-- ИЗМЕНЕНО

    virtual ~Person();

    std::string getName() const;
    void setName(const std::string& name);

    std::string getPhoneNumber() const; // <-- ДОБАВЛЕНО
    void setPhoneNumber(const std::string& phone); // <-- ДОБАВЛЕНО

    // Имя "displayInfo" изменено на "display", чтобы override работал
    virtual void display() const; // <-- ИЗМЕНЕНО ИМЯ
};

#endif // PERSON_H
