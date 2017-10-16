#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QStringList>
#include "tabledialog.h"
#include <QSqlDatabase>

Dialog::Dialog(QSqlDatabase db, QStringList dataList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(ui->buttonOK,SIGNAL(clicked(bool)),this,SLOT(accept()));
    connect(ui->buttonCancel,SIGNAL(clicked(bool)),this,SLOT(reject()));
    connect(ui->buttonSelectPilot, SIGNAL(clicked(bool)), this, SLOT(onButtonSelectPilot()));
    connect(ui->buttonSelectCopilot, SIGNAL(clicked(bool)), this, SLOT(onButtonSelectCopilot()));
    connect(ui->buttonSelectModel, SIGNAL(clicked(bool)), this, SLOT(onButtonSelectModel()));

    this->db = db;
    this->dataList = dataList;
    edit = false;
    if(!this->dataList.isEmpty() && this->dataList.count() == 5)
    {
        ui->lineNumber->setText(this->dataList.at(0));
        ui->linePilot->setText(this->dataList.at(1));
        ui->lineCopilot->setText(this->dataList.at(2));
        ui->lineModel->setText(this->dataList.at(3));
        ui->lineMothership->setText(this->dataList.at(4));
        edit = true;
        prevNumber = this->dataList.at(0);
    }
    this->dataList.clear();

    idiotCounter = 0;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::accept()
{
    if(ui->lineNumber->text().isEmpty())
    {
        QMessageBox::warning(this, "No number", "You must set a number");
        idiotCounter++;
        return;
    }
    bool notUnique = false;
    QSqlQuery query;
    query.prepare("SELECT number FROM fightercraft");
    query.exec();
    while(query.next())
    {
        if(query.value(0).toString() == ui->lineNumber->text() && prevNumber != ui->lineNumber->text())
        {
            notUnique = true;
        }
    }
    if(notUnique)
    {
        QMessageBox::warning(this, "Number is not unique", "You must set a unique number");
        idiotCounter++;
        return;
    }
    dataList << ui->lineNumber->text() << ui->linePilot->text();
    dataList << ui->lineCopilot->text() << ui->lineModel->text();
    dataList << ui->lineMothership->text();
    if(edit)
    {
        query.prepare("UPDATE fightercraft SET number=:num, pilot=:p, copilot=:cp, model=:m, mothership_homebase=:mh WHERE number=" + prevNumber);
    }
    else
    {
        query.prepare("INSERT INTO fightercraft VALUES(:num, :p, :cp, :m, :mh)");
    }
    query.bindValue(":num", dataList.at(0));
    query.bindValue(":p", dataList.at(1));
    query.bindValue(":cp", dataList.at(2));
    query.bindValue(":m", dataList.at(3));
    query.bindValue(":mh", dataList.at(4));
    if(!query.exec())
    {
        if(edit)
        {
            QMessageBox::warning(this, "ERROR 404", "Could not update data" + query.lastError().text());
        }
        else
        {
        QMessageBox::warning(this, "ERROR 202", "Could not insert data" + query.lastError().text());
        }
        return;
    }
    QDialog::accept();
}

int Dialog::getIdiotCounter()
{
    return idiotCounter;
}

void Dialog::onButtonSelectPilot()
{
    QStringList headers;
    headers << "dogtag" << "name" << "surname" << "race" << "birth_date" << "rank";
    QString tableName = "pilot";
    TableDialog d(db, tableName, headers);
    if(d.exec() == QDialog::Accepted)
    {
        ui->linePilot->setText(d.getData());
        idiotCounter += d.getIdiotCounter();
    }
}

void Dialog::onButtonSelectCopilot()
{
    QStringList headers;
    headers << "dogtag" << "name" << "surname" << "race" << "birth_date" << "rank";
    QString tableName = "pilot";
    TableDialog d(db, tableName, headers);
    if(d.exec() == QDialog::Accepted)
    {
        ui->lineCopilot->setText(d.getData());
        idiotCounter += d.getIdiotCounter();
    }
}

void Dialog::onButtonSelectModel()
{
    QStringList headers;
    headers << "name" << "type" << "class" << "powercore" << "fuel" << "high_atmo_engines" << "low_atmo_engines" << "ftl_drive" << "primary_weapon" << "secondary_weapon";
    QString tableName = "models";
    TableDialog d(db, tableName, headers);
    if(d.exec() == QDialog::Accepted)
    {
        ui->lineModel->setText(d.getData());
        idiotCounter += d.getIdiotCounter();
    }
}
