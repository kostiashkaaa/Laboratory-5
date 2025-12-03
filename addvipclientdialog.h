// addvipclientdialog.h
// Диалоговое окно для добавления/редактирования VIP-клиента

#ifndef ADDVIPCLIENTDIALOG_H
#define ADDVIPCLIENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include "VIPClient.h"

class AddVIPClientDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit AddVIPClientDialog(QWidget *parent = nullptr);
    AddVIPClientDialog(QWidget *parent, const VIPClient& client);
    ~AddVIPClientDialog();
    
    VIPClient getVIPClient() const;
    
private slots:
    void onAccept();
    void onCancel();
    
private:
    QLineEdit *nameEdit;
    QLineEdit *phoneEdit;
    QDoubleSpinBox *balanceSpinBox;
    QDoubleSpinBox *discountSpinBox;
    QLineEdit *managerEdit;
    
    bool isEditMode;
    
    void setupUI();
    void validateAndAccept();
};

#endif // ADDVIPCLIENTDIALOG_H