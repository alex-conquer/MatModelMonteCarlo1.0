#ifndef CALCULATE_H
#define CALCULATE_H
#include "wall.h"

class Calculate {
public:
    void intersectionSearch(Coordinates& newCoordinates, int k);
    bool flightMolecule_Disk();

    Coordinates calculateFlightMolecule(Coordinates& coordinates);
    Coordinates calculateFlightMoleculeCylinder(Coordinates& coordinates, int i);
    Coordinates calculateFlightMoleculeDisk(Coordinates& coordinates, int i);

    void iterationForCylinder(Coordinates& coordinates);
    void iterationForDisk(Coordinates& coordinates);

    int core(int countMoleculs, int iteration);

    void creatingPortal(int val2 = 0, bool flag = false);
    void DefineDownOrUp(int count);


    double findDiskCoordZ(int index);
    bool checkForBoundCondition(Coordinates coordinates, Сylinder* cylinder);
    void iteration(Coordinates& NewCoordinates, int iteration);

    void test(){};
};

#endif // CALCULATE_H
