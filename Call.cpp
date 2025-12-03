#include "Call.h"
#include <iostream>

Call::Call() : callerName(""), destination(""), duration(0), cost(0.0) {}

Call::Call(const std::string& callerName, const std::string& destination,
           int duration, double cost)
    : callerName(callerName), destination(destination),
    duration(duration), cost(cost) {}

// ДОБАВЬТЕ:
std::string Call::getCallerName() const {
    return callerName;
}

std::string Call::getDestination() const {
    return destination;
}

int Call::getDuration() const {
    return duration;
}

double Call::getCost() const {
    return cost;
}

void Call::display() const {
    std::cout << "Абонент: " << callerName << ", Направление: " << destination
              << ", Длительность: " << duration << " мин, Стоимость: "
              << cost << " руб" << std::endl;
}
