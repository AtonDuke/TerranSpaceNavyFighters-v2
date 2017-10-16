#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAbstractItemModel>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QItemSelectionModel>
#include <QStringList>
#include "dialog.h"
#include <QSqlDatabase>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lcdIdiotCounter->hide();
    connect(ui->buttonAdd_1,SIGNAL(clicked(bool)),this,SLOT(onButtonAdd_1()));
    connect(ui->buttonAdd_2,SIGNAL(clicked(bool)),this,SLOT(onButtonAdd_2()));
    connect(ui->buttonAdd_3,SIGNAL(clicked(bool)),this,SLOT(onButtonAdd_3()));
    connect(ui->buttonDel_1,SIGNAL(clicked(bool)),this,SLOT(onButtonDel_1()));
    connect(ui->buttonDel_2,SIGNAL(clicked(bool)),this,SLOT(onButtonDel_2()));
    connect(ui->buttonDel_3,SIGNAL(clicked(bool)),this,SLOT(onButtonDel_3()));
    connect(ui->buttonEdit_1,SIGNAL(clicked(bool)),this,SLOT(onButtonEdit_1()));
    connect(ui->buttonViewPilot,SIGNAL(clicked(bool)),this,SLOT(onButtonViewPilot()));
    connect(ui->buttonViewCopilot,SIGNAL(clicked(bool)),this,SLOT(onButtonViewCopilot()));
    connect(ui->buttonViewModel,SIGNAL(clicked(bool)),this,SLOT(onButtonViewModel()));
    connect(ui->buttonViewAll_1,SIGNAL(clicked(bool)),this,SLOT(onButtonViewAll_1()));
    connect(ui->buttonViewAll_2,SIGNAL(clicked(bool)),this,SLOT(onButtonViewAll_2()));
    connect(ui->buttonIdiotCounter, SIGNAL(clicked(bool)), this, SLOT(onButtonIdiotCounter()));

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../TerranSpaceNavyFighters/db.sqlite");
    if(!db.open())
    {
        QMessageBox::critical(this, "ERROR 101", "Could not open database");
    }

    ui->tabWidget->setCurrentIndex(0);

    model1 = new QSqlTableModel(this, db);
    model1->setTable("fightercraft");
    model1->select();
    model1->setHeaderData(0, Qt::Horizontal, "number");
    model1->setHeaderData(1, Qt::Horizontal, "pilot");
    model1->setHeaderData(2, Qt::Horizontal, "copilot");
    model1->setHeaderData(3, Qt::Horizontal, "model");
    model1->setHeaderData(4, Qt::Horizontal, "mothership_homebase");
    ui->tableView_1->setModel(model1);
    ui->tableView_1->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_1->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_1->setEditTriggers(QAbstractItemView::NoEditTriggers);

    model2 = new QSqlTableModel(this, db);
    model2->setTable("pilot");
    model2->select();
    model2->setHeaderData(0, Qt::Horizontal, "dogtag");
    model2->setHeaderData(1, Qt::Horizontal, "name");
    model2->setHeaderData(2, Qt::Horizontal, "surname");
    model2->setHeaderData(3, Qt::Horizontal, "race");
    model2->setHeaderData(4, Qt::Horizontal, "birth_date");
    model2->setHeaderData(5, Qt::Horizontal, "rank");
    ui->tableView_2->setModel(model2);
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_2->setSelectionMode(QAbstractItemView::SingleSelection);

    model3 = new QSqlTableModel(this, db);
    model3->setTable("models");
    model3->select();
    model3->setHeaderData(0, Qt::Horizontal, "name");
    model3->setHeaderData(1, Qt::Horizontal, "type");
    model3->setHeaderData(2, Qt::Horizontal, "class");
    model3->setHeaderData(3, Qt::Horizontal, "powercore");
    model3->setHeaderData(4, Qt::Horizontal, "fuel");
    model3->setHeaderData(5, Qt::Horizontal, "high_atmo_engines");
    model3->setHeaderData(6, Qt::Horizontal, "low_atmo_engines");
    model3->setHeaderData(7, Qt::Horizontal, "ftl_drive");
    model3->setHeaderData(8, Qt::Horizontal, "primary_weapon");
    model3->setHeaderData(9, Qt::Horizontal, "secondary_weapon");
    ui->tableView_3->setModel(model3);
    ui->tableView_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_3->setSelectionMode(QAbstractItemView::SingleSelection);

    idiotCounter = 0;
    ui->lcdIdiotCounter->display(idiotCounter);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(decreaseIdiotCounter()));
    timer->start(60000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onButtonAdd_1()
{
    QStringList list;
    Dialog d(db, list);
    if(d.exec() == QDialog::Accepted)
    {
        idiotCounter += d.getIdiotCounter();
        ui->lcdIdiotCounter->display(idiotCounter);
    }
    model1->select();
}

void MainWindow::onButtonAdd_2()
{
    if(ui->lineDogtag->text().isEmpty())
    {
        QMessageBox::warning(this, "No dogtag", "You must set a dogtag");
        idiotCounter++;
        ui->lcdIdiotCounter->display(idiotCounter);
        idiotWarning();
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT dogtag FROM pilot");
    bool notUnique = false;
    query.exec();
    while(query.next())
    {
        if(query.value(0).toString() == ui->lineDogtag->text())
        {
            notUnique = true;
        }
    }
    if(notUnique)
    {
        QMessageBox::warning(this, "Dogtag is not unique", "You must set a unique dogtag");
        idiotCounter++;
        ui->lcdIdiotCounter->display(idiotCounter);
        idiotWarning();
        return;
    }
    if(ui->lineName->text().isEmpty())
    {
        QMessageBox::warning(this, "No name", "You must set a name");
        idiotCounter++;
        ui->lcdIdiotCounter->display(idiotCounter);
        return;
    }
    if(ui->lineSurname->text().isEmpty())
    {
        QMessageBox::warning(this, "No surname", "You must set a surname");
        idiotCounter++;
        ui->lcdIdiotCounter->display(idiotCounter);
        return;
    }
    query.prepare("INSERT INTO pilot VALUES(:dogtag , :name, :surname, :race, :birth_date, :rank)");
    query.bindValue(":dogtag", ui->lineDogtag->text());
    query.bindValue(":name", ui->lineName->text());
    query.bindValue(":surname", ui->lineSurname->text());
    query.bindValue(":race", ui->lineRace->text());
    query.bindValue(":birth_date", ui->dateEdit->date().toString("yyyy/MM/dd"));
    query.bindValue(":rank", ui->lineRank->text());
    if(!query.exec())
    {
        QMessageBox::warning(this, "ERROR 202", "Could not insert data" + query.lastError().text());
    }
    model2->select();
}

void MainWindow::onButtonAdd_3()
{
    if(ui->lineModelName->text().isEmpty())
    {
        QMessageBox::warning(this, "No name", "You must set a name");
        idiotCounter++;
        ui->lcdIdiotCounter->display(idiotCounter);
        idiotWarning();
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT name FROM models");
    bool notUnique = false;
    query.exec();
    while(query.next())
    {
        if(query.value(0).toString() == ui->lineModelName->text())
        {
            notUnique = true;
        }
    }
    if(notUnique)
    {
        QMessageBox::warning(this, "Name is not unique", "You must set a unique name");
        idiotCounter++;
        ui->lcdIdiotCounter->display(idiotCounter);
        idiotWarning();
        return;
    }
    query.prepare("INSERT INTO models VALUES(:name, :type, :class, :powercore, :fuel, :high_atmo_engines, :low_atmo_engines, :ftl_drive, :primary_weapon, :secondary_weapon)");
    query.bindValue(":name", ui->lineModelName->text());
    query.bindValue(":type", ui->lineType->text());
    query.bindValue(":class", ui->lineClass->text());
    query.bindValue(":powercore", ui->linePowercore->text());
    query.bindValue(":fuel", ui->lineFuel->text());
    query.bindValue(":high_atmo_engines", ui->lineHighAtmoEngines->text());
    query.bindValue(":low_atmo_engines", ui->lineLowAtmoEngines->text());
    query.bindValue(":ftl_drive", ui->lineFTLDrive->text());
    query.bindValue(":primary_weapon", ui->linePrimaryWeapon->text());
    query.bindValue(":secondary_weapon", ui->lineSecondaryWeapon->text());
    if(!query.exec())
    {
        QMessageBox::warning(this, "ERROR 202", "Could not insert data" + query.lastError().text());
    }
    model3->select();
}

void MainWindow::onButtonDel_1()
{
    switch(QMessageBox::question(this, "Delete records?", "Do you really want to delete these records?"))
    {
    case QMessageBox::Yes:
    {
        QString sql = QString("DELETE FROM fightercraft WHERE number = '");
        QItemSelectionModel *selected = ui->tableView_1->selectionModel();
        if(selected->selectedRows(0).isEmpty())
        {
            return;
        }
        sql += selected->selectedRows(0).at(0).data().toString();
        sql += "'";
        QSqlQuery query;
        query.prepare(sql);
        if(!query.exec())
        {
            QMessageBox::warning(this, "ERROR 303", "Could not delete data" + query.lastError().text());
        }
        break;
    }
    case QMessageBox::No:
        return;
        break;
    default:
        return;
    }
    model1->select();
}

void MainWindow::onButtonDel_2()
{
    switch(QMessageBox::question(this, "Delete records?", "Do you really want to delete these records?"))
    {
    case QMessageBox::Yes:
    {
        QString sql = QString("DELETE FROM pilot WHERE dogtag = '");
        QItemSelectionModel *selected = ui->tableView_2->selectionModel();
        if(selected->selectedRows(0).isEmpty())
        {
            return;
        }
        sql += selected->selectedRows(0).at(0).data().toString();
        sql += "'";
        QSqlQuery query;
        query.prepare(sql);
        if(!query.exec())
        {
            QMessageBox::warning(this, "ERROR 303", "Could not delete data" + query.lastError().text());
        }
        model2->select();
        break;
    }
    case QMessageBox::No:
        return;
        break;
    default:
        return;
    }
}

void MainWindow::onButtonDel_3()
{
    switch(QMessageBox::question(this, "Delete records?", "Do you really want to delete these records?"))
    {
    case QMessageBox::Yes:
    {
        QString sql = QString("DELETE FROM models WHERE name = '");
        QItemSelectionModel *selected = ui->tableView_3->selectionModel();
        if(selected->selectedRows(0).isEmpty())
        {
            return;
        }
        sql += selected->selectedRows(0).at(0).data().toString();
        sql += "'";
        QSqlQuery query;
        query.prepare(sql);
        if(!query.exec())
        {
            QMessageBox::warning(this, "ERROR 303", "Could not delete data" + query.lastError().text());
        }
        model3->select();
        break;
    }
    case QMessageBox::No:
        return;
        break;
    default:
        return;
    }
}

void MainWindow::onButtonEdit_1()
{
    QStringList dataList;
    QItemSelectionModel *selected = ui->tableView_1->selectionModel();
    if(selected->selectedRows(0).isEmpty())
    {
        QMessageBox::warning(this, "No row selected", "You must select a row first");
        idiotCounter++;
        ui->lcdIdiotCounter->display(idiotCounter);
        idiotWarning();
        return;
    }
    for(int i = 0; i < 5; ++i)
    {
        dataList << selected->selectedRows(i).at(0).data().toString();
    }
    Dialog dialog(db, dataList);
    if(dialog.exec() == QDialog::Accepted)
    {
        idiotCounter += dialog.getIdiotCounter();
        ui->lcdIdiotCounter->display(idiotCounter);
    }
    model1->select();
}

void MainWindow::onButtonViewPilot()
{
    QItemSelectionModel *selected = ui->tableView_1->selectionModel();
    QSqlQuery query;
    if(selected->selectedRows(0).isEmpty())
    {
        QMessageBox::warning(this, "No row selected", "You must select a row first");
        idiotCounter++;
        ui->lcdIdiotCounter->display(idiotCounter);
        idiotWarning();
        return;
    }
    query.prepare("SELECT pilot FROM fightercraft WHERE number='" + selected->selectedRows(0).at(0).data().toString() + "'");
    query.exec();
    query.first();
    model2->setFilter("dogtag='" + query.value(0).toString() + "'");
    ui->labelFilter_1->setText("Filter set to: " + query.value(0).toString());
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::onButtonViewCopilot()
{
    QItemSelectionModel *selected = ui->tableView_1->selectionModel();
    QSqlQuery query;
    if(selected->selectedRows(0).isEmpty())
    {
        QMessageBox::warning(this, "No row selected", "You must select a row first");
        idiotCounter++;
        ui->lcdIdiotCounter->display(idiotCounter);
        idiotWarning();
        return;
    }
    query.prepare("SELECT copilot FROM fightercraft WHERE number='" + selected->selectedRows(0).at(0).data().toString() + "'");
    query.exec();
    query.first();
    model2->setFilter("dogtag='" + query.value(0).toString() + "'");
    ui->labelFilter_1->setText("Filter set to: " + query.value(0).toString());
    ui->tabWidget->setCurrentIndex(1);
}
void MainWindow::onButtonViewModel()
{
    QItemSelectionModel *selected = ui->tableView_1->selectionModel();
    QSqlQuery query;
    if(selected->selectedRows(0).isEmpty())
    {
        QMessageBox::warning(this, "No row selected", "You must select a row first");
        idiotCounter++;
        ui->lcdIdiotCounter->display(idiotCounter);
        idiotWarning();
        return;
    }
    query.prepare("SELECT model FROM fightercraft WHERE number='" + selected->selectedRows(0).at(0).data().toString() + "'");
    query.exec();
    query.first();
    model3->setFilter("name='" + query.value(0).toString() + "'");
    ui->labelFilter_2->setText("Filter set to: " + query.value(0).toString());
    ui->tabWidget->setCurrentIndex(2);
}

void MainWindow::onButtonViewAll_1()
{
    model2->setFilter("");
    ui->labelFilter_1->clear();
}

void MainWindow::onButtonViewAll_2()
{
    model3->setFilter("");
    ui->labelFilter_2->clear();
}

void MainWindow::decreaseIdiotCounter()
{
    if(idiotCounter > 0)
    {
        idiotCounter--;
    }
    ui->lcdIdiotCounter->display(idiotCounter);
}

void MainWindow::onButtonIdiotCounter()
{
    ui->lcdIdiotCounter->setHidden(!ui->lcdIdiotCounter->isHidden());
}

void MainWindow::idiotWarning()
{
    if(idiotCounter >= 10 && idiotCounter < 20)
    {
        QMessageBox::warning(this, "PEBKAC", "Problem Exists Between Keyboard And Chair");
    }
    else if(idiotCounter >= 20 && idiotCounter < 30)
    {
        QMessageBox::warning(this, "ID10T", "ID10T");
    }
    else if(idiotCounter >= 30)
    {
        QMessageBox::critical(this, "Fuck you!", "Fuck you, idiot! \n I quit!");
        QApplication::quit();
    }
}
