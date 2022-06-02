#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "camera.h"
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
    
    void on_Camera_Haut_clicked();

    void on_Camera_Droite_clicked();

    void on_Camera_Bas_clicked();

    void on_Camera_Gauche_clicked();

private:
    Ui::MainWindow *ui;
    Camera *camera;
    MyRobot myRobot;
};
#endif // MAINWINDOW_H
