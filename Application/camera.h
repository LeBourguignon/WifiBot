#ifndef CAMERA_H
#define CAMERA_H

#include <QWebEngineView>
#include <QMainWindow>

class Camera
{
private:
    QWebEngineView *view;
    QMainWindow *mainWindow;

public:
    Camera(QMainWindow *_mainWindow);
    void move(int a);
};

#endif // CAMERA_H
