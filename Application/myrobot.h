#ifndef MYROBOT_H
#define MYROBOT_H

#include <QObject>

class MyRobot : public QObject
{
    Q_OBJECT
public:
    explicit MyRobot(QObject *parent = nullptr);

signals:

};

#endif // MYROBOT_H
