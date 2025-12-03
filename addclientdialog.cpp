// addclientdialog.cpp
#include "addclientdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QRegularExpression>

AddClientDialog::AddClientDialog(QWidget *parent)
    : QDialog(parent), isEditMode(false) {
    setWindowTitle("Добавить клиента");
    setupUI();
}

AddClientDialog::AddClientDialog(QWidget *parent, const Client& client)
    : QDialog(parent), isEditMode(true) {
    setWindowTitle("Редактировать клиента");
    setupUI();
    
    nameEdit->setText(QString::fromStdString(client.getName()));
    phoneEdit->setText(QString::fromStdString(client.getPhoneNumber()));
    balanceSpinBox->setValue(client.getBalance());
}

AddClientDialog::~AddClientDialog() {
}

void AddClientDialog::setupUI() {
    setMinimumWidth(400);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    QFormLayout *formLayout = new QFormLayout();
    
    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Иван Иванов");
    
    phoneEdit = new QLineEdit(this);
    phoneEdit->setPlaceholderText("+79001234567");
    
    balanceSpinBox = new QDoubleSpinBox(this);
    balanceSpinBox->setRange(0.0, 100000.0);
    balanceSpinBox->setDecimals(2);
    balanceSpinBox->setSuffix(" ₽");
    balanceSpinBox->setValue(0.0);
    
    formLayout->addRow("Имя:", nameEdit);
    formLayout->addRow("Телефон:", phoneEdit);
    formLayout->addRow("Баланс:", balanceSpinBox);
    
    mainLayout->addLayout(formLayout);
    
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton(isEditMode ? "Сохранить" : "Добавить", this);
    QPushButton *cancelButton = new QPushButton("Отмена", this);
    
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);
    
    mainLayout->addLayout(buttonsLayout);
    
    connect(okButton, &QPushButton::clicked, this, &AddClientDialog::onAccept);
    connect(cancelButton, &QPushButton::clicked, this, &AddClientDialog::onCancel);
}

void AddClientDialog::onAccept() {
    validateAndAccept();
}

void AddClientDialog::onCancel() {
    reject();
}

void AddClientDialog::validateAndAccept() {
    QString name = nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите имя клиента!");
        return;
    }
    
    QString phone = phoneEdit->text().trimmed();
    if (phone.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите номер телефона!");
        return;
    }
    
    QRegularExpression phoneRegex("^\\+?[0-9]{10,15}$");
    if (!phoneRegex.match(phone).hasMatch()) {
        QMessageBox::warning(this, "Ошибка", "Неверный формат номера телефона! Используйте формат: +79001234567");
        return;
    }
    
    if (balanceSpinBox->value() < 0) {
        QMessageBox::warning(this, "Ошибка", "Баланс не может быть отрицательным!");
        return;
    }
    
    accept();
}

Client AddClientDialog::getClient() const {
    return Client(
        nameEdit->text().trimmed().toStdString(),
        phoneEdit->text().trimmed().toStdString(),
        balanceSpinBox->value()
    );
}