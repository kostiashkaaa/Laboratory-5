// addclientdialog.h
// Диалоговое окно для добавления/редактирования клиента

#ifndef ADDCLIENTDIALOG_H
#define ADDCLIENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include "Client.h"

class AddClientDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit AddClientDialog(QWidget *parent = nullptr);
    AddClientDialog(QWidget *parent, const Client& client);
    ~AddClientDialog();
    
    Client getClient() const;
    
private slots:
    void onAccept();
    void onCancel();
    
private:
    QLineEdit *nameEdit;
    QLineEdit *phoneEdit;
    QDoubleSpinBox *balanceSpinBox;
    
    bool isEditMode;
    
    void setupUI();
    void validateAndAccept();
};

#endif // ADDCLIENTDIALOG_H