// addvipclientdialog.cpp
#include "addvipclientdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QRegularExpression>

AddVIPClientDialog::AddVIPClientDialog(QWidget *parent)
    : QDialog(parent), isEditMode(false) {
    setWindowTitle("Добавить VIP-клиента");
    setupUI();
}

AddVIPClientDialog::AddVIPClientDialog(QWidget *parent, const VIPClient& client)
    : QDialog(parent), isEditMode(true) {
    setWindowTitle("Редактировать VIP-клиента");
    setupUI();
    
    nameEdit->setText(QString::fromStdString(client.getName()));
    phoneEdit->setText(QString::fromStdString(client.getPhoneNumber()));
    balanceSpinBox->setValue(client.getBalance());
    discountSpinBox->setValue(client.getDiscount());
    managerEdit->setText(QString::fromStdString(client.getPersonalManager()));
}

AddVIPClientDialog::~AddVIPClientDialog() {
}

void AddVIPClientDialog::setupUI() {
    setMinimumWidth(450);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    QFormLayout *formLayout = new QFormLayout();
    
    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Алексей Богатов");
    
    phoneEdit = new QLineEdit(this);
    phoneEdit->setPlaceholderText("+79009999999");
    
    balanceSpinBox = new QDoubleSpinBox(this);
    balanceSpinBox->setRange(0.0, 100000.0);
    balanceSpinBox->setDecimals(2);
    balanceSpinBox->setSuffix(" ₽");
    balanceSpinBox->setValue(1000.0);
    
    discountSpinBox = new QDoubleSpinBox(this);
    discountSpinBox->setRange(0.0, 100.0);
    discountSpinBox->setDecimals(1);
    discountSpinBox->setSuffix(" %");
    discountSpinBox->setValue(10.0);
    
    managerEdit = new QLineEdit(this);
    managerEdit->setPlaceholderText("Менеджер Анна");
    
    formLayout->addRow("Имя:", nameEdit);
    formLayout->addRow("Телефон:", phoneEdit);
    formLayout->addRow("Баланс:", balanceSpinBox);
    formLayout->addRow("Скидка:", discountSpinBox);
    formLayout->addRow("Персональный менеджер:", managerEdit);
    
    mainLayout->addLayout(formLayout);
    
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton(isEditMode ? "Сохранить" : "Добавить", this);
    QPushButton *cancelButton = new QPushButton("Отмена", this);
    
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);
    
    mainLayout->addLayout(buttonsLayout);
    
    connect(okButton, &QPushButton::clicked, this, &AddVIPClientDialog::onAccept);
    connect(cancelButton, &QPushButton::clicked, this, &AddVIPClientDialog::onCancel);
}

void AddVIPClientDialog::onAccept() {
    validateAndAccept();
}

void AddVIPClientDialog::onCancel() {
    reject();
}

void AddVIPClientDialog::validateAndAccept() {
    QString name = nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите имя VIP-клиента!");
        return;
    }
    
    QString phone = phoneEdit->text().trimmed();
    if (phone.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите номер телефона!");
        return;
    }
    
    QRegularExpression phoneRegex("^\\+?[0-9]{10,15}$");
    if (!phoneRegex.match(phone).hasMatch()) {
        QMessageBox::warning(this, "Ошибка", "Неверный формат номера телефона!");
        return;
    }
    
    if (balanceSpinBox->value() < 0) {
        QMessageBox::warning(this, "Ошибка", "Баланс не может быть отрицательным!");
        return;
    }
    
    if (discountSpinBox->value() < 0 || discountSpinBox->value() > 100) {
        QMessageBox::warning(this, "Ошибка", "Скидка должна быть от 0 до 100%!");
        return;
    }
    
    QString manager = managerEdit->text().trimmed();
    if (manager.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите имя персонального менеджера!");
        return;
    }
    
    accept();
}

VIPClient AddVIPClientDialog::getVIPClient() const {
    return VIPClient(
        nameEdit->text().trimmed().toStdString(),
        phoneEdit->text().trimmed().toStdString(),
        balanceSpinBox->value(),
        discountSpinBox->value(),
        managerEdit->text().trimmed().toStdString()
    );
}