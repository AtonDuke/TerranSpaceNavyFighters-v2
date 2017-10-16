#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QStringList>
#include <QSqlDatabase>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QSqlDatabase db, QStringList dataList, QWidget *parent = 0);
    ~Dialog();
    QStringList dataList;
    void accept();
    bool edit;
    QString prevNumber;
    int idiotCounter;
    int  getIdiotCounter();
    QSqlDatabase db;
public slots:
    void onButtonSelectPilot();
    void onButtonSelectCopilot();
    void onButtonSelectModel();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
