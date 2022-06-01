#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    myRobot.disConnect();
    delete ui;
}


void MainWindow::on_b_connexion_clicked()
{
    myRobot.doConnect();
    myRobot.move();
}
