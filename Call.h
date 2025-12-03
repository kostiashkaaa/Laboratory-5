#ifndef CALL_H
#define CALL_H

#include <string>

class Call {
private:
    std::string callerName;
    std::string destination;
    int duration;
    double cost;

public:
    Call();
    Call(const std::string& callerName, const std::string& destination,
         int duration, double cost);

    // ДОБАВЬТЕ ЭТИ МЕТОДЫ:
    std::string getCallerName() const;
    std::string getDestination() const;
    int getDuration() const;
    double getCost() const;

    void display() const;
};

#endif
