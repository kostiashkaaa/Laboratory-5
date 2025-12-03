#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <vector>
#include <string>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QFile>

#include "Tariff.h"
#include "Client.h"
#include "VIPClient.h"
#include "Call.h"

class DataManager {
private:
    std::vector<Tariff> tariffs;
    std::vector<Client> clients;
    std::vector<VIPClient> vipClients;
    std::vector<Call> calls;

    QSqlDatabase db;
    QString dbPath;

    void createTables();
    void loadFromDatabase();

public:
    DataManager();
    ~DataManager();

    bool connectToDatabase();

    // CRUD методы
    void addTariff(const Tariff& tariff);
    void removeTariff(int index);
    void updateTariff(int index, const Tariff& tariff);
    const std::vector<Tariff>& getTariffs() const;
    Tariff* findTariffByCity(const std::string& city);

    void addClient(const Client& client);
    void removeClient(int index);
    void updateClient(int index, const Client& client);
    const std::vector<Client>& getClients() const;
    bool clientExists(const std::string& name) const;

    void addVIPClient(const VIPClient& client);
    void removeVIPClient(int index);
    void updateVIPClient(int index, const VIPClient& client);
    const std::vector<VIPClient>& getVIPClients() const;

    bool addCall(const Call& call);
    void removeCall(int index);
    const std::vector<Call>& getCalls() const;

    // Статистика
    double calculateClientTotalCost(const std::string& clientName) const;
    int getClientCallCount(const std::string& clientName) const;
    double calculateTotalRevenue() const;

    // Сортировка
    void sortTariffsByPrice(bool ascending = true);
    void sortClientsByName(bool ascending = true);
    void sortVIPClientsByDiscount(bool ascending = true);
    void sortCallsByDuration(bool ascending = true);

    // Бэкап и Восстановление
    bool backupDatabase(const QString& destinationPath);
    bool restoreDatabase(const QString& sourcePath);

    void clearAll();

    void initializeTestData();
};

#endif
