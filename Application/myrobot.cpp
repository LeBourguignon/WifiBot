// myrobot.cpp

#include "myrobot.h"

MyRobot::MyRobot(QObject *parent) : QObject(parent) {
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;

    DataToSend[2] = 0x00;
    DataToSend[3] = 0x00;
    DataToSend[4] = 0x00;
    DataToSend[5] = 0x00;
    DataToSend[6] = 0x00;

    quint16 crc = crc16(DataToSend, 7);

    DataToSend[7] = crc;
    DataToSend[8] = (crc >> 8);

    DataReceived.resize(21);
    TimerEnvoi = new QTimer();
    // setup signal and slot
    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot())); //Send data to wifibot timer
}


bool MyRobot::doConnect() {
    socket = new QTcpSocket(this); // socket creation
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    qDebug() << "connecting..."; // this is not blocking call
    //socket->connectToHost("LOCALHOST", 15020);
    socket->connectToHost("192.168.1.106", 15020); // connection to wifibot
    // we need to wait...
    if(!socket->waitForConnected(5000)) {
        qDebug() << "Error: " << socket->errorString();
        return false;
    }
    TimerEnvoi->start(75);
    return true;
}

void MyRobot::disConnect() {
    TimerEnvoi->stop();
    socket->close();
}

bool const MyRobot::isConnect() { return _isConnect; }

void MyRobot::connected() {
    qDebug() << "connected...";
    _isConnect = true;

}

void MyRobot::disconnected() {
    qDebug() << "disconnected...";
    _isConnect = false;
}

void MyRobot::bytesWritten(qint64 bytes) {
    //qDebug() << bytes << " bytes written...";
}

void MyRobot::readyRead() {
    //qDebug() << "reading..."; // read the data from the socket
    DataReceived = socket->readAll();
    emit updateUI(DataReceived);
}

void MyRobot::MyTimerSlot() {
    //qDebug() << "Timer...";
    while(Mutex.tryLock());
    socket->write(DataToSend);
    Mutex.unlock();
}

// Mise à jours des octets pour le déplacement du robot (Update Direction + Vitesse)
void MyRobot::move(Direction direction, quint8 velocity)
{
    while(Mutex.tryLock());
    this->DataToSend[2] = velocity;
    this->DataToSend[4] = velocity;
    switch(direction){
    case Direction::FORWARD:
        this->DataToSend[6] = 0b01010000;
        break;
    case Direction::FORWARD_LEFT:
        this->DataToSend[6] = 0b01010000;
        this->DataToSend[2] = 0;
        break;
    case Direction::FORWARD_RIGHT:
        this->DataToSend[6] = 0b01010000;
        this->DataToSend[4] = 0;
        break;
    case Direction::LEFT:
        this->DataToSend[6] = 0b00010000;
        break;
    case Direction::RIGHT:
        this->DataToSend[6] = 0b01000000;
        break;
    case Direction::BACKWARD:
        this->DataToSend[6] = 0b00000000;
        break;
    case Direction::BACKWARD_LEFT:
        this->DataToSend[6] = 0b00000000;
        this->DataToSend[4] = 0;
        break;
    case Direction::BACKWARD_RIGHT:
        this->DataToSend[6] = 0b00000000;
        this->DataToSend[2] = 0;
        break;
    default:
        this->DataToSend[2] = 0;
        this->DataToSend[4] = 0;
        this->DataToSend[6] = 0b01010000;
        break;
    }

    quint16 crc = crc16(DataToSend, 7);

    DataToSend[7] = crc;
    DataToSend[8] = (crc >> 8);

    Mutex.unlock();
}

// Calcul du CRC
quint16 MyRobot::crc16(QByteArray adresseTab, unsigned int tailleMax) {
    quint16 crc = 0xFFFF;
    quint16 polynome = 0xA001;
    quint16 parity =0;
    unsigned int cptBit = 0;

    for(auto it = adresseTab.begin()+1; it != adresseTab.begin()+tailleMax; ++it) {
        crc ^= (unsigned char)*it;
        for(cptBit = 0; cptBit <= 7; ++cptBit) {
            parity = crc;
            crc >>= 1;
            if(parity%2 == true)
                crc ^= polynome;
        }
    }
    return crc;
}
