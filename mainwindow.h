#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include<iostream>
#include<stack>
#include<string>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void updateTree(string str);
    ~MainWindow();

private slots:
    void on_calc_clicked();
    void on_drawPlot_clicked();

    void on_close_clicked();

    void on_save_clicked();

    void on_Wczytaj_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
