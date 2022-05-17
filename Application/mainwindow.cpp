#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->radioButton->setText("");

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_radioButton_clicked(bool checked)
{
   //avec ce bouton on paramètre le fait d'aller tout droit
   //c'est à dire on fait en sorte que la vitesse de la roue droite soit la même que la roue gauche
}

