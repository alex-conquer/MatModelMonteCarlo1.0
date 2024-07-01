#ifndef WALL_H
#define WALL_H

#include <vector>
#include <stack>
#include <iostream>
#include <QObject>
#include <QComboBox>
#include "interface.h"

enum {
    EXIT = -1,
    NOT_FOUND = 0,
    FOUND = 1
};

class Generator;

struct guidingVector {
    double p1;
    double p2;
    double p3;
};

struct RandomValues {
    int index = 0;
    double height = 0.;
    double fi;
    double teta;
    double gamma;
    double point = 0.;
};


struct FindingCylinder {
    int index = 0;
    double diff;
};

struct Coeficients {
    double CylinderCoef = 0;
    double DiskCoef = 0;
};

struct Coordinates {
    Coordinates& operator=(const RandomValues& other);
    int flag = 0;
    double p1 = 0.;
    double p2 = 0.;
    double p3 = 0.;
    double x;
    double y;
    double z;
    int index;
};

class Wall: public QObject {

    Q_OBJECT
public:
    static int indexNumber;
    static double coordinateZ;
    QString name = "Wall";
    Wall();
    virtual ~Wall();
    friend double GeneratorMonteCarlo();
};

class Disk : public Wall {

public:
    double radiusInsideDisk;
    double radiusOutsideDisk;
    Disk(double radiusOutside, double radiusInside); // объявляем конструктор
    ~Disk(); // объявляем деструктор
    int index;
    bool portal = false;// true если да, false если нет
    bool location; // true если сверху, false если снизу, мои изменения
};



class Сylinder : public Wall {

public:

    double radiusOutsideCylinder;
    double Height;
    Сylinder(double radiusOutside, double height);
    ~Сylinder();
    int index;
};

template <typename T>
class Stack {
private:
  std::stack<T*> elements;
public:
  Stack();
  void push(T* value){
      elements.push(value);
  }

  std::size_t Size() {
      return elements.size();
  }

  T* top() const {
      return elements.top();
  }

  void pop() {
      elements.pop();
  }

  bool empty() const {
      return elements.empty();
  }

  ~Stack() {
      while (!elements.empty()){
          delete elements.top();
          elements.pop();
      }
  }
};



#endif // WALL_H
