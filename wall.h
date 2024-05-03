#ifndef WALL_H
#define WALL_H

#include <stack>
#include <iostream>
#include <QObject>
#include <QComboBox>

class Wall {

public:
    double radiusInside;
    double radiusOutside;
    double Height;
    static int indexNumber;
    static double coordinateZ;
    const QString name = "Wall";
    Wall();
    ~Wall();
};

class Disk : public Wall {

public:
    double radiusInside;
    double radiusOutside;
    Disk(double radiusOutside, double radiusInside); // объявляем конструктор
    ~Disk(); // объявляем деструктор
    const QString name = "Disk";
};



class Сylinder : public Wall {

public:

    double radiusOutside;
    double Height;
    Сylinder(double radiusOutside, double height);
    ~Сylinder();
    const QString name = "Cylinder";
};

template <typename T>
class Stack {
private:
  std::stack<T*> elements;
public:
  void push( T* value){
      elements.push(value);
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

class Create3DModel : public QObject {
    Q_OBJECT


};

#endif // WALL_H
