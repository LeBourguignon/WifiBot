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
    updateCaptor(data);

    if (myRobot.isConnect())
        ui->Connexion->setText("Connecté");
    else
        ui->Connexion->setText("Déconnecté");
}

void MainWindow::updateBattery(QByteArray data) {
    unsigned char battery = (unsigned char)data[2];
    if(battery > 170) {
        ui->battery->setText("En charge");
    }
    else {
        ui->battery->setText(QString::number(battery * 100 / 123) + " %");
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

    double speed = (speedL + speedR) / 20000;


    if (speed > 1000000)
        speed = 0;

    QString textSpeed = QString::number(speed) + " m/s";
    ui->speed->setText(textSpeed);
    //qDebug() << "Speed: " << speed;
}

void MainWindow::updateCaptor(QByteArray data) {
    unsigned char irRF = (unsigned char)data[3];
    //unsigned char irRB = (unsigned char)data[4];

    unsigned char irLF = (unsigned char)data[11];
    unsigned char irLB = (unsigned char)data[12];

    //qDebug() << "Captor: " << irLF << "; " << irRF << "; " << irLB << "; " << irRB;

    if (irLF >= 180) {
        ui->sensorFL->setValue(100);
    }
    else {
        ui->sensorFL->setValue(irLF * 100 / 180);
    }

    if (irRF >= 180) {
        ui->sensorFR->setValue(100);
    }
    else {
        ui->sensorFR->setValue(irRF * 100 / 180);
    }

    if (irLB >= 180) {
        ui->sensorBL->setValue(100);
    }
    else {
        ui->sensorBL->setValue(irLB * 100 / 180);
    }

    /* Capteur de marche pas
    if (irLB >= 180) {

    }
    else {

    }
    */
}

void MainWindow::on_Connexion_clicked()
{
    if (myRobot.isConnect()) {
        myRobot.disConnect();
    }
    else {
        myRobot.doConnect();
    }
}


void MainWindow::on_Avancer_pressed()
{
    myRobot.move(Direction::FORWARD, ui->setSpeed->value());
}

void MainWindow::on_Avancer_released()
{
    myRobot.move();
}


void MainWindow::on_Gauche_pressed()
{
    myRobot.move(Direction::LEFT, ui->setSpeed->value());
}

void MainWindow::on_Gauche_released()
{
    myRobot.move();
}


void MainWindow::on_Droite_pressed()
{
    myRobot.move(Direction::RIGHT, ui->setSpeed->value());
}

void MainWindow::on_Droite_released()
{
    myRobot.move();
}


void MainWindow::on_Reculer_pressed()
{
    myRobot.move(Direction::BACKWARD, ui->setSpeed->value());
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
        [this](int deviceId, QGamepadManager::GamepadButton button){
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

Direction MainWindow::toDirection(double x, double y) {
    if((x*x)+(y*y) <= (0.1*0.1))
        return Direction::NONE;
    else if ((-2*x)>=y && (2*x)>=y)
        return Direction::FORWARD;
    else if ((2*x)<=y && (0.5*x)>=y)
        return Direction::FORWARD_RIGHT;
    else if ((0.5*x)<=y && (-0.5*x)>=y)
        return Direction::RIGHT;
    else if ((-0.5*x)<=y && (-2*x)>=y)
        return Direction::BACKWARD_RIGHT;
    else if ((-2*x)<=y && (2*x)<=y)
        return Direction::BACKWARD;
    else if ((2*x)>=y && (0.5*x)<=y)
        return Direction::BACKWARD_LEFT;
    else if ((0.5*x)>=y && (-0.5*x)<=y)
        return Direction::LEFT;
    else if ((-0.5*x)>=y && (-2*x)<=y)
        return Direction::FORWARD_LEFT;
    else return Direction::NONE;
}
