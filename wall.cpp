#include "wall.h"
#include <string>
#include <memory>
#include "interface.h"
#include <random>
#include <iostream>
#include <cmath>
#define PI 3.14


static int exitMolecules;

extern QVector<Wall*> vector;
QVector<int> indexVector;
double Wall::coordinateZ = 0;
int Wall::indexNumber = -2;
static Generator generator;

Wall::Wall() {
    indexNumber++;
}

Wall::~Wall() {
    // деструктор Wall
}


Disk::Disk(double radiusOutside, double radiusInside): radiusInsideDisk(radiusInside), radiusOutsideDisk(radiusOutside) {
    index = indexNumber;
    name = "Disk";
}

Disk::~Disk() {
    std::cout << "poka";
}


Сylinder::Сylinder(double radiusOutside, double height): radiusOutsideCylinder(radiusOutside), Height(height) {
    index = indexNumber;
    name = "Cylinder";
    coordinateZ += Height;
}

Сylinder::~Сylinder() {}


//берет значения от нуля до высоты нашей модели и находит случайную
//точку по координате z , и именно сечение по этой координате мы будем исследовать
double Generator::GeneratorMonteCarlo_Height()
{
    std::mt19937 generator; // Инициализация генератора
    int z_finish = Wall::coordinateZ;
    generator.seed(std::random_device()()); // Использование случайного устройства для засевания генератора
    std::uniform_real_distribution<double> distribution(0, z_finish); // Равномерное распределение от 1 до 100
    double randomValue = distribution(generator); // Генерация случайного числа
    return randomValue;
}

//далее в сечении из предыдущей функции находим конкретную точку
double Generator::GeneratorMonteCarlo_Fi()
{
    std::mt19937 generator;
    generator.seed(std::random_device()()); // Использование случайного устройства для засевания генератора
    std::uniform_real_distribution<double> distribution(0, 360);
    double randomValue = distribution(generator);
    return randomValue;
}

//это один из 2 углов, который будет необходим для построения луча, по которому молекула будет вылетать
double Generator::GeneratorMonteCarlo_Teta()
{
    std::mt19937 generator;
    generator.seed(std::random_device()());
    std::uniform_real_distribution<double> distribution(0, 180); // Равномерное распределение от 1 до 180
    double teta = distribution(generator);
    return teta;
}

//это 2 угол, который будет необходим для построения луча, по которому молекула будет вылетать
double Generator::GeneratorMonteCarlo_Gamma()
{
    std::mt19937 generator;
    generator.seed(std::random_device()());
    std::uniform_real_distribution<double> distribution(0, 180); // Равномерное распределение от 1 до 180
    double gamma = distribution(generator);
    return gamma;
}

RandomValues Generator::GeneratorMonteCarlo_Cylinder()
{
    RandomValues cylinderValues;
    cylinderValues.height = GeneratorMonteCarlo_Height();
    cylinderValues.fi = GeneratorMonteCarlo_Fi();
    cylinderValues.teta = GeneratorMonteCarlo_Teta();
    cylinderValues.gamma = GeneratorMonteCarlo_Gamma();
    return cylinderValues;
}

Coordinates Generator::GeneratorMonteCarlo_GVector(Coordinates& coordinates) {// возможно ошибка
    double teta = GeneratorMonteCarlo_Teta();
    double gamma = GeneratorMonteCarlo_Gamma();
    teta = (teta * PI) / 180;
    gamma = (gamma * PI) / 180;
    coordinates.p1 = sin(teta) * cos(gamma);
    coordinates.p2 = sin(teta) * sin(gamma);
    coordinates.p3 = cos(teta);
    return coordinates;
}

void Generator::LookDiskIndexes()
{
    for (Wall* wall : vector) {
            Disk* disk = dynamic_cast<Disk*>(wall); // Попытка приведения типа
            if (disk && disk->name == "Disk") {
                indexVector.push_back(disk->index);
            }
    }
}

bool Generator::IsDiskInVector()
{
    for (int i = 0; i < vector.size(); i++){
        if (vector[i]->name=="Disk"){
            return true;
        }
    } return false;
}

int Generator::GeneratorMonteCarlo_index()
{    // Проверка на пустоту вектора
    if (indexVector.empty() &&(!generator.IsDiskInVector())) {
        throw std::runtime_error("Vector is empty");
    }
    LookDiskIndexes();

    // Статические объекты для генерации случайных чисел
    static std::mt19937 gen(time(nullptr)); // генератор случайных чисел, инициализированный системными часами
    std::uniform_int_distribution<size_t> dist(0, indexVector.size() - 1); // распределение для индексов

    // Выбор случайного индекса и возвращение элемента
    return indexVector[dist(gen)];
}

double Generator::GeneratorMonteCarlo_Point(int index)
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

RandomValues Generator::GeneratorMonteCarlo_Disk()
{
    RandomValues diskValues;
    diskValues.index = GeneratorMonteCarlo_index();
    diskValues.fi = GeneratorMonteCarlo_Fi();
    diskValues.point = GeneratorMonteCarlo_Point(diskValues.index);
    diskValues.teta = GeneratorMonteCarlo_Teta();
    diskValues.gamma = GeneratorMonteCarlo_Gamma();
    return diskValues;
}

double Generator::CylindersArea()
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

double Generator::DiskArea()
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

Coeficients Generator::Distribution()
{
    Coeficients coeficitions;
    double areaCylinders = generator.CylindersArea();
    double areaDisk = generator.DiskArea();
    coeficitions.CylinderCoef = areaCylinders/(areaCylinders + areaDisk);
    coeficitions.DiskCoef = 1 - coeficitions.CylinderCoef;
    return coeficitions;
}


findingCylinder Generator::FindCylinderIndex(double height) {
    findingCylinder coord;
    coord.diff = 0.;
    coord.index = 0;
    for (Wall* wall: vector) {
        Сylinder* cylinder = dynamic_cast<Сylinder*>(wall);
        if (cylinder != nullptr && cylinder->name == "Cylinder") {
            if (height < cylinder->coordinateZ) {
                coord.diff = cylinder->Height - (cylinder->coordinateZ - height);
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
        findingCylinder coord = generator.FindCylinderIndex(other.height);
        Сylinder* cylinder = dynamic_cast<Сylinder*>(vector[coord.index]);
        double x0 = cylinder->radiusOutsideCylinder * cos((other.fi * pi) / 180);
        double y0 = cylinder->radiusOutsideCylinder * sin((other.fi * pi) / 180);
        double z0 = other.height;
        this->x = x0;
        this->y = y0;
        this->z = z0;
        this->index = coord.index;
    } else {
        this->x = other.point * cos((other.fi * pi) / 180);
        this->y = other.point * sin((other.fi * pi) / 180);
        this->z = vector[other.index - 1]->coordinateZ;
        this->index = other.index;
    }
    return *this;
}


void Generator::IntersectionSearch(Coordinates& NewCoordinates, int k)
{
    if (vector[k]->name == "Disk") {
        NewCoordinates = FlightMoleculeDisk(NewCoordinates, k);
        if (NewCoordinates.flag == FOUND) {
            Disk* disk = dynamic_cast<Disk*>(vector[NewCoordinates.index]);
            if (disk->portal) {
                NewCoordinates.flag = EXIT;
                ++exitMolecules;
                return;
            }
            generator.GeneratorMonteCarlo_GVector(NewCoordinates);
            return;
            // я думаю тут сделать чтоб оно возвращало структуру , и если флаг FOUND то выходим из ф-и и возращаем структуру
            //или сделать все по ссылке как ты и хотел,
        }
    } else if (vector[k]->name == "Cylinder") {
        // Необходимо перезаписать индекс именно найденной фигуры
        NewCoordinates = FlightMoleculeCylinder(NewCoordinates, k);
        if (NewCoordinates.flag == FOUND) {
            generator.GeneratorMonteCarlo_GVector(NewCoordinates);// Надо понять перезаписывается ли направляющий вектор
            return;
        }
        return;
    }
}



void Generator::IterationForCylinder(Coordinates& NewCoordinates)
{

    bool breakCondition = false;
    int count = 1;
    while (NewCoordinates.flag == NOT_FOUND) {
        if (breakCondition) {
            break;
        }
        for (int k = NewCoordinates.index; (k < NewCoordinates.index + count + 1) && (k < vector.size()); ++k) {// Нашел ошибку здесь
            generator.IntersectionSearch(NewCoordinates, k);
            if (NewCoordinates.flag == EXIT || NewCoordinates.flag == FOUND) {
                return;
            }
        }
        if (NewCoordinates.flag == NOT_FOUND) {
            for (int k = NewCoordinates.index; (k > NewCoordinates.index - count - 1) && (k > -1); --k) {
                generator.IntersectionSearch(NewCoordinates, k);
                if (NewCoordinates.flag == EXIT || NewCoordinates.flag == FOUND) {
                    return;
                }
            }
        }
        if (count == vector.size()) {
            breakCondition = true;
        }
    }
}


void Generator::IterationForDisk(Coordinates& NewCoordinates)
{
    Disk* disk = dynamic_cast<Disk*>(vector[NewCoordinates.index]);
    if (disk->location) {
        for (int i = NewCoordinates.index + 1; i < vector.size(); ++i) {
            generator.IntersectionSearch(NewCoordinates, i);
            if (NewCoordinates.flag == EXIT || NewCoordinates.flag == FOUND) {
                return;
            }
        }
    } else {
        for (int i = NewCoordinates.index - 1; i > 0; --i) {
            generator.IntersectionSearch(NewCoordinates, i);
            if (NewCoordinates.flag == EXIT || NewCoordinates.flag == FOUND) {
                return;
            }
        }
    }
}


void Generator::Iteration(Coordinates& NewCoordinates, int iteration) {

    int j = 0;
    while (j < iteration - 1) {
        if (vector[NewCoordinates.index]->name == "Disk") {// Здесь может быть SIGABRT, проверено, надо исправить
             IterationForDisk(NewCoordinates);
             if (NewCoordinates.flag == EXIT) {return;}
             j++;
             NewCoordinates.flag = NOT_FOUND;
        } else if (vector[NewCoordinates.index]->name == "Cylinder") {
             IterationForCylinder(NewCoordinates);
             if (NewCoordinates.flag == EXIT) {return;}
             j++;
             NewCoordinates.flag = NOT_FOUND;
        }
    }

}


int Generator::Core(int countMoleculs, int iteration)
{
    generator.CreatingPortal();
    RandomValues rand;
    Coeficients coeficionts = {};
    Coordinates NewCoordinates = {};
    coeficionts = generator.Distribution();//убрал лишние вызовы

    for (int i = 0; i < countMoleculs * coeficionts.CylinderCoef; ++i) {
        rand = generator.GeneratorMonteCarlo_Cylinder();
        NewCoordinates = rand;
        IterationForCylinder(NewCoordinates);
        if (NewCoordinates.flag == EXIT) {
            NewCoordinates.flag = NOT_FOUND;
            continue;
        } else if (NewCoordinates.flag == FOUND) {
            NewCoordinates.flag = NOT_FOUND;
        }
        generator.Iteration(NewCoordinates, iteration);
    }


    for (int i = 0; i < countMoleculs * coeficionts.DiskCoef; ++i) {
        rand = generator.GeneratorMonteCarlo_Disk();
        NewCoordinates = rand;
        IterationForDisk(NewCoordinates);
        if (NewCoordinates.flag == EXIT) {
            NewCoordinates.flag = NOT_FOUND;
            continue;
        } else if (NewCoordinates.flag == FOUND) {
            NewCoordinates.flag = NOT_FOUND;
        }
        generator.Iteration(NewCoordinates, iteration);
    }
    return exitMolecules;
}


Coordinates Generator::FlightMoleculeDisk(Coordinates& coordinates, int i)
{
    Disk* disk = dynamic_cast<Disk*>(vector[i]);
    double t = (disk->coordinateZ - coordinates.z) / coordinates.p3;
    double x_0 = coordinates.x + coordinates.p1 * t;
    double y_0 = coordinates.y + coordinates.p2 * t;
    if (sqrt(pow(x_0, 2) + pow(y_0, 2)) > disk->radiusInsideDisk &&
        sqrt(pow(x_0, 2) + pow(y_0, 2)) < disk->radiusOutsideDisk) {
        coordinates.x = x_0;
        coordinates.y = y_0;
        coordinates.z = disk->coordinateZ;
        coordinates.flag = FOUND;
        coordinates.index = i;
    } else {
        coordinates.flag = NOT_FOUND;
    }
    return coordinates;
}


bool Generator::CheckForBoundCondition(Coordinates coordinates, Сylinder *cylinder) {
    //std::cout << "coordinate z: " << cylinder->coordinateZ << std::endl;
    return (coordinates.z < cylinder->coordinateZ - cylinder->Height) || (coordinates.z > cylinder->coordinateZ);
}

Coordinates Generator::FlightMoleculeCylinder(Coordinates& coordinates, int i) { // ИСПРАВИТЬ СРОЧНО!!!!!!! НЕТ ПРОВЕРКИ НА ГРАНИЧНЫЕ УСЛОВИЯ
    //findingCylinder coord = FindCylinderIndex(coordinates.z);
    Coordinates pointBegin;
    Coordinates pointEnd;
    Сylinder* cylinder = dynamic_cast<Сylinder*>(vector[i]);
    double A = pow(coordinates.p1, 2) + pow(coordinates.p2, 2);
    double B = 2*(coordinates.x * coordinates.p1 + coordinates.y * coordinates.p2);
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

        if (CheckForBoundCondition(pointBegin, cylinder)) {
            pointBegin.flag = NOT_FOUND;
        } else {
            pointBegin.flag = FOUND;
        }

        if (CheckForBoundCondition(pointEnd, cylinder)) {
            pointEnd.flag = NOT_FOUND;
        } else {
            pointEnd.flag = FOUND;
        }

        if (pointBegin.flag == FOUND) {
            if (!(t1 == 0. || (t1 > -1e-12 && t1 < 1e-12))) {
                findingCylinder coord = generator.FindCylinderIndex(pointBegin.z);
                pointBegin.index = coord.index;
                return pointBegin;
            }
        }

        if (pointEnd.flag == FOUND) {
            if (!(t2 == 0. || (t2 > -1e-12 && t2 < 1e-12))) {
                findingCylinder coord = generator.FindCylinderIndex(pointEnd.z);
                pointEnd.index = coord.index;
                return pointEnd;
            }
        }

        if (pointBegin.flag == NOT_FOUND || pointEnd.flag == NOT_FOUND) {
            coordinates.flag = NOT_FOUND;
            return coordinates;
        }
    } else if (Dis < 0) {
        throw std::exception();
        //coordinates.flag = NOT_FOUND;
        //return coordinates;
    } else {
        coordinates.flag = NOT_FOUND;
        return coordinates;
    }

}
