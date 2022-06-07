#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    camera = new Camera(this);
    setStyleSheet("background-color: rgb(200,200,200)");

    connect(&myRobot, SIGNAL(updateUI(QByteArray)), this, SLOT(updateWindow(QByteArray)));
}

MainWindow::~MainWindow()
{
    if (myRobot.isConnect())
        myRobot.disConnect();
    delete ui;
}

void MainWindow::updateWindow(QByteArray data) {
    updateBattery(data);
    updateSpeed(data);

    unsigned char irLF = (unsigned char)data[3];
    unsigned char irLB = (unsigned char)data[4];

    unsigned char irRF = (unsigned char)data[11];
    unsigned char irRB = (unsigned char)data[12];
}

void MainWindow::updateBattery(QByteArray data) {
    unsigned char battery = (unsigned char)data[2];
    if(battery > 170) {
        ui->Niv_Batterie->display(200);
    }
    else {
        ui->Niv_Batterie->display((battery * 100 / 123));
    }
}

void MainWindow::updateSpeed(QByteArray data) {
    long odometryL = ((long)data[8]<<24)+((long)data[7]<<16)+((long)data[6]<<8)+((long)data[5]);
    long odometryR = ((long)data[16]<<24)+((long)data[15]<<16)+((long)data[14]<<8)+((long)data[13]);

    double speedL;
    if (odometryLBefore<odometryL) {
        speedL = (odometryL - odometryLBefore) * 3.14 * 0.135 / 0.025;
        odometryLBefore = odometryL;
    }
    else {
        speedL = (0xFFFFFFFF-odometryLBefore + odometryL) * 3.14 * 0.135 / 0.025;
        odometryLBefore = odometryL;
    }

    double speedR;
    if (odometryRBefore<odometryR) {
        speedR = (odometryR - odometryRBefore) * 3.14 * 0.135 / 0.025;
        odometryRBefore = odometryR;
    }
    else {
        speedR = (0xFFFFFFFF-odometryRBefore + odometryR) * 3.14 * 0.135 / 0.025;
        odometryRBefore = odometryR;
    }

    double speed = (speedL + speedR) / 2;
    ui->Vitesse_Roue->display((int)speed);
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


void MainWindow::on_Camera_Haut_clicked()
{
    camera->move(0);
}


void MainWindow::on_Camera_Droite_clicked()
{
    camera->move(2);
}


void MainWindow::on_Camera_Bas_clicked()
{
    camera->move(3);
}


void MainWindow::on_Camera_Gauche_clicked()
{
    camera->move(1);
}
