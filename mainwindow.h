#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSqlDatabase db;
public slots:
    void onButtonAdd_1();
    void onButtonAdd_2();
    void onButtonAdd_3();
    void onButtonDel_1();
    void onButtonDel_2();
    void onButtonDel_3();
    void onButtonEdit_1();
    void onButtonViewPilot();
    void onButtonViewCopilot();
    void onButtonViewModel();
    void onButtonViewAll_1();
    void onButtonViewAll_2();
    void decreaseIdiotCounter();
    void onButtonIdiotCounter();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model1;
    QSqlTableModel *model2;
    QSqlTableModel *model3;
    int idiotCounter;
    void idiotWarning();
};

#endif // MAINWINDOW_H
