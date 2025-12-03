// addtariffdialog.cpp
#include "addtariffdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

AddTariffDialog::AddTariffDialog(QWidget *parent)
    : QDialog(parent), isEditMode(false) {
    setWindowTitle("Добавить тариф");
    setupUI();
}

AddTariffDialog::AddTariffDialog(QWidget *parent, const Tariff& tariff)
    : QDialog(parent), isEditMode(true) {
    setWindowTitle("Редактировать тариф");
    setupUI();
    
    cityEdit->setText(QString::fromStdString(tariff.getCity()));
    priceSpinBox->setValue(tariff.getPricePerMinute());
    connectionFeeSpinBox->setValue(tariff.getConnectionFee());
}

AddTariffDialog::~AddTariffDialog() {
}

void AddTariffDialog::setupUI() {
    setMinimumWidth(400);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    QFormLayout *formLayout = new QFormLayout();
    
    cityEdit = new QLineEdit(this);
    cityEdit->setPlaceholderText("Москва");
    
    priceSpinBox = new QDoubleSpinBox(this);
    priceSpinBox->setRange(0.0, 1000.0);
    priceSpinBox->setDecimals(2);
    priceSpinBox->setSuffix(" ₽");
    priceSpinBox->setValue(2.0);
    
    connectionFeeSpinBox = new QDoubleSpinBox(this);
    connectionFeeSpinBox->setRange(0.0, 1000.0);
    connectionFeeSpinBox->setDecimals(2);
    connectionFeeSpinBox->setSuffix(" ₽");
    connectionFeeSpinBox->setValue(10.0);
    
    formLayout->addRow("Город:", cityEdit);
    formLayout->addRow("Цена за минуту:", priceSpinBox);
    formLayout->addRow("Плата за подключение:", connectionFeeSpinBox);
    
    mainLayout->addLayout(formLayout);
    
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton(isEditMode ? "Сохранить" : "Добавить", this);
    QPushButton *cancelButton = new QPushButton("Отмена", this);
    
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);
    
    mainLayout->addLayout(buttonsLayout);
    
    connect(okButton, &QPushButton::clicked, this, &AddTariffDialog::onAccept);
    connect(cancelButton, &QPushButton::clicked, this, &AddTariffDialog::onCancel);
}

void AddTariffDialog::onAccept() {
    validateAndAccept();
}

void AddTariffDialog::onCancel() {
    reject();
}

void AddTariffDialog::validateAndAccept() {
    QString city = cityEdit->text().trimmed();
    if (city.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите название города!");
        return;
    }
    
    if (priceSpinBox->value() <= 0) {
        QMessageBox::warning(this, "Ошибка", "Цена за минуту должна быть больше 0!");
        return;
    }
    
    if (connectionFeeSpinBox->value() < 0) {
        QMessageBox::warning(this, "Ошибка", "Плата за подключение не может быть отрицательной!");
        return;
    }
    
    accept();
}

Tariff AddTariffDialog::getTariff() const {
    return Tariff(
        cityEdit->text().trimmed().toStdString(),
        priceSpinBox->value(),
        connectionFeeSpinBox->value()
    );
}