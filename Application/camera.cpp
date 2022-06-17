#include "camera.h"

Camera::Camera(QMainWindow *_mainWindow)
    : mainWindow(_mainWindow)
{
    view = mainWindow->findChild<QWebEngineView*>("Camera");
    view->load(QUrl("http://192.168.1.106:8080/?action=stream"));
    view->show();
}

void Camera::move(Direction direction, bool slow){
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    switch(direction){
    case Direction::FORWARD: //haut
        if (!slow)
            manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-200")));
        else
            manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-20")));
        break;

    case Direction::LEFT: //gauche
        if (!slow)
            manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=200")));
        else
            manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=20")));
        break;

    case Direction::RIGHT: //droite
        if(!slow)
            manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-200")));
        else
            manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-20")));
        break;

    case Direction::BACKWARD: //bas
        if (!slow)
            manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=200")));
        else
            manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=20")));
        break;

    default:
        break;
    }

}
