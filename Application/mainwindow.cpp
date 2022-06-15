#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGamepadManager>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    camera = new Camera(this);
    setStyleSheet("background-color: rgb(200,200,200)");
    GamepadPressButton();
    GamepadReleaseButton();
    GamepadLeftAxis();
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

void MainWindow::keyPressEvent(QKeyEvent *event){
    int touche = event->key();
    switch(touche){

            case Qt::Key_Z:
            myRobot.move(Direction::FORWARD, 0x7F);
            break;

            case Qt::Key_Q:
            myRobot.move(Direction::LEFT, 0x7F);
            break;

            case Qt::Key_D:
            myRobot.move(Direction::RIGHT, 0x7F);
            break;

            case Qt::Key_S:
            myRobot.move(Direction::BACKWARD, 0x7F);
            break;

            case Qt::Key_I:
            camera->move(0);
            break;

            case Qt::Key_J:
            camera->move(1);
            break;

            case Qt::Key_L:
            camera->move(2);
            break;

            case Qt::Key_K:
            camera->move(3);
            break;

     }

}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
    int touche = event->key();
    switch(touche){

            case Qt::Key_Z:
            case Qt::Key_Q:
            case Qt::Key_D:
            case Qt::Key_S:
            myRobot.move();
            break;

     }

}


void MainWindow::on_Camera_Haut_clicked()
{
    camera->move(0);
}

void MainWindow::on_Camera_Gauche_clicked()
{
    camera->move(1);
}

void MainWindow::on_Camera_Droite_clicked()
{
    camera->move(2);
}


void MainWindow::on_Camera_Bas_clicked()
{
    camera->move(3);
}


void MainWindow::GamepadPressButton(){
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonPressEvent, this,
            [this](int deviceId, QGamepadManager::GamepadButton button, double value){
        switch(button){
            case QGamepadManager::GamepadButton::ButtonY:
            camera->move(0);
            break;
            case QGamepadManager::GamepadButton::ButtonX:
            camera->move(1);
            break;
            case QGamepadManager::GamepadButton::ButtonB:
            camera->move(2);
            break;
            case QGamepadManager::GamepadButton::ButtonA:
            camera->move(3);
            break;
            case QGamepadManager::GamepadButton::ButtonUp:
            myRobot.move(Direction::FORWARD, 0x7F);
            break;
            case QGamepadManager::GamepadButton::ButtonLeft:
            myRobot.move(Direction::LEFT, 0x7F);
            break;
            case QGamepadManager::GamepadButton::ButtonRight:
            myRobot.move(Direction::RIGHT, 0x7F);
            break;
            case QGamepadManager::GamepadButton::ButtonDown:
            myRobot.move(Direction::BACKWARD, 0x7F);
            break;
            default:
            break;
        }
    });
}

void MainWindow::GamepadReleaseButton(){
connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonReleaseEvent, this,
        [this](int deviceId, QGamepadManager::GamepadButton button, double value){
        switch(button){

        case QGamepadManager::GamepadButton::ButtonUp:
        case QGamepadManager::GamepadButton::ButtonLeft:
        case QGamepadManager::GamepadButton::ButtonRight:
        case QGamepadManager::GamepadButton::ButtonDown:
        myRobot.move();
        break;
        default:
        break;

        }

});
}

void MainWindow::GamepadLeftAxis(){
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadAxisEvent, this,
            [this](int deviceId, QGamepadManager::GamepadAxis button, double value){
       qDebug() << value;
       qDebug() << button;
        switch(button){
            case QGamepadManager::GamepadAxis::AxisLeftX:
            camera->move(2);
            break;
            default:
            break;
        }
    });
}
