#include "tabledialog.h"
#include "ui_tabledialog.h"
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QStringList>
#include <QAbstractItemModel>
#include <QSqlTableModel>
#include <QItemSelectionModel>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QStringList>

TableDialog::TableDialog(QSqlDatabase db, QString tableName, QStringList headers, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableDialog)
{
    ui->setupUi(this);

    this->tableName = tableName;
    this->db = db;
    this->headers = headers;
    idiotCounter = 0;

    connect(ui->buttonOK, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(ui->buttonCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));

    model = new QSqlTableModel(this, this->db);
    model->setTable(this->tableName);
    model->select();
    for(int i = 0; i < this->headers.length(); ++i)
    {
        model->setHeaderData(i, Qt::Horizontal, this->headers.at(i));
    }
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

TableDialog::~TableDialog()
{
    delete ui;
}

QString TableDialog::getData()
{
    return dataString;
}

int TableDialog::getIdiotCounter()
{
    return idiotCounter;
}

void TableDialog::accept()
{
    QItemSelectionModel *selected = ui->tableView->selectionModel();
    if(selected->selectedRows(0).isEmpty())
    {
        QMessageBox::warning(this, "No row selected", "You must select a row first");
        idiotCounter++;
        return;
    }
    dataString = selected->selectedRows(0).at(0).data().toString();
    QDialog::accept();
}
