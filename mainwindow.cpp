#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <iostream>
#include<fstream>
#include<ostream>

using namespace  std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
 //
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    lidar.stop();
    delete ui;
}

double aktualX = 2100;
double aktualY = 300;
double angle = 0;               // aktualny uhol natocenia robota v stupnoch
double cieloveX = 2100;//3450+500;//3450;//146;
double cieloveY = 2500;//1830+500;//138;
double vzdialenostOdCiela = 0;
double ziadanyUhol = 0;
//double zaciatocneX = 0;
//double zaciatocneY = 0;
double rychlostOtocenia = 100;
double presnostZastavenia = 10;
bool mozemVpred = false;
bool vidimCiel = true;
bool pomocnaZaciatok = true;
int prejdenaVzdialenost=0;
int mapa[200][200];
FILE *vystup;


void MainWindow::natocSaKCielu(int cielx, int ciely)
{
/*    double ziadanyUhol = (cieloveY-aktualY) / vzdialenostOdCiela *180/Pi;

    while (fabs(ziadanyUhol-angle) > 25) {
        if(angle >= 360)
            angle = angle-360;
        robot.move(10,-10);

    }
    robot.move(0,0);*/
}

void MainWindow::otocRobot(int rychlostPravehoKolesa, int rychlostLavehoKolesa){
    robot.move(rychlostPravehoKolesa, rychlostLavehoKolesa);
}

void MainWindow::chodRobot(int rychlostPravehoKolesa, int rychlostLavehoKolesa){
    robot.move(rychlostPravehoKolesa, rychlostLavehoKolesa);
}

void MainWindow::vidimCiel(LaserMeasurement &variable){
    for(int i=0; i < variable.numberOfScans;i++){
        double uhol = variable.Data[i].scanAngle;
        double vzdialen= variable.Data[i].scanDistance*1.662;

        if(vzdialen < 400 && vzdialen != 0){

        }
     //   sleep(0.5);
    }
}

void MainWindow::zmerajLidar(LaserMeasurement &variable){

  //  sleep(0.250);

    try {
    double xPrekazka = 0;
    double yPrekazka = 0;
    double xGlobPrekazka = 0;
    double yGlobPrekazka = 0;

    for(int i=0; i < variable.numberOfScans;i++){
        double uhol = variable.Data[i].scanAngle;
        double vzdialen= variable.Data[i].scanDistance*1.662;

        if(vzdialen < 400 && vzdialen != 0){
            //printf("Dobry uhol %f   vzdialenost:%f \n",uhol,vzdialen);
            xPrekazka = cos(uhol*(Pi/180))*vzdialen;
            yPrekazka = sin(uhol*(Pi/180))*vzdialen;

            xGlobPrekazka = cos((angle-uhol)*(Pi/180))*vzdialen + aktualX/10;
            yGlobPrekazka = sin((angle-uhol)*(Pi/180))*vzdialen + aktualY/10;

            int rozmer1 = yGlobPrekazka/10;
            int rozmer2 = xGlobPrekazka/10;

            if(rozmer1 >= 0 && rozmer2 >= 0)
                mapa[rozmer1+50][rozmer2+50] = 1;
        }
     //   sleep(0.5);
    }


        for (int prechodPolom1 = 0; prechodPolom1 < 46; prechodPolom1++){ //prechadzanie x
            for (int prechodPolom2 = 0; prechodPolom2 < 32; prechodPolom2++){ // prechadzanie y
                printf("%d", mapa[prechodPolom1][prechodPolom2]);
            }
            printf("\n");
        }
        printf("\n\n");
      //  printf("\n");

    } catch (int g) {
        printf("ahoj");
    }
}

void MainWindow::vykresliMapu(){
    for (int prechodPolom1 = 0; prechodPolom1 < 46; prechodPolom1++){ //prechadzanie x
        for (int prechodPolom2 = 0; prechodPolom2 < 32; prechodPolom2++){ // prechadzanie y
            printf("%d", mapa[prechodPolom1][prechodPolom2]);
        }
        printf("\n");
    }
}

int pocetY ;
int pocetX;
bool zapisalSom = false;

int MainWindow::demoCallback(CreateSensors inputData,void *ioPointer)
{

    vystup = fopen("vystup.txt","a");
  /*  fprintf(vystup, "%d ",mapa[0][0]);
    fclose(vystup);*/

    if (fabs(vzdialenostOdCiela < presnostZastavenia && zapisalSom==false) ){
        fprintf(vystup, "\n");
        fprintf(vystup, "\n");
        fprintf(vystup, "\n");

        for (pocetX=100; pocetX>=0; pocetX--){
            for(pocetY = 0; pocetY <= 100; pocetY++){
                fprintf(vystup, "%d ",mapa[pocetX][pocetY]);
            }
            fprintf(vystup, "\n");
        }
        fclose(vystup);
        zapisalSom=true;
    }





    MainWindow *currentWindow=(MainWindow*)ioPointer;
  //  currentWindow->update();
    QString vystup = "Neplatny udaj.";
    vystup = QString::number(inputData.Voltage);

    LaserMeasurement variable = currentWindow->lidar.getMeasurement();

    QString uhol = QString::number(inputData.Angle);
    QString vzdialenost = QString::number(inputData.Distance);          // prejdena vzdialenost

    angle = angle + inputData.Angle;                                    // aktualne natocenie robota
    aktualX = aktualX + inputData.Distance * cos(((2*Pi)/360) * angle);
    aktualY = aktualY + inputData.Distance * sin(((2*Pi)/360) * angle);

    prejdenaVzdialenost = prejdenaVzdialenost+inputData.Distance;

    if(abs(prejdenaVzdialenost%80)<20){
        ziadanyUhol = atan2((cieloveY-aktualY),(cieloveX-aktualX))*180/Pi;
        prejdenaVzdialenost=0;
    }

    vzdialenostOdCiela = sqrt(pow(cieloveX-aktualX,2)+pow(cieloveY-aktualY,2));
    QString vzdialenostOdCielaS = QString::number(vzdialenostOdCiela);

    QString uholaktualny = QString::number(angle);
    QString osX = QString::number(aktualX);
    QString osY = QString::number(aktualY);

    currentWindow->ui->label->setText(uholaktualny);
    currentWindow->ui->label_2->setText(vzdialenost);
    currentWindow->ui->label_3->setText(osX);
    currentWindow->ui->label_4->setText(osY);
    currentWindow->ui->label_5->setText(vzdialenostOdCielaS);
   //printf("nejde\n");
    //printf("data %i\n",inputData.BatteryCapacity);

    // Zaciatocne otocenie
    if(angle >= 180)
        angle = angle - 180;
    if(angle <= -180)
        angle = angle + 180;

    if(fabs(ziadanyUhol-angle) > 20 && vzdialenostOdCiela > presnostZastavenia){
        mozemVpred=false;

        if((ziadanyUhol-angle) >= 0){
            currentWindow->otocRobot(rychlostOtocenia, -rychlostOtocenia);
        }

        else if((ziadanyUhol-angle) < 0){
            currentWindow->otocRobot(-rychlostOtocenia, rychlostOtocenia);
        }
    }

    else if(fabs(ziadanyUhol-angle) > 8 && fabs(ziadanyUhol-angle) <= 20 && vzdialenostOdCiela > presnostZastavenia){
        mozemVpred=false;

        if((ziadanyUhol-angle) >= 0){
            currentWindow->otocRobot(rychlostOtocenia/2, -rychlostOtocenia/2);
        }

        else if((ziadanyUhol-angle) < 0){
            currentWindow->otocRobot(-rychlostOtocenia/2, rychlostOtocenia/2);
        }
    }

    else{
        currentWindow->otocRobot(0,0);
        mozemVpred=true;
    }

    // Pohyb vpred na ziadanu hodnotu
    if(fabs(vzdialenostOdCiela) > presnostZastavenia && mozemVpred == true){
            if(vzdialenostOdCiela < 200){
                currentWindow->chodRobot(100,100);
            }
            else if(vzdialenostOdCiela < 150){
                currentWindow->chodRobot(60,60);
            }
            else if(vzdialenostOdCiela < 100){
                currentWindow->chodRobot(20,20);
            }
            else{
                currentWindow->chodRobot(150,150);
            }

    }
    else if(fabs(vzdialenostOdCiela < presnostZastavenia) && mozemVpred == true ){
        currentWindow->chodRobot(0, 0);


/*
        for (int pocetX=0; pocetX<200; pocetX++){
            for(int pocetY = 200; pocetY < 0; pocetY--){
                fprintf(vystup, "%d ",mapa[pocetX][pocetY]);
            }
            fprintf(vystup, "\n");
        }

        fclose(vystup);*/
       // vystup << "data";
    }

    currentWindow->zmerajLidar(variable);
 //   currentWindow->vykresliMapu();

  /*  for(int i=0; i < 1000;i++){
        double uhol = variable.Data[i].scanAngle;
        double vzdialenost = 1.4*variable.Data[i].scanDistance;

        if(vzdialenost < 10){
            printf("Dobry uhol %f   vzdialenost:%f \n",uhol,vzdialenost);
        }
    }*/
}
void MainWindow::on_pushButton_clicked()
{
    vzdialenostOdCiela = sqrt(pow(cieloveX-aktualX, 2)+pow(cieloveY-aktualY, 2));

    ziadanyUhol = atan2((cieloveY-aktualY),(cieloveX-aktualX))*180/Pi;

    for (int prechodPolom1 = 0; prechodPolom1 < 46; prechodPolom1++){ //prechadzanie x
        for (int prechodPolom2 = 0; prechodPolom2 < 32; prechodPolom2++){ // prechadzanie y
            mapa[prechodPolom1][prechodPolom2] = 0;
        }
    }

    lidar.connect("/dev/laser");
    lidar.enable();
    lidar.start();
  /* lidar.getMeasurement();
    lidar.measure();*/
    LaserMeasurement variable = lidar.getMeasurement();
    vidimCiel(variable);
    sleep(0.300);
    robot.ConnectToPort("/dev/robot",this);
    connect( this, SIGNAL( showMB() ), this, SLOT( showMessageBox() ), Qt::BlockingQueuedConnection );
    robot.dataProcess(this,&demoCallback);

  //  lidar.stop();
}

void MainWindow::on_pushButton_2_clicked()
{
  //  robot.move(50,50);
   // usleep(1000*1000);
   // robot.move(0,0);
    //robot.doSensorReadings(this, &demoCallback);
    //natocSaKCielu(50,50);
   //ziadanyUhol = (cieloveY-aktualY) / vzdialenostOdCiela *180/Pi;
   //vzdialenostOdCiela = sqrt(pow(cieloveX-aktualX, 2)+pow(cieloveY-aktualY, 2));
}
