#include "DataManager.h"
#include <algorithm>
#include <iostream>

DataManager::DataManager() {
    // Указываем жесткий путь к файлу базы данных
    dbPath = "/Users/kostiashka/lab4_atc_gui/atc_database.sqlite";

    // При запуске подключаемся, создаем таблицы и загружаем данные в память
    if (connectToDatabase()) {
        createTables();
        loadFromDatabase();
    }
}

DataManager::~DataManager() {
    if (db.isOpen()) {
        db.close();
    }
}


bool DataManager::connectToDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "CRITICAL ERROR: Не удалось открыть БД:" << db.lastError().text();
        return false;
    }

    qDebug() << "==================================================";
    qDebug() << "БАЗА ДАННЫХ ПОДКЛЮЧЕНА:" << dbPath;
    qDebug() << "==================================================";

    return true;
}


bool DataManager::backupDatabase(const QString& destinationPath) {
    // Если файл назначения уже существует, удаляем его, чтобы перезаписать
    if (QFile::exists(destinationPath)) {
        if (!QFile::remove(destinationPath)) {
            qDebug() << "Ошибка: Не удалось перезаписать файл резервной копии.";
            return false;
        }
    }

    // Копируем текущий рабочий файл базы данных в выбранное пользователем место
    if (QFile::copy(dbPath, destinationPath)) {
        qDebug() << "Backup успешно создан:" << destinationPath;
        return true;
    } else {
        qDebug() << "Ошибка при создании Backup!";
        return false;
    }
}

bool DataManager::restoreDatabase(const QString& sourcePath) {
    // 1. Закрываем текущее соединение с БД, чтобы освободить файл
    db.close();

    // 2. Удаляем текущий рабочий файл базы данных
    if (QFile::exists(dbPath)) {
        if (!QFile::remove(dbPath)) {
            qDebug() << "Ошибка: Не удалось удалить текущую БД для замены.";
            // Пытаемся открыть обратно, чтобы программа не упала
            db.open();
            return false;
        }
    }

    // 3. Копируем файл пользователя (из бэкапа) на место нашей рабочей базы
    if (QFile::copy(sourcePath, dbPath)) {
        // 4. Открываем базу заново
        if (db.open()) {
            // 5. Загружаем данные из новой (восстановленной) базы в оперативную память
            loadFromDatabase();
            qDebug() << "База данных успешно восстановлена из:" << sourcePath;
            return true;
        } else {
            qDebug() << "Ошибка: Не удалось открыть восстановленную БД.";
        }
    } else {
        qDebug() << "Ошибка копирования файла восстановления.";
    }

    return false;
}


void DataManager::createTables() {
    QSqlQuery query;

    // Таблица тарифов
    query.exec("CREATE TABLE IF NOT EXISTS tariffs ("
               "city TEXT PRIMARY KEY, "
               "price REAL, "
               "fee REAL)");

    // Таблица клиентов
    query.exec("CREATE TABLE IF NOT EXISTS clients ("
               "name TEXT PRIMARY KEY, "
               "phone TEXT, "
               "balance REAL)");

    // Таблица VIP клиентов
    query.exec("CREATE TABLE IF NOT EXISTS vip_clients ("
               "name TEXT PRIMARY KEY, "
               "phone TEXT, "
               "balance REAL, "
               "discount REAL, "
               "manager TEXT)");

    // Таблица звонков
    query.exec("CREATE TABLE IF NOT EXISTS calls ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "client_name TEXT, "
               "destination TEXT, "
               "duration INTEGER, "
               "cost REAL)");
}

void DataManager::loadFromDatabase() {
    // Очищаем векторы перед загрузкой новых данных
    tariffs.clear();
    clients.clear();
    vipClients.clear();
    calls.clear();

    QSqlQuery query;

    // Загрузка Тарифов
    if (query.exec("SELECT * FROM tariffs")) {
        while (query.next()) {
            tariffs.push_back(Tariff(
                query.value("city").toString().toStdString(),
                query.value("price").toDouble(),
                query.value("fee").toDouble()
                ));
        }
    }

    // Загрузка Клиентов
    if (query.exec("SELECT * FROM clients")) {
        while (query.next()) {
            clients.push_back(Client(
                query.value("name").toString().toStdString(),
                query.value("phone").toString().toStdString(),
                query.value("balance").toDouble()
                ));
        }
    }

    // Загрузка VIP Клиентов
    if (query.exec("SELECT * FROM vip_clients")) {
        while (query.next()) {
            vipClients.push_back(VIPClient(
                query.value("name").toString().toStdString(),
                query.value("phone").toString().toStdString(),
                query.value("balance").toDouble(),
                query.value("discount").toDouble(),
                query.value("manager").toString().toStdString()
                ));
        }
    }

    // Загрузка Звонков
    if (query.exec("SELECT * FROM calls")) {
        while (query.next()) {
            calls.push_back(Call(
                query.value("client_name").toString().toStdString(),
                query.value("destination").toString().toStdString(),
                query.value("duration").toInt(),
                query.value("cost").toDouble()
                ));
        }
    }
}


void DataManager::addTariff(const Tariff& tariff) {
    QSqlQuery query;
    query.prepare("INSERT INTO tariffs (city, price, fee) VALUES (:city, :price, :fee)");
    query.bindValue(":city", QString::fromStdString(tariff.getCity()));
    query.bindValue(":price", tariff.getPricePerMinute());
    query.bindValue(":fee", tariff.getConnectionFee());

    if (query.exec()) {
        tariffs.push_back(tariff);
    } else {
        qDebug() << "SQL Error (addTariff):" << query.lastError().text();
    }
}

void DataManager::removeTariff(int index) {
    if (index >= 0 && index < static_cast<int>(tariffs.size())) {
        std::string city = tariffs[index].getCity();

        QSqlQuery query;
        query.prepare("DELETE FROM tariffs WHERE city = :city");
        query.bindValue(":city", QString::fromStdString(city));

        if (query.exec()) {
            tariffs.erase(tariffs.begin() + index);
        }
    }
}

void DataManager::updateTariff(int index, const Tariff& tariff) {
    if (index >= 0 && index < static_cast<int>(tariffs.size())) {
        removeTariff(index);
        addTariff(tariff);
    }
}

const std::vector<Tariff>& DataManager::getTariffs() const {
    return tariffs;
}

Tariff* DataManager::findTariffByCity(const std::string& city) {
    for (auto& tariff : tariffs) {
        if (tariff.getCity() == city) {
            return &tariff;
        }
    }
    return nullptr;
}


void DataManager::addClient(const Client& client) {
    QSqlQuery query;
    query.prepare("INSERT INTO clients (name, phone, balance) VALUES (:name, :phone, :balance)");
    query.bindValue(":name", QString::fromStdString(client.getName()));
    query.bindValue(":phone", QString::fromStdString(client.getPhoneNumber()));
    query.bindValue(":balance", client.getBalance());

    if (query.exec()) {
        clients.push_back(client);
    } else {
        qDebug() << "SQL Error (addClient):" << query.lastError().text();
    }
}

void DataManager::removeClient(int index) {
    if (index >= 0 && index < static_cast<int>(clients.size())) {
        std::string name = clients[index].getName();

        QSqlQuery query;
        query.prepare("DELETE FROM clients WHERE name = :name");
        query.bindValue(":name", QString::fromStdString(name));

        if (query.exec()) {
            clients.erase(clients.begin() + index);
        }
    }
}

void DataManager::updateClient(int index, const Client& client) {
    if (index >= 0 && index < static_cast<int>(clients.size())) {
        removeClient(index);
        addClient(client);
    }
}

const std::vector<Client>& DataManager::getClients() const {
    return clients;
}

bool DataManager::clientExists(const std::string& name) const {
    for (const auto& client : clients) {
        if (client.getName() == name) return true;
    }
    for (const auto& vip : vipClients) {
        if (vip.getName() == name) return true;
    }
    return false;
}


void DataManager::addVIPClient(const VIPClient& client) {
    QSqlQuery query;
    query.prepare("INSERT INTO vip_clients (name, phone, balance, discount, manager) "
                  "VALUES (:name, :phone, :balance, :discount, :manager)");
    query.bindValue(":name", QString::fromStdString(client.getName()));
    query.bindValue(":phone", QString::fromStdString(client.getPhoneNumber()));
    query.bindValue(":balance", client.getBalance());
    query.bindValue(":discount", client.getDiscount());
    query.bindValue(":manager", QString::fromStdString(client.getPersonalManager()));

    if (query.exec()) {
        vipClients.push_back(client);
    } else {
        qDebug() << "SQL Error (addVIPClient):" << query.lastError().text();
    }
}

void DataManager::removeVIPClient(int index) {
    if (index >= 0 && index < static_cast<int>(vipClients.size())) {
        std::string name = vipClients[index].getName();

        QSqlQuery query;
        query.prepare("DELETE FROM vip_clients WHERE name = :name");
        query.bindValue(":name", QString::fromStdString(name));

        if (query.exec()) {
            vipClients.erase(vipClients.begin() + index);
        }
    }
}

void DataManager::updateVIPClient(int index, const VIPClient& client) {
    if (index >= 0 && index < static_cast<int>(vipClients.size())) {
        removeVIPClient(index);
        addVIPClient(client);
    }
}

const std::vector<VIPClient>& DataManager::getVIPClients() const {
    return vipClients;
}


bool DataManager::addCall(const Call& call) {
    // Проверка целостности данных: клиент должен существовать
    if (!clientExists(call.getCallerName())) {
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO calls (client_name, destination, duration, cost) "
                  "VALUES (:name, :dest, :dur, :cost)");
    query.bindValue(":name", QString::fromStdString(call.getCallerName()));
    query.bindValue(":dest", QString::fromStdString(call.getDestination()));
    query.bindValue(":dur", call.getDuration());
    query.bindValue(":cost", call.getCost());

    if (query.exec()) {
        calls.push_back(call);
        return true;
    } else {
        qDebug() << "SQL Error (addCall):" << query.lastError().text();
        return false;
    }
}

void DataManager::removeCall(int index) {
    if (index >= 0 && index < static_cast<int>(calls.size())) {
        Call c = calls[index];
        QSqlQuery query;
        // Удаляем по совпадению всех полей
        query.prepare("DELETE FROM calls WHERE client_name = :name AND destination = :dest AND duration = :dur AND cost = :cost");
        query.bindValue(":name", QString::fromStdString(c.getCallerName()));
        query.bindValue(":dest", QString::fromStdString(c.getDestination()));
        query.bindValue(":dur", c.getDuration());
        query.bindValue(":cost", c.getCost());

        if (query.exec()) {
            calls.erase(calls.begin() + index);
        }
    }
}

const std::vector<Call>& DataManager::getCalls() const {
    return calls;
}


double DataManager::calculateClientTotalCost(const std::string& clientName) const {
    double total = 0.0;
    for (const auto& call : calls) {
        if (call.getCallerName() == clientName) {
            total += call.getCost();
        }
    }
    return total;
}

int DataManager::getClientCallCount(const std::string& clientName) const {
    int count = 0;
    for (const auto& call : calls) {
        if (call.getCallerName() == clientName) {
            count++;
        }
    }
    return count;
}

double DataManager::calculateTotalRevenue() const {
    double total = 0.0;
    for (const auto& call : calls) {
        total += call.getCost();
    }
    return total;
}

void DataManager::sortTariffsByPrice(bool ascending) {
    if (ascending) {
        std::sort(tariffs.begin(), tariffs.end(),
                  [](const Tariff& a, const Tariff& b) { return a.getPricePerMinute() < b.getPricePerMinute(); });
    } else {
        std::sort(tariffs.begin(), tariffs.end(),
                  [](const Tariff& a, const Tariff& b) { return a.getPricePerMinute() > b.getPricePerMinute(); });
    }
}

void DataManager::sortClientsByName(bool ascending) {
    if (ascending) {
        std::sort(clients.begin(), clients.end(),
                  [](const Client& a, const Client& b) { return a.getName() < b.getName(); });
    } else {
        std::sort(clients.begin(), clients.end(),
                  [](const Client& a, const Client& b) { return a.getName() > b.getName(); });
    }
}

void DataManager::sortVIPClientsByDiscount(bool ascending) {
    if (ascending) {
        std::sort(vipClients.begin(), vipClients.end(),
                  [](const VIPClient& a, const VIPClient& b) { return a.getDiscount() < b.getDiscount(); });
    } else {
        std::sort(vipClients.begin(), vipClients.end(),
                  [](const VIPClient& a, const VIPClient& b) { return a.getDiscount() > b.getDiscount(); });
    }
}

void DataManager::sortCallsByDuration(bool ascending) {
    if (ascending) {
        std::sort(calls.begin(), calls.end(),
                  [](const Call& a, const Call& b) { return a.getDuration() < b.getDuration(); });
    } else {
        std::sort(calls.begin(), calls.end(),
                  [](const Call& a, const Call& b) { return a.getDuration() > b.getDuration(); });
    }
}


void DataManager::clearAll() {
    QSqlQuery query;
    query.exec("DELETE FROM calls");
    query.exec("DELETE FROM vip_clients");
    query.exec("DELETE FROM clients");
    query.exec("DELETE FROM tariffs");

    tariffs.clear();
    clients.clear();
    vipClients.clear();
    calls.clear();
}
void DataManager::initializeTestData() {
    clearAll();
    // Пример данных
    addTariff(Tariff("Москва", 2.50, 0.50));
    addTariff(Tariff("Санкт-Петербург", 2.30, 0.50));
    addTariff(Tariff("Минск", 1.80, 0.20));

    addClient(Client("Иванов", "+79001234567", 100.0));
    addClient(Client("Петров", "+79007654321", 50.0));

    addVIPClient(VIPClient("Сидоров", "+79009876543", 500.0, 15.0, "Анна"));
}
