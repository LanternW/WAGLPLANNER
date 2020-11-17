#ifndef ADD_LOADINGMACHINE_H
#define ADD_LOADINGMACHINE_H

#include <QDialog>
#include <QMainWindow>
#include "ui_add_loadingmachine.h"

namespace Ui {
class AddLoadingmachine;
}

class AddLoadingmachine : public QDialog
{
    Q_OBJECT

public:
    AddLoadingmachine(QWidget *parent = nullptr);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::AddLoadingmachine *ui;

};

#endif // ADD_LOADINGMACHINE_H
