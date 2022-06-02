#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    camera = new Camera(this);
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


void MainWindow::on_Niv_Batterie_overflow()
{

}
