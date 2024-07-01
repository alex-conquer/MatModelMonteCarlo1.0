#ifndef ANALYTICAL_H
#define ANALYTICAL_H
#include <QVector>

class Analytics
{
public:
    void matModeling();
    double DispCalc(QVector<double>& XiVec, QVector<double>& dispVec, int molecules, int iteration);
    void checkingReproducibility(const QVector<double>& dispVec);
    QVector<double> polinomialCoefCalc(const QVector<double>& avgVec);
    int significanceOfTheCoefficients(const QVector<double>& polinomCoef, double& reproducibilityDisp,const QVector<double>& dispVec);
    void adequacyOfTheModel(int alphaSig, const QVector<double>& polinomCoef, const double& reproducibilityDisp, const QVector<double>& avgVec);
};


#endif // ANALYTICAL_H
