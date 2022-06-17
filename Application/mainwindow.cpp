#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGamepadManager>
#include <QThread>

//Constructeur
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
      
    camera = new Camera(this);
    setStyleSheet("background-color: rgb(200,200,200)");
    
    GamepadPressButton();
    GamepadReleaseButton();
    GamepadAxis();
    
    connect(&myRobot, SIGNAL(updateUI(QByteArray)), this, SLOT(updateWindow(QByteArray)));
}

//Destructeur
MainWindow::~MainWindow()
{
    if (myRobot.isConnect())
        myRobot.disConnect();
    delete ui;
}

//Mise à jour de la fenetre
void MainWindow::updateWindow(QByteArray data) {
    updateBattery(data);
    updateSpeed(data);
    updateCaptor(data);

    /*
    if (myRobot.isConnect())
        ui->Connexion->setText("Connecté");
    else
        ui->Connexion->setText("Déconnecté");
    */
}

//Mise à jour de la batterie
void MainWindow::updateBattery(QByteArray data) {
    unsigned char battery = (unsigned char)data[2];
    if(battery > 170) {
        ui->battery->setText("En charge");
    }
    else {
        ui->battery->setText(QString::number(battery * 100 / 123) + " %");
    }
}

//Mise à jour de la vitesse du robot
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

//Mise à jour des capteur d'obstacle
void MainWindow::updateCaptor(QByteArray data) {
    unsigned char irLF = (unsigned char)data[3];
    unsigned char irLB = (unsigned char)data[4];

    unsigned char irRF = (unsigned char)data[11];
    unsigned char irRB = (unsigned char)data[12];

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

    if (irRB >= 180) {
        ui->sensorBR->setValue(100);
    }
    else {
        ui->sensorBR->setValue(irRB * 100 / 180);
    }


    /*
    if (ui->sensorBL->value() == 100 || ui->sensorBR->value() == 100 || ui->sensorFL->value() == 100 || ui->sensorFL->value() == 100 || ui->sensorFR->value() == 100) {
        // Create a Vibraton State
        XINPUT_VIBRATION Vibration;

        // Zeroise the Vibration
        ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

        // Set the Vibration Values
        Vibration.wLeftMotorSpeed = 1;
        Vibration.wRightMotorSpeed = 1;

        // Vibrate the controller
        XInputSetState(0, &Vibration);
    }
    else {
        // Create a Vibraton State
        XINPUT_VIBRATION Vibration;

        // Zeroise the Vibration
        ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

        // Set the Vibration Values
        Vibration.wLeftMotorSpeed = 0;
        Vibration.wRightMotorSpeed = 0;

        // Vibrate the controller
        XInputSetState(0, &Vibration);
    }*/
}

//Connexion ou déconnexion au robot lorsque l'évènement du bouton est declanché
void MainWindow::on_Connexion_clicked()
{
    if (myRobot.isConnect()) {
        myRobot.disConnect();
    }
    else {
        myRobot.doConnect();
    }

    /*
    if (myRobot.isConnect())
        //ui->Connexion->setText("Connecté");
    else
        //ui->Connexion->setText("Déconnecté");
    */
}

//Pilotage du robot sur l'interface
void MainWindow::on_Avancer_pressed()
{
    controlMoveRobot(ControllerType::INTERFACE, Direction::FORWARD, ui->setSpeed->value());
}

void MainWindow::on_Avancer_released()
{
    controlMoveRobot(ControllerType::INTERFACE);
}


void MainWindow::on_Gauche_pressed()
{
    controlMoveRobot(ControllerType::INTERFACE, Direction::LEFT, ui->setSpeed->value());
}

void MainWindow::on_Gauche_released()
{
    controlMoveRobot(ControllerType::INTERFACE);
}


void MainWindow::on_Droite_pressed()
{
    controlMoveRobot(ControllerType::INTERFACE, Direction::RIGHT, ui->setSpeed->value());
}

void MainWindow::on_Droite_released()
{
    controlMoveRobot(ControllerType::INTERFACE);
}


void MainWindow::on_Reculer_pressed()
{
    controlMoveRobot(ControllerType::INTERFACE, Direction::BACKWARD, ui->setSpeed->value());
}

void MainWindow::on_Reculer_released()
{
    controlMoveRobot(ControllerType::INTERFACE);
}

//Pilotage du robot et de la caméra avec les touches du clavier
void MainWindow::keyPressEvent(QKeyEvent *event){
    int touche = event->key();
    switch(touche){

            case Qt::Key_Z:
            controlMoveRobot(ControllerType::INTERFACE, Direction::FORWARD, ui->setSpeed->value());
            break;

            case Qt::Key_Q:
            controlMoveRobot(ControllerType::INTERFACE, Direction::LEFT, ui->setSpeed->value());
            break;

            case Qt::Key_D:
            controlMoveRobot(ControllerType::INTERFACE, Direction::RIGHT, ui->setSpeed->value());
            break;

            case Qt::Key_S:
            controlMoveRobot(ControllerType::INTERFACE, Direction::BACKWARD, ui->setSpeed->value());
            break;

            case Qt::Key_I:
            controlMoveCamera(ControllerType::INTERFACE, Direction::FORWARD);
            break;

            case Qt::Key_J:
            controlMoveCamera(ControllerType::INTERFACE, Direction::LEFT);
            break;

            case Qt::Key_L:
            controlMoveCamera(ControllerType::INTERFACE, Direction::RIGHT);
            break;

            case Qt::Key_K:
            controlMoveCamera(ControllerType::INTERFACE, Direction::BACKWARD);
            break;

            case Qt::Key_P:
            controlMoveRobot(ControllerType::INTERFACE, Direction::FORWARD, ui->setSpeed->value());
            QThread::sleep(2);
            controlMoveRobot(ControllerType::INTERFACE, Direction::RIGHT, ui->setSpeed->value());
            QThread::sleep(2);
            controlMoveRobot(ControllerType::INTERFACE, Direction::FORWARD, ui->setSpeed->value());
            QThread::sleep(2);
            controlMoveRobot(ControllerType::INTERFACE, Direction::RIGHT, ui->setSpeed->value());
            QThread::sleep(2);
            controlMoveRobot(ControllerType::INTERFACE, Direction::FORWARD, ui->setSpeed->value());
            QThread::sleep(2);
            controlMoveRobot(ControllerType::INTERFACE, Direction::RIGHT, ui->setSpeed->value());
            QThread::sleep(2);
            controlMoveRobot(ControllerType::INTERFACE, Direction::FORWARD, ui->setSpeed->value());

     }

}


void MainWindow::keyReleaseEvent(QKeyEvent *event){
    int touche = event->key();
    switch(touche){
            case Qt::Key_Z:
            case Qt::Key_Q:
            case Qt::Key_D:
            case Qt::Key_S:
            controlMoveRobot(ControllerType::INTERFACE);
            break;
     }
}

//Déplacement de la caméra à l'aide de l'interface
void MainWindow::on_Camera_Haut_clicked()
{
    controlMoveCamera(ControllerType::INTERFACE, Direction::FORWARD);
}

void MainWindow::on_Camera_Gauche_clicked()
{
    controlMoveCamera(ControllerType::INTERFACE, Direction::LEFT);
}

void MainWindow::on_Camera_Droite_clicked()
{
    controlMoveCamera(ControllerType::INTERFACE, Direction::RIGHT);
}


void MainWindow::on_Camera_Bas_clicked()
{
    controlMoveCamera(ControllerType::INTERFACE, Direction::BACKWARD);
}

//Pilotage du robot avec les touches de la manette
void MainWindow::GamepadPressButton(){
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonPressEvent, this,
            [this](int deviceId, QGamepadManager::GamepadButton button, double value){
        switch(button){
            case QGamepadManager::GamepadButton::ButtonY:
            controlMoveCamera(ControllerType::MANNETTE_BUTTON, Direction::FORWARD);
            break;

            case QGamepadManager::GamepadButton::ButtonX:
            controlMoveCamera(ControllerType::MANNETTE_BUTTON, Direction::LEFT);
            break;

            case QGamepadManager::GamepadButton::ButtonB:
            controlMoveCamera(ControllerType::MANNETTE_BUTTON, Direction::RIGHT);
            break;

            case QGamepadManager::GamepadButton::ButtonA:
            controlMoveCamera(ControllerType::MANNETTE_BUTTON, Direction::BACKWARD);
            break;

            case QGamepadManager::GamepadButton::ButtonUp:
            controlMoveRobot(ControllerType::MANNETTE_BUTTON, Direction::FORWARD, ui->setSpeed->value());
            break;

            case QGamepadManager::GamepadButton::ButtonLeft:
            controlMoveRobot(ControllerType::MANNETTE_BUTTON, Direction::LEFT, ui->setSpeed->value());
            break;

            case QGamepadManager::GamepadButton::ButtonRight:
            controlMoveRobot(ControllerType::MANNETTE_BUTTON, Direction::RIGHT, ui->setSpeed->value());
            break;

            case QGamepadManager::GamepadButton::ButtonDown:
            controlMoveRobot(ControllerType::MANNETTE_BUTTON, Direction::BACKWARD, ui->setSpeed->value());
            break;

            case QGamepadManager::GamepadButton::ButtonR2:
            //qDebug() << value;
            r2Press = value;
            controlMoveRobot(ControllerType::MANNETTE_R2);
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
            controlMoveRobot(ControllerType::MANNETTE_BUTTON);
            break;

            case QGamepadManager::GamepadButton::ButtonR2:
            r2Press = 0;
            controlMoveRobot(ControllerType::MANNETTE_R2);
            break;

            default:
            break;

            }

    });
}

//Récupération des valeurs des joysticks
void MainWindow::GamepadAxis(){
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadAxisEvent, this,
            [this](int deviceId, QGamepadManager::GamepadAxis button, double value){
       //qDebug() << value;
       //qDebug() << button;
        switch(button){
            case QGamepadManager::GamepadAxis::AxisLeftX:
            axisLeftX = value;
            controlMoveRobot(ControllerType::MANNETTE_AXIS);
            break;

            case QGamepadManager::GamepadAxis::AxisLeftY:
            axisLeftY = value;
            controlMoveRobot(ControllerType::MANNETTE_AXIS);
            break;

            case QGamepadManager::GamepadAxis::AxisRightX:
            axisRightX = value;
            controlMoveCamera(ControllerType::MANNETTE_AXIS);
            break;

            case QGamepadManager::GamepadAxis::AxisRightY:
            axisRightY = value;
            controlMoveCamera(ControllerType::MANNETTE_AXIS);
            break;

            default:
            break;
        }
    });
}

//Traduction de la postion du joystick en vecteur direction pour le déplacement du robot
Direction MainWindow::toDirectionRobot(double x, double y) {
    if((x*x)+(y*y) <= (0.1*0.1))
        return Direction::NONE;
    else if ((-2*x)<=y && (2*x)<=y)
        return Direction::BACKWARD;
    else if ((2*x)>=y && (0.5*x)<=y)
        return Direction::BACKWARD_RIGHT;
    else if ((0.5*x)>=y && (-0.5*x)<=y)
        return Direction::RIGHT;
    else if ((-0.5*x)>=y && (-2*x)<=y)
        return Direction::FORWARD_RIGHT;
    else if ((-2*x)>=y && (2*x)>=y)
        return Direction::FORWARD;
    else if ((2*x)<=y && (0.5*x)>=y)
        return Direction::FORWARD_LEFT;
    else if ((0.5*x)<=y && (-0.5*x)>=y)
        return Direction::LEFT;
    else if ((-0.5*x)<=y && (-2*x)>=y)
        return Direction::BACKWARD_LEFT;
    else return Direction::NONE;
}

//Gestion des différents controleur pour le déplacement du robot
void MainWindow::controlMoveRobot(ControllerType type, Direction direction, int speed) {
    Direction mannetteDirection = toDirectionRobot(axisLeftX, axisLeftY);
    if ((direction == Direction::FORWARD_LEFT || direction == Direction::FORWARD || direction == Direction::FORWARD_RIGHT || mannetteDirection == Direction::FORWARD_LEFT || mannetteDirection == Direction::FORWARD || mannetteDirection == Direction::FORWARD_RIGHT) && (ui->sensorFL->value() == 100 || ui->sensorFR->value() == 100))
        myRobot.move();
    else if((direction == Direction::BACKWARD_LEFT || direction == Direction::BACKWARD || direction == Direction::BACKWARD_RIGHT || mannetteDirection == Direction::FORWARD_LEFT || mannetteDirection == Direction::BACKWARD || mannetteDirection == Direction::BACKWARD_RIGHT) && (ui->sensorBL->value() == 100 || ui->sensorBR->value() == 100))
        myRobot.move();
    else if ((type == ControllerType::INTERFACE || type == ControllerType::MANNETTE_BUTTON) && mannetteDirection == Direction::NONE)
        myRobot.move(direction, speed);
    else if (type == ControllerType::MANNETTE_AXIS) {
        if (allAxisLeft)
            myRobot.move(mannetteDirection, r2Press*255);
        allAxisLeft = !allAxisLeft;
    }
    else if (type == ControllerType::MANNETTE_R2 && allAxisLeft)
        myRobot.move(mannetteDirection, r2Press*255);
}

//Traduction de la postion du joystick en vecteur direction pour le déplacement de la caméra
Direction MainWindow::toDirectionCamera(double x, double y) {
    if((x*x)+(y*y) <= (0.1*0.1))
        return Direction::NONE;
    else if ((-x)<=y && (x)<=y)
        return Direction::BACKWARD;
    else if ((-x)<=y && (x)>=y)
        return Direction::RIGHT;
    else if ((-x)>=y && (x)>=y)
        return Direction::FORWARD;
    else if ((-x)>=y && (x)<=y)
        return Direction::LEFT;
    else return Direction::NONE;
}

//Gestion des différents controleur pour le déplacement de la caméra
void MainWindow::controlMoveCamera(ControllerType type, Direction direction) {
    Direction mannetteDirection = toDirectionCamera(axisRightX, axisRightY);
    if ((type == ControllerType::INTERFACE || type == ControllerType::MANNETTE_BUTTON) && mannetteDirection == Direction::NONE)
        camera->move(direction);
    else if (type == ControllerType::MANNETTE_AXIS) {
        if (allAxisRight)
            camera->move(mannetteDirection);
        allAxisRight = !allAxisRight;
    }
}
