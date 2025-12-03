// addcalldialog.h
// Диалоговое окно для добавления звонка

#ifndef ADDCALLDIALOG_H
#define ADDCALLDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include "Call.h"
#include "DataManager.h"

class AddCallDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit AddCallDialog(QWidget *parent, DataManager *dataManager);
    ~AddCallDialog();
    
    Call getCall() const;
    
private slots:
    void onAccept();
    void onCancel();
    void onDestinationChanged();
    void onDurationChanged();
    
private:
    QComboBox *callerComboBox;
    QComboBox *destinationComboBox;
    QSpinBox *durationSpinBox;
    QLabel *costLabel;
    
    DataManager *dataManager;
    double calculatedCost;
    
    void setupUI();
    void validateAndAccept();
    void updateCost();
};

#endif // ADDCALLDIALOG_H