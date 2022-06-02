#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "myrobot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_Connexion_clicked();

    void on_Avancer_pressed();

    void on_Avancer_released();

    void on_Gauche_pressed();

    void on_Gauche_released();

    void on_Droite_pressed();

    void on_Droite_released();

    void on_Reculer_pressed();

    void on_Reculer_released();

private:
    Ui::MainWindow *ui;
    MyRobot myRobot;
};
#endif // MAINWINDOW_H
