#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "irobotcreate.h"
#include "rplidar.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void showMB();
public:

    void forMsgBox(){emit showMB();}

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static int demoCallback(CreateSensors inputData,void *ioPointer);
        iRobotCreate robot;
       rplidar lidar;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void showMessageBox()
            {
                QMessageBox Msgbox;
                Msgbox.setText("Save work and change Battery!");
                Msgbox.setIcon(QMessageBox::Critical);
                Msgbox.exec();
                exit(-1);
            }
    void natocSaKCielu(int cielx, int ciely);
    void otocRobot(int rychlostPravehoKolesa, int rychlostLavehoKolesa);
    void chodRobot(int rychlostPravehoKolesa, int rychlostLavehoKolesa);
    void zmerajLidar(LaserMeasurement &variable);
    void vykresliMapu();
    void vidimCiel(LaserMeasurement &variable);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
