#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QStringList>

namespace Ui {
class TableDialog;
}

class TableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TableDialog(QSqlDatabase db, QString tableName, QStringList headers, QWidget *parent = 0);
    ~TableDialog();
    QString dataString;
    QString getData();
    int idiotCounter;
    int getIdiotCounter();

public slots:
    void accept();

private:
    Ui::TableDialog *ui;
    QString tableName;
    QSqlTableModel *model;
    QSqlDatabase db;
    QStringList headers;
};

#endif // TABLEDIALOG_H
