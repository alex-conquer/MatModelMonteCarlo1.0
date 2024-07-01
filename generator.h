#ifndef GENERATOR_H
#define GENERATOR_H
#include "wall.h"

class Generator {
  public:
    bool isDiskInVector();
    double generateMonteCarlo_Height();
    double generatorMonteCarlo_Fi();
    double generateMonteCarlo_Teta();
    double generateMonteCarlo_Gamma();
    RandomValues generateMonteCarlo_Cylinder();
    Coordinates generateMonteCarlo_GVector(Coordinates& coordinates);

    int generateMonteCarlo_index();
    void lookDiskIndexes();
    double generateMonteCarlo_Point(int index);
    RandomValues generateMonteCarlo_Disk();

    RandomValues placeForMolecul();

    double findCylindersArea();
    double findDiskArea();
    Coeficients makeDistributionArea();
    FindingCylinder findCylinderIndex(double height);
};

#endif // GENERATOR_H
