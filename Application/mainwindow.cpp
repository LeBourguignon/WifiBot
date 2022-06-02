#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet("background-color: rgb(200,200,200)");
}

MainWindow::~MainWindow()
{
    if (myRobot.isConnect())
        myRobot.disConnect();
    delete ui;
}


void MainWindow::on_Connexion_clicked()
{
    if (myRobot.isConnect()) {
        myRobot.disConnect();
        ui->Connexion->setText("Déconnecté");
    }
    else {
        if (myRobot.doConnect())
            ui->Connexion->setText("Connecté");
    }
}


void MainWindow::on_Avancer_pressed()
{
    myRobot.move(Direction::FORWARD, 0x7F);
}

void MainWindow::on_Avancer_released()
{
    myRobot.move();
}


void MainWindow::on_Gauche_pressed()
{
    myRobot.move(Direction::LEFT, 0x7F);
}

void MainWindow::on_Gauche_released()
{
    myRobot.move();
}


void MainWindow::on_Droite_pressed()
{
    myRobot.move(Direction::RIGHT, 0x7F);
}

void MainWindow::on_Droite_released()
{
    myRobot.move();
}


void MainWindow::on_Reculer_pressed()
{
    myRobot.move(Direction::BACKWARD, 0x7F);
}

void MainWindow::on_Reculer_released()
{
    myRobot.move();
}
