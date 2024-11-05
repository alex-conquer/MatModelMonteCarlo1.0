#include "analytical.h"
#include "wall.h"
#include <QDebug>
#include <math.h>
#include <iostream>
#include "generator.h"
#include "calculate.h"

extern QVector<Wall*> vector;
static Calculate calcul;

void Analytics::adequacyOfTheModel(int alphaSig, const QVector<double>& polinomialCoef,
                                   const double& ReproducibilityDisp, const QVector<double>& avgVec) {
    double Y1 = polinomialCoef[0] - polinomialCoef[1] - polinomialCoef[2] + polinomialCoef[3];
    double Y2 = polinomialCoef[0] + polinomialCoef[1] - polinomialCoef[2] - polinomialCoef[3];
    double Y3 = polinomialCoef[0] - polinomialCoef[1] + polinomialCoef[2] - polinomialCoef[3];
    double Y4 = polinomialCoef[0] + polinomialCoef[1] + polinomialCoef[2] + polinomialCoef[3];
    QVector<double> YVector;
    double DispAdecuacy = 0.;
    double Sum = 0.;
    double Fe = 0.;
    double N = 4;
    YVector.push_back(Y1);
    YVector.push_back(Y2);
    YVector.push_back(Y3);
    YVector.push_back(Y4);
    if (alphaSig == N) {
        ++N;
    }

    for (int i = 0; i < YVector.size(); ++i) {
        Sum += pow((avgVec[i] - YVector[i]), 2);
    }
    DispAdecuacy = (Sum * 5.) / (N - alphaSig);
    Fe = DispAdecuacy / ReproducibilityDisp;
    if (N - alphaSig == 1) {
        if (Fe <= 4.49) {
            qDebug() << "Fe =" << Fe << "<= Fcr: the model is adequate";
        } else {
            qDebug() << "Fe =" << Fe << ">= Fcr: the model is inadequate";
        }
    } else if (N - alphaSig == 2) {
        if (Fe <= 3.63) {
            qDebug() << "Fe =" << Fe << "<= Fcr: the model is adequate";
        } else {
            qDebug() << "Fe =" << Fe << ">= Fcr: the model is inadequate";
        }
    } else if (N - alphaSig == 3) {
        if (Fe <= 3.24) {
            qDebug() << "Fe =" << Fe << "<= Fcr: the model is adequate";
        } else {
            qDebug() << "Fe =" << Fe << ">= Fcr: the model is inadequate";
        }
    }
}

void Analytics::checkingReproducibility(const QVector<double>& DispVec) {
    double MaxDisp = DispVec[0];
    double Ge = 0.;
    double sumOfDisp = 0.;
    for (int i = 0; i < DispVec.size(); ++i) {
        if (DispVec[i] > MaxDisp) {
            MaxDisp = DispVec[i];
        }
        sumOfDisp += DispVec[i];
    }
    Ge = MaxDisp / sumOfDisp;
    if (Ge < 0.5441) {
        qDebug() << "The dispersions are homogeneous, the experiment is reproducible";
    } else {
        qDebug() << "The dispersions are heterogeneous, the experiment is not reproducible";
    }

}

int Analytics::significanceOfTheCoefficients(const QVector<double>& polinomCoef, double& ReproducibilityDisp, const QVector<double>& DispVec) {
    double DispSum = 0.;
    double DispJ = 0.;
    double ti = 0.;
    QVector<double> tiVec;
    int alphaSig = 0;
    for (int i = 0; i < DispVec.size(); ++i) {
        DispSum += DispVec[i];
    }
    ReproducibilityDisp = DispSum / DispVec.size();
    DispJ = ReproducibilityDisp / (20);
    qDebug() << "Significant coefficients: ";
    for (int i = 0; i < polinomCoef.size(); ++i) {
        ti = polinomCoef[i] / (sqrt(DispJ));
        if (ti > 2.1199) {
            ++alphaSig;
            qDebug() << "t" << i << ": " << ti;
            tiVec.push_back(ti);
        } else {
            tiVec.push_back(0.);
        }

    }
    return alphaSig;
}

QVector<double> Analytics::polinomialCoefCalc(const QVector<double>& avgVec) {
    double b0 = (avgVec[0] + avgVec[1] + avgVec[2] + avgVec[3]) / 4.;
    double b1 = (-avgVec[0] + avgVec[1] - avgVec[2] + avgVec[3]) / 4.;
    double b2 = (-avgVec[0] - avgVec[1] + avgVec[2] + avgVec[3]) / 4.;
    double b12 = (avgVec[0] - avgVec[1] - avgVec[2] + avgVec[3]) / 4.;
    QVector<double> polinomCoef;
    polinomCoef.push_back(b0);
    polinomCoef.push_back(b1);
    polinomCoef.push_back(b2);
    polinomCoef.push_back(b12);
    return polinomCoef;
}

double Analytics::DispCalc(QVector<double>& XiVec, QVector<double>& DispVec, int molecules, int iteration) {
    double part = 1 / static_cast<double>(molecules);
    double sum = 0.;
    double Disp = 0.;
    double avg = 0.;
    for (int i = 0; i < 5; ++i) {
        double Xi = calcul.core(molecules, iteration) * part * 100.;
        XiVec.push_back(Xi);
        sum += Xi;
    }
    avg = sum / 5.;
    for (int i = 0; i < 5; ++i) {
        sum += pow(XiVec[i] - avg, 2);
    }
    Disp = sum / 4;
    DispVec.push_back(Disp);
    return avg;
}



void Analytics::matModeling() {
    Disk* disk = new Disk(34,0);
    disk->location=true;
    vector.push_back(disk);
    Сylinder* cylinder = new Сylinder(34,305);
    vector.push_back(cylinder);
    Disk* disk1 = new Disk(305, 34);
    disk1->location=true;
    vector.push_back(disk1);
    Сylinder* cylinder1 = new Сylinder(305, 210);
    vector.push_back(cylinder1);
    Disk* disk3 = new Disk(305, 51);
    disk3->location=false;
    vector.push_back(disk3);
    QVector<double> XiVec1;
    QVector<double> XiVec2;
    QVector<double> XiVec3;
    QVector<double> XiVec4;
    QVector<double> DispVec;
    double avg = 0.;
    QVector<double> avgVec;
    QVector<double> polinomCoef;
    int alphaSig = 0;
    double ReproducibilityDisp = 0.;
    // Входные параметры:
    // количество раз, которое молекула отбивается от стенок
    // количество молекул, которые генерируются на стенках криостата
    avg = DispCalc(XiVec1, DispVec, 25000, 3);
    avgVec.push_back(avg);
    qDebug() << "The first selection(molecules = 25000, iterations = 3): ";
    for (int i = 0; i < XiVec1.size(); ++i) {
        qDebug() << XiVec1[i];
    }
    qDebug() << "Average: " << avg;
    qDebug() << "Disp: " << DispVec[0];


    avg = DispCalc(XiVec2, DispVec, 75000, 3);
    avgVec.push_back(avg);
    qDebug() << "The second selection(molecules = 75000, iterations = 3): ";
    for (int i = 0; i < XiVec2.size(); ++i) {
        qDebug() << XiVec2[i];
    }
    qDebug() << "Average: " << avg;
    qDebug() << "Disp: " << DispVec[1];


    avg = DispCalc(XiVec3, DispVec, 25000, 9);
    avgVec.push_back(avg);
    qDebug() << "The third selection(molecules = 25000, iterations = 9): ";
    for (int i = 0; i < XiVec3.size(); ++i) {
        qDebug() << XiVec3[i];
    }
    qDebug() << "Average: " << avg;
    qDebug() << "Disp: " << DispVec[2];


    avg = DispCalc(XiVec4, DispVec, 75000, 9);
    avgVec.push_back(avg);
    qDebug() << "The forth selection(molecules = 75000, iterations = 9): ";
    for (int i = 0; i < XiVec4.size(); ++i) {
        qDebug() << XiVec4[i];
    }
    qDebug() << "Average: " << avg;
    qDebug() << "Disp: " << DispVec[3];




    checkingReproducibility(DispVec);
    polinomCoef = polinomialCoefCalc(avgVec);
    qDebug() << "Polinomial coefficients: ";
    for (int i = 0; i < polinomCoef.size(); ++i) {
        qDebug() << polinomCoef[i];
    }

    alphaSig = significanceOfTheCoefficients(polinomCoef, ReproducibilityDisp, DispVec);
    adequacyOfTheModel(alphaSig, polinomCoef, ReproducibilityDisp, avgVec);
}
