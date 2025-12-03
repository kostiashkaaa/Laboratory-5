#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QLabel>
#include <QToolBar>
#include "DataManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddTariff();
    void onEditTariff();
    void onDeleteTariff();
    void onSortTariffs();

    void onAddClient();
    void onEditClient();
    void onDeleteClient();
    void onSortClients();

    void onAddVIPClient();
    void onEditVIPClient();
    void onDeleteVIPClient();
    void onSortVIPClients();

    void onAddCall();
    void onDeleteCall();
    void onSortCalls();
    void onShowCallStatistics();

    void onSaveData();      // Слот для кнопки Бэкапа
    void onLoadData();      // Слот для кнопки Восстановления
    void onInitTestData();  // Слот для загрузки тестовых данных
    void onClearAllData();
    void onAbout();

private:
    Ui::MainWindow *ui;
    DataManager *dataManager;

    // Таблицы
    QTableWidget *tariffsTable;
    QTableWidget *clientsTable;
    QTableWidget *vipClientsTable;
    QTableWidget *callsTable;

    QLabel *statsLabel;

    void updateTariffsTable();
    void updateClientsTable();
    void updateVIPClientsTable();
    void updateCallsTable();
    void updateStatistics();

    void setupTariffsTab();
    void setupClientsTab();
    void setupVIPClientsTab();
    void setupCallsTab();
    void setupMenuBar();
    void setupToolBar();

    void showMessage(const QString& title, const QString& message);
    void showError(const QString& message);
};

#endif
