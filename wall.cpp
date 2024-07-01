#include "wall.h"
#include "generator.h"
#include "calculate.h"
#include <string>
#include <memory>
#include <random>
#include <iostream>
#include <cmath>
#include <map>
#include "interface.h"
#include "calculate.h"
#define PI 3.14

int exitMolecules = 0;

extern QVector<Wall*> vector;
QVector<int> indexVector;
double Wall::coordinateZ = 0;
int Wall::indexNumber = -2;
static Generator generator;
extern Calculate calcul;
static bool flag = true;
static std::map<std::size_t, double> coordinateZMap;

Wall::Wall() {
    indexNumber++;
}

Wall::~Wall() {
    // деструктор Wall
}


Disk::Disk(double radiusOutside, double radiusInside): radiusInsideDisk(radiusInside), radiusOutsideDisk(radiusOutside) {
    index = indexNumber;
    name = "Disk";
    coordinateZMap[vector.size()] = coordinateZ;
}

Disk::~Disk() {
    std::cout << "poka";
}


Сylinder::Сylinder(double radiusOutside, double height): radiusOutsideCylinder(radiusOutside), Height(height) {
    index = indexNumber;
    name = "Cylinder";
    coordinateZ += Height;
    coordinateZMap[vector.size()] = coordinateZ;

}

Сylinder::~Сylinder() {}


//берет значения от нуля до высоты нашей модели и находит случайную
//точку по координате z , и именно сечение по этой координате мы будем исследовать
double Generator::generateMonteCarlo_Height()
{
    std::mt19937 generator; // Инициализация генератора
    int z_finish = Wall::coordinateZ;
    generator.seed(std::random_device()()); // Использование случайного устройства для засевания генератора
    std::uniform_real_distribution<double> distribution(0, z_finish); // Равномерное распределение от 1 до 100
    double randomValue = distribution(generator); // Генерация случайного числа
    return randomValue;
}

//далее в сечении из предыдущей функции находим конкретную точку
double Generator::generatorMonteCarlo_Fi()
{
    std::mt19937 generator;
    generator.seed(std::random_device()()); // Использование случайного устройства для засевания генератора
    std::uniform_real_distribution<double> distribution(0, 360);
    double randomValue = distribution(generator);
    return randomValue;
}

//это один из 2 углов, который будет необходим для построения луча, по которому молекула будет вылетать
double Generator::generateMonteCarlo_Teta()
{
    std::mt19937 generator;
    generator.seed(std::random_device()());
    std::uniform_real_distribution<double> distribution(0, 180); // Равномерное распределение от 1 до 180
    double teta = distribution(generator);
    return teta;
}

//это 2 угол, который будет необходим для построения луча, по которому молекула будет вылетать
double Generator::generateMonteCarlo_Gamma()
{
    std::mt19937 generator;
    generator.seed(std::random_device()());
    std::uniform_real_distribution<double> distribution(0, 180); // Равномерное распределение от 1 до 180
    double gamma = distribution(generator);
    return gamma;
}

RandomValues Generator::generateMonteCarlo_Cylinder()
{
    RandomValues cylinderValues;
    cylinderValues.height = generateMonteCarlo_Height();
    cylinderValues.fi = generatorMonteCarlo_Fi();
    cylinderValues.teta = generateMonteCarlo_Teta();
    cylinderValues.gamma = generateMonteCarlo_Gamma();
    return cylinderValues;
}

Coordinates Generator::generateMonteCarlo_GVector(Coordinates& coordinates) {// возможно ошибка
    double teta = generateMonteCarlo_Teta();
    double gamma = generateMonteCarlo_Gamma();
    teta = (teta * PI) / 180;
    gamma = (gamma * PI) / 180;
    coordinates.p1 = sin(teta) * cos(gamma);
    coordinates.p2 = sin(teta) * sin(gamma);
    coordinates.p3 = cos(teta);
    return coordinates;
}

void Generator::lookDiskIndexes()
{
    for (Wall* wall : vector) {
            Disk* disk = dynamic_cast<Disk*>(wall); // Попытка приведения типа
            if (disk && disk->name == "Disk" ) {
                indexVector.push_back(disk->index);
            }
    }
    flag = false;
}

bool Generator::isDiskInVector()
{
    for (int i = 0; i < vector.size(); i++){
        if (vector[i]->name=="Disk"){
            return true;
        }
    } return false;
}

int Generator::generateMonteCarlo_index()
{    // Проверка на пустоту вектора
    if (indexVector.empty() && (!generator.isDiskInVector())) {
        throw std::runtime_error("Vector is empty");
    }
    if (flag == true) {
    lookDiskIndexes();
    }
    // Статические объекты для генерации случайных чисел
    static std::mt19937 gen(time(nullptr)); // генератор случайных чисел, инициализированный системными часами
    Disk* first_disk = dynamic_cast<Disk*>(vector[0]);
    Disk* second_disk = dynamic_cast<Disk*>(vector[vector.size() - 1]);
    if (!first_disk || !second_disk) {
        throw std::exception();
    }
    int ternar1 = first_disk->portal == true ? 1 : 0;
    int ternar2 = second_disk->portal == true ? indexVector.size() - 2 : indexVector.size() - 1;
    std::uniform_int_distribution<size_t> dist(ternar1, ternar2);// распределение для индексов

    return indexVector[dist(gen)];// Выбор случайного индекса и возвращение элемента
}

double Generator::generateMonteCarlo_Point(int index)
{
    Wall* topWall = vector[index];
    Disk* disk = dynamic_cast<Disk*>(topWall);
    int firstPoint = disk->radiusInsideDisk;
    int twicePoint = disk->radiusOutsideDisk;
    // Создаем генератор случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(firstPoint, twicePoint);

    // Генерируем и возвращаем случайное число из заданного диапазона
    return dist(gen);
}

RandomValues Generator::generateMonteCarlo_Disk()
{
    RandomValues diskValues;
    diskValues.index = generateMonteCarlo_index();
    diskValues.fi = generatorMonteCarlo_Fi();
    diskValues.point = generateMonteCarlo_Point(diskValues.index);
    diskValues.teta = generateMonteCarlo_Teta();
    diskValues.gamma = generateMonteCarlo_Gamma();
    return diskValues;
}

double Generator::findCylindersArea()
{
    double area = 0;
    for (Wall* wall : vector) {
            Сylinder* cylinder = dynamic_cast<Сylinder*>(wall); // Попытка приведения типа
            if (cylinder && cylinder->name == "Cylinder") {
                double height = cylinder->Height;
                double radius = cylinder->radiusOutsideCylinder;
                area += height * 2 * PI * radius;
            }
        }
    return area;
}

double Generator::findDiskArea()
{
    double area = 0;
    for (Wall* wall : vector) {
            Disk* disk = dynamic_cast<Disk*>(wall); // Попытка приведения типа
            if (disk && disk->name == "Disk" && disk->portal!=true) {//мы не должны учитывать площадь дисков которые являются поверхностями вылета
                double radiusInside = disk->radiusInsideDisk;
                double radiusOutside = disk->radiusOutsideDisk;
                area += 2 * PI * (pow(radiusOutside, 2) - pow(radiusInside, 2));
            }
        }
    return area;
}

Coeficients Generator::makeDistributionArea()
{
    Coeficients coeficitions;
    double areaCylinders = generator.findCylindersArea();
    double areaDisk = generator.findDiskArea();
    coeficitions.CylinderCoef = areaCylinders / (areaCylinders + areaDisk);
    coeficitions.DiskCoef = 1 - coeficitions.CylinderCoef;
    return coeficitions;
}


FindingCylinder Generator::findCylinderIndex(double height) {
    FindingCylinder coord;
    coord.diff = 0.;
    coord.index = 0;
    for (int i = 0; i < vector.size(); ++i) {
        Сylinder* cylinder = dynamic_cast<Сylinder*>(vector[i]);
        if (cylinder != nullptr && cylinder->name == "Cylinder") {
            if (height < coordinateZMap[i]) {
                coord.diff = cylinder->Height - (coordinateZMap[i] - height);
                break;
            }
        }
        ++(coord.index);
    }
    return coord;
}

Coordinates& Coordinates::operator=(const RandomValues& other)
{
    const double pi = PI;
    double p1 = sin((other.teta * pi) / 180) * cos((other.gamma * pi) / 180);
    double p2 = sin((other.teta * pi) / 180) * sin((other.gamma * pi) / 180);
    double p3 = cos((other.teta * pi) / 180);
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    if (other.height != 0.) {
        FindingCylinder coord = generator.findCylinderIndex(other.height);
        Сylinder* cylinder = dynamic_cast<Сylinder*>(vector[coord.index]);
        double x0 = cylinder->radiusOutsideCylinder * cos((other.fi * pi) / 180);
        double y0 = cylinder->radiusOutsideCylinder * sin((other.fi * pi) / 180);
        double z0 = other.height;
        this->x = x0;
        this->y = y0;
        this->z = z0;
        this->index = coord.index;
    }
    else {
        //Сylinder* cylinder = dynamic_cast<Сylinder*>(vector[other.index - 1]);
        //double coordZ = cylinder->coordinateZ;
        this->x = other.point * cos((other.fi * pi) / 180);
        this->y = other.point * sin((other.fi * pi) / 180);
        //this->z = other.index != 0 ? coordZ : 0;
        this->z = coordinateZMap[other.index];
        this->index = other.index;
    }
    return *this;
}


void Calculate::intersectionSearch(Coordinates& NewCoordinates, int k)
{
    if (vector[k]->name == "Disk") {
        NewCoordinates = calculateFlightMoleculeDisk(NewCoordinates, k);
        if (NewCoordinates.flag == FOUND) {
            Disk* disk = dynamic_cast<Disk*>(vector[NewCoordinates.index]);
            if (disk->portal) {
                NewCoordinates.flag = EXIT;
                ++exitMolecules;
                return;
            }
            generator.generateMonteCarlo_GVector(NewCoordinates);
            return;
            // я думаю тут сделать чтоб оно возвращало структуру , и если флаг FOUND то выходим из ф-и и возращаем структуру
            //или сделать все по ссылке как ты и хотел,
        }
    }
    else if (vector[k]->name == "Cylinder") {
        // Необходимо перезаписать индекс именно найденной фигуры
        NewCoordinates = calculateFlightMoleculeCylinder(NewCoordinates, k);
        if (NewCoordinates.flag == FOUND) {
            generator.generateMonteCarlo_GVector(NewCoordinates);// Надо понять перезаписывается ли направляющий вектор
            return;
        }
        return;
    }
}



void Calculate::iterationForCylinder(Coordinates& NewCoordinates)
{

    bool breakCondition = false;
    int count = 1;
    while (NewCoordinates.flag == NOT_FOUND) {
        if (breakCondition) {
            break;
        }
        for (int k = NewCoordinates.index; (k < NewCoordinates.index + count + 1) && (k < vector.size()); ++k) {// Нашел ошибку здесь
            calcul.intersectionSearch(NewCoordinates, k);
            if (NewCoordinates.flag == EXIT || NewCoordinates.flag == FOUND) {
                //++count1;
                return;
            }
            //++count1;
        }
        if (NewCoordinates.flag == NOT_FOUND) {
            for (int k = NewCoordinates.index; (k > NewCoordinates.index - count - 1) && (k > -1); --k) {
                calcul.intersectionSearch(NewCoordinates, k);
                if (NewCoordinates.flag == EXIT || NewCoordinates.flag == FOUND) {
                    //++count1;
                    return;
                }
                //++count1;
            }
        }
        ++count;
        if (count == vector.size()) {
            breakCondition = true;
        }
    }
}


void Calculate::iterationForDisk(Coordinates& NewCoordinates)
{
    Disk* disk = dynamic_cast<Disk*>(vector[NewCoordinates.index]);
    if (disk->location) {
        for (int i = NewCoordinates.index + 1; i < vector.size(); ++i) {
            calcul.intersectionSearch(NewCoordinates, i);
            if (NewCoordinates.flag == EXIT || NewCoordinates.flag == FOUND) {
                //++count1;
                return;
            }
            //++count1;
            //std::cout << "ЫЫЫЫ" << std::endl;
        }
    }
    else {
        for (int i = NewCoordinates.index - 1; i > -1; --i) {
            calcul.intersectionSearch(NewCoordinates, i);
            if (NewCoordinates.flag == EXIT || NewCoordinates.flag == FOUND) {
                //++count1;
                return;
            }
            //++count1;
            //std::cout << "АААА" << std::endl;
        }
    }
}


void Calculate::iteration(Coordinates& NewCoordinates, int iteration) {

    int j = 0;
    while (j < iteration - 1) {
        if (vector[NewCoordinates.index]->name == "Disk") {// Здесь может быть SIGABRT, проверено, надо исправить
             iterationForDisk(NewCoordinates);
             if (NewCoordinates.flag == EXIT) {return;}
             j++;
             NewCoordinates.flag = NOT_FOUND;
        }
        else if (vector[NewCoordinates.index]->name == "Cylinder") {
             iterationForCylinder(NewCoordinates);
             if (NewCoordinates.flag == EXIT) {return;}
             j++;
             NewCoordinates.flag = NOT_FOUND;
        }
    }

}


int Calculate::core(int countMoleculs, int iteration)
{
    exitMolecules = 0;
    calcul.creatingPortal();
    RandomValues rand;
    Coeficients coeficionts = {};
    Coordinates NewCoordinates = {};
    coeficionts = generator.makeDistributionArea();//убрал лишние вызовы

    for (int i = 0; i < countMoleculs * coeficionts.CylinderCoef; ++i) {
        rand = generator.generateMonteCarlo_Cylinder();
        NewCoordinates = rand;
        iterationForCylinder(NewCoordinates);
        //std::cout << i << std::endl;
        if (NewCoordinates.flag == EXIT) {
            NewCoordinates.flag = NOT_FOUND;
            continue;
        } else if (NewCoordinates.flag == FOUND) {
            NewCoordinates.flag = NOT_FOUND;
        }
        calcul.iteration(NewCoordinates, iteration);
    }

    for (int i = 0; i < countMoleculs * coeficionts.DiskCoef; ++i) {
        rand = generator.generateMonteCarlo_Disk();
        NewCoordinates = rand;
        iterationForDisk(NewCoordinates);
        if (NewCoordinates.flag == EXIT) {
            NewCoordinates.flag = NOT_FOUND;
            continue;
        } else if (NewCoordinates.flag == FOUND) {
            NewCoordinates.flag = NOT_FOUND;
        }

        calcul.iteration(NewCoordinates, iteration);
    }

    return exitMolecules;
}

Coordinates Calculate::calculateFlightMoleculeDisk(Coordinates& coordinates, int i)
{
    Coordinates point;
    Disk* disk = dynamic_cast<Disk*>(vector[i]);
    Сylinder* cylinder = dynamic_cast<Сylinder*>(vector[coordinates.index]);
    if (abs(coordinates.index - i) == 1 && cylinder && cylinder->radiusOutsideCylinder == disk->radiusInsideDisk) {
        return coordinates;
    }
    double t = (coordinateZMap[i] - coordinates.z) / coordinates.p3;
    double x_0 = coordinates.x + coordinates.p1 * t;
    double y_0 = coordinates.y + coordinates.p2 * t;
    if (sqrt(pow(x_0, 2) + pow(y_0, 2)) > disk->radiusInsideDisk &&
        sqrt(pow(x_0, 2) + pow(y_0, 2)) < disk->radiusOutsideDisk) {
        point.x = x_0;
        point.y = y_0;
        point.z = coordinateZMap[i];
        point.flag = FOUND;
        point.index = i;
        return point;
    }
    else {
        coordinates.flag = NOT_FOUND;
        return coordinates;
    }

}


bool Calculate::checkForBoundCondition(Coordinates coordinates, Сylinder *cylinder) {
    //std::cout << "coordinate z: " << cylinder->coordinateZ << std::endl;
    return (coordinates.z < coordinateZMap[cylinder->index] - cylinder->Height) || (coordinates.z > coordinateZMap[cylinder->index]);
}

Coordinates Calculate::calculateFlightMoleculeCylinder(Coordinates& coordinates, int i) { // ИСПРАВИТЬ СРОЧНО!!!!!!! НЕТ ПРОВЕРКИ НА ГРАНИЧНЫЕ УСЛОВИЯ

    Coordinates pointBegin;
    Coordinates pointEnd;
    Сylinder* cylinder = dynamic_cast<Сylinder*>(vector[i]);
    Disk* disk = dynamic_cast<Disk*>(vector[coordinates.index]);
    if (abs(coordinates.index - i) == 1 && disk && disk->radiusInsideDisk == cylinder->radiusOutsideCylinder) {
        return coordinates;
    }
    double A = pow(coordinates.p1, 2) + pow(coordinates.p2, 2);
    double B = 2 * (coordinates.x * coordinates.p1 + coordinates.y * coordinates.p2);
    double C = pow(coordinates.x, 2) + pow(coordinates.y, 2) - pow(cylinder->radiusOutsideCylinder, 2);
    double Dis = pow(B, 2) - 4 * A * C;
    if (Dis > 0) {
        double t1 = (-B + pow(Dis, 0.5)) / (2 * A);
        double t2 = (-B - pow(Dis, 0.5)) / (2 * A);
        pointBegin.x = coordinates.x + coordinates.p1 * t1;
        pointBegin.y = coordinates.y + coordinates.p2 * t1;
        pointBegin.z = coordinates.z + coordinates.p3 * t1;
        pointEnd.x = coordinates.x + coordinates.p1 * t2;
        pointEnd.y = coordinates.y + coordinates.p2 * t2;
        pointEnd.z = coordinates.z + coordinates.p3 * t2;

        if (checkForBoundCondition(pointBegin, cylinder)) {
            pointBegin.flag = NOT_FOUND;
        } else {
            pointBegin.flag = FOUND;
        }

        if (checkForBoundCondition(pointEnd, cylinder)) {
            pointEnd.flag = NOT_FOUND;
        } else {
            pointEnd.flag = FOUND;
        }

        if (pointBegin.flag == FOUND) {
            if (!(t1 == 0. || (t1 > -1e-10 && t1 < 1e-10))) {
                //findingCylinder coord = generator.FindCylinderIndex(pointBegin.z);
                pointBegin.index = i;
                return pointBegin;
            }
        }

        if (pointEnd.flag == FOUND) {
            if (!(t2 == 0. || (t2 > -1e-10 && t2 < 1e-10))) {
                //findingCylinder coord = generator.FindCylinderIndex(pointEnd.z);
                pointEnd.index = i;
                return pointEnd;
            }
        }

        if (pointBegin.flag == NOT_FOUND || pointEnd.flag == NOT_FOUND) {
            coordinates.flag = NOT_FOUND;
            return coordinates;
        }
    }
    else if (Dis < 0) {
        throw std::exception(); // вылетает на этой строке
        //coordinates.flag = NOT_FOUND;
        //return coordinates;
    }
    else {
        coordinates.flag = NOT_FOUND;
        return coordinates;
    }

}
