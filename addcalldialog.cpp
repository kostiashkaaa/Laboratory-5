// addcalldialog.cpp
#include "addcalldialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>

AddCallDialog::AddCallDialog(QWidget *parent, DataManager *dm)
    : QDialog(parent), dataManager(dm), calculatedCost(0.0) {
    setWindowTitle("Добавить звонок");
    setupUI();
}

AddCallDialog::~AddCallDialog() {
}

void AddCallDialog::setupUI() {
    setMinimumWidth(400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();

    callerComboBox = new QComboBox(this);
    // ЗАЩИТА: Запрещаем ручной ввод, разрешаем только выбор из списка
    callerComboBox->setEditable(false);

    const auto& clients = dataManager->getClients();
    for (const auto& client : clients) {
        callerComboBox->addItem(QString::fromStdString(client.getName()));
    }
    const auto& vipClients = dataManager->getVIPClients();
    for (const auto& vip : vipClients) {
        callerComboBox->addItem(QString::fromStdString(vip.getName()) + " (VIP)");
    }

    if (callerComboBox->count() == 0) {
        QMessageBox::warning(this, "Предупреждение", "Сначала добавьте клиентов!");
        reject();
        return;
    }

    destinationComboBox = new QComboBox(this);
    const auto& tariffs = dataManager->getTariffs();
    for (const auto& tariff : tariffs) {
        destinationComboBox->addItem(QString::fromStdString(tariff.getCity()));
    }

    if (destinationComboBox->count() == 0) {
        QMessageBox::warning(this, "Предупреждение", "Сначала добавьте тарифы!");
        reject();
        return;
    }

    durationSpinBox = new QSpinBox(this);
    durationSpinBox->setRange(1, 9999);
    durationSpinBox->setSuffix(" мин");
    durationSpinBox->setValue(5);

    costLabel = new QLabel("0.00 ₽", this);
    costLabel->setStyleSheet("QLabel { font-size: 14pt; font-weight: bold; color: green; }");

    formLayout->addRow("Абонент:", callerComboBox);
    formLayout->addRow("Направление:", destinationComboBox);
    formLayout->addRow("Длительность:", durationSpinBox);
    formLayout->addRow("Стоимость звонка:", costLabel);

    mainLayout->addLayout(formLayout);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Добавить", this);
    QPushButton *cancelButton = new QPushButton("Отмена", this);

    buttonsLayout->addStretch();
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonsLayout);

    connect(okButton, &QPushButton::clicked, this, &AddCallDialog::onAccept);
    connect(cancelButton, &QPushButton::clicked, this, &AddCallDialog::onCancel);
    connect(destinationComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AddCallDialog::onDestinationChanged);
    connect(durationSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &AddCallDialog::onDurationChanged);

    updateCost();
}

void AddCallDialog::onAccept() {
    validateAndAccept();
}

void AddCallDialog::onCancel() {
    reject();
}

void AddCallDialog::onDestinationChanged() {
    updateCost();
}

void AddCallDialog::onDurationChanged() {
    updateCost();
}

void AddCallDialog::updateCost() {
    QString destination = destinationComboBox->currentText();
    int duration = durationSpinBox->value();

    Tariff* tariff = dataManager->findTariffByCity(destination.toStdString());
    if (tariff) {
        calculatedCost = tariff->getConnectionFee() + (tariff->getPricePerMinute() * duration);

        QString caller = callerComboBox->currentText();
        if (caller.contains("(VIP)")) {
            const auto& vipClients = dataManager->getVIPClients();
            QString cleanName = caller.replace(" (VIP)", "");
            for (const auto& vip : vipClients) {
                if (QString::fromStdString(vip.getName()) == cleanName) {
                    double discount = vip.getDiscount() / 100.0;
                    calculatedCost *= (1.0 - discount);
                    break;
                }
            }
        }

        costLabel->setText(QString::number(calculatedCost, 'f', 2) + " ₽");
    }
}

void AddCallDialog::validateAndAccept() {
    if (callerComboBox->currentIndex() < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите абонента!");
        return;
    }

    if (destinationComboBox->currentIndex() < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите направление!");
        return;
    }

    if (durationSpinBox->value() <= 0) {
        QMessageBox::warning(this, "Ошибка", "Длительность должна быть больше 0!");
        return;
    }

    accept();
}

Call AddCallDialog::getCall() const {
    QString caller = callerComboBox->currentText();
    caller = caller.replace(" (VIP)", "");

    return Call(
        caller.toStdString(),
        destinationComboBox->currentText().toStdString(),
        durationSpinBox->value(),
        calculatedCost
        );
}
