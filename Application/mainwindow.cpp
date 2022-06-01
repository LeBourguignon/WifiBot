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
    delete ui;
}



void MainWindow::on_Connexion_clicked()
{

    ui->Connexion->setText("Deconnexion");

}


void MainWindow::on_Reculer_clicked()
{

}


void MainWindow::on_Gauche_clicked()
{

}


void MainWindow::on_Avancer_clicked()
{

}


void MainWindow::on_Droite_clicked()
{

}


void MainWindow::on_Camera_Haut_clicked()
{

}


void MainWindow::on_Camera_Droite_clicked()
{

}


void MainWindow::on_Camera_Bas_clicked()
{

}


void MainWindow::on_Camera_Gauche_clicked()
{

}


void MainWindow::on_Niv_Batterie_overflow()
{

}


void MainWindow::on_Distance_Infra_overflow()
{

}

void MainWindow::on_Vitesse_Roue_overflow()
{

}

