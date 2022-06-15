#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>


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
    void updateWindow(QByteArray data);

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

protected:
    void MainWindow::keyPressEvent(QKeyEvent *event);
    void MainWindow::keyReleaseEvent(QKeyEvent *event);
    void MainWindow::GamepadPressButton();
    void MainWindow::GamepadReleaseButton();
    void MainWindow::GamepadLeftAxis();

private:
    Ui::MainWindow *ui;
    Camera *camera;
    MyRobot myRobot;

    void updateBattery(QByteArray data);
    void updateSpeed(QByteArray data);
    void updateCaptor(QByteArray data);

    long odometryLBefore = 0;
    long odometryRBefore = 0;

    Direction toDirection(double x, double y);
};
#endif // MAINWINDOW_H
