#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nodes.h"
#include "expressionbuilder.h"
#include <string>
#include <QtDebug>
#include <stack>
#include <queue>
#include <ostream>
#include <sstream>
#include <QTableView>
#include <QItemDelegate>
#include <QStandardItemModel>
#include <QFile>

using namespace std;

template <class T>
QString toString(const T& t)
{
    std::ostringstream oss;
    oss << t;
    return QString::fromStdString(oss.str());
}

string delSpaces(string &str)
{
   str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
   return str;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QTreeWidgetItem* convertTree(QTreeWidget* tree, ExpressionElementNode* root, QTreeWidgetItem *parent) {
    if(root->left != 0 && root->right != 0) {
        QTreeWidgetItem *left = new QTreeWidgetItem(), *right = new QTreeWidgetItem();
        left = convertTree(tree, root->left, left);
        right = convertTree(tree, root->right, right);

        parent->setText(0, toString(root->binary_op));
        parent->addChild(left);
        parent->addChild(right);
    } else if(root->left == 0 && root->right == 0) {
        if(root->variable != NULL) {
            parent->setText(0, toString(root->variable));
        } else {
            parent->setText(0, toString(root->number));
        }
    } else if(root->left == 0) {
        QTreeWidgetItem *right = new QTreeWidgetItem();
        right = convertTree(tree, root->right, right);

        parent->setText(0, toString(root->binary_op));
        parent->addChild(right);
    } else if(root->right == 0) {
        QTreeWidgetItem *left = new QTreeWidgetItem();
        left = convertTree(tree, root->left, left);

        parent->setText(0, toString(root->binary_op));
        parent->addChild(left);
    }
    return parent;
}

void MainWindow::updateTree(string exp) {
    exp = delSpaces(exp);
    BinaryExpressionBuilder b;
    ExpressionElementNode* root;
    root = b.parse(exp);
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->onpTree);

    item = convertTree(ui->onpTree, root, item);

    ui->onpTree->addTopLevelItem(item);
    ui->onpTree->update();
}


void MainWindow::on_calc_clicked()
{
    try
    {
        ui->onpTree->clear();
        updateTree(ui->function->text().toStdString());
        ui->onpTree->expandAll();
    }
    catch(BinaryExpressionBuilder::NotWellFormed obj)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error",obj.what());
        messageBox.setFixedSize(500,200);
    }
}

void MainWindow::on_drawPlot_clicked()
{
    try {
        double from = ui->from->value();
        double to = ui->to->value();
        double step =  ui->step->value();

        if(from > to) {
            double temp = to;
            to = from;
            from = temp;

            ui->from->setValue(from);
            ui->to->setValue(to);
        }

        if(step == 0) {
            step = 1;
            ui->step->setValue(1.0);
        }

        double itemsD = (to-from) / step;
        int items = (int)itemsD;

        string exp = ui->function->text().toStdString();
        exp = delSpaces(exp);
        BinaryExpressionBuilder b;
        ExpressionElementNode* root;
        root = b.parse(exp);

        QStandardItemModel *model = new QStandardItemModel();

        model = new QStandardItemModel(items,2, ui->table);
        model->setHeaderData(0, Qt::Horizontal, tr("x"));
        model->setHeaderData(1, Qt::Horizontal, tr("f(x)"));

        double x = from;
        QVector<double> key(items), values(items);

        for(int row = 0; row < items; row++)
        {
            double value = b.calculate(root, x);
            QModelIndex indexKey = model->index(row,0,QModelIndex());
            model->setData(indexKey,x);

            QModelIndex indexValue = model->index(row,1,QModelIndex());
            model->setData(indexValue, value);

            key[row] = x;
            values[row] = value;

            x += step;
        }

        ui->plot->addGraph();
        ui->plot->graph(0)->setData(key, values);
        ui->plot->xAxis->setLabel("x");
        ui->plot->yAxis->setLabel("f(x)");
        ui->plot->graph(0)->rescaleAxes();
        ui->plot->replot();

        ui->table->setModel(model);

        ui->table->update();
    }
    catch(BinaryExpressionBuilder::NotWellFormed obj)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error",obj.what());
        messageBox.setFixedSize(500,200);
    }

}

void MainWindow::on_close_clicked()
{
    QApplication::quit();
}

void MainWindow::on_save_clicked()
{
    QString filename = QFileDialog::getSaveFileName( ui->centralWidget, "Zapisz plik", "", "" );
    QFile f( filename );
    f.open( QIODevice::WriteOnly );
    QTextStream outStream(&f);
    outStream << ui->function->text();
    f.close();
}

void MainWindow::on_Wczytaj_clicked()
{
    QString filename = QFileDialog::getOpenFileName( ui->centralWidget, "Wczytaj plik", "", "" );
    QFile f( filename );
    f.open( QIODevice::ReadOnly );
    QTextStream inStream(&f);
    QString s;
    s = inStream.readAll();
    ui->function->setText(s);
    f.close();

    ui->calc->click();
    ui->drawPlot->click();
}
