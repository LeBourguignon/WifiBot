#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    void on_Reculer_clicked();

    void on_Gauche_clicked();

    void on_Avancer_clicked();

    void on_Droite_clicked();

    void on_Camera_Haut_clicked();

    void on_Camera_Droite_clicked();

    void on_Camera_Bas_clicked();

    void on_Camera_Gauche_clicked();

    void on_Niv_Batterie_overflow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
