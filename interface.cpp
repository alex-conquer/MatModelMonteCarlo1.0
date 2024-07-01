#include "interface.h"
#include <QGridLayout>
#include "button.h"
#include "wall.h"
#include <QDebug>
#include <QMessageBox>
#include "exception.h"
#include "generator.h"
#include "calculate.h"

std::stack<Wall*> stack;
QVector<Wall*> vector;
extern Calculate calcul;
class Test;
Wall wall;



Interface::Interface() {

    m_first_display_up   = new QLineEdit;
    m_first_display_down = new QLineEdit;

    m_figure       = new QLabel;
    m_first_label  = new QLabel;
    m_second_label = new QLabel;

    m_first_display_up->setMaxLength(15);
    m_first_display_down->setMaxLength(15);
    QFont font = m_first_display_up->font();
    font.setPointSize(font.pointSize() + 8);
    m_first_display_up->setFont(font);
    m_first_display_down->setFont(font);

    QGridLayout* main_layout = new QGridLayout;
    setLayout(main_layout);
    main_layout->setSizeConstraint(QLayout::SetFixedSize);


    Button* create = new Button("Create", this);
    Button* calculate = new Button("Calculate", this);


    dropdown->addItem("Cylinder");
    dropdown->addItem("Disk");
    dropdown->setEditable(false);


    main_layout->addWidget(create,               3, 0, 1, 2);
    main_layout->addWidget(dropdown,             0, 1, 1, 1);
    main_layout->addWidget(m_figure,             0, 0, 1, 1);
    main_layout->addWidget(m_first_label,        1, 0, 1, 1);
    main_layout->addWidget(m_second_label,       2, 0, 1, 1);
    main_layout->addWidget(m_first_display_up,   1, 1, 1, 1);
    main_layout->addWidget(m_first_display_down, 2, 1, 1, 1);
    main_layout->addWidget(calculate,            4, 0, 1, 2);


    connect(dropdown, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        QString selected_text = dropdown->itemText(index);
        if (selected_text == "Cylinder") {
            m_first_label->setText("Hight:");
            m_second_label->setText("Radius:");
        } else {
            m_first_label->setText("Outer diameter:");
            m_second_label->setText("Inner diameter:");
        }
    });
    connect(create, &QPushButton::clicked, this, &Interface::readingValues);
    //connect(create, &QPushButton::clicked, this, &Interface::test);
    m_figure->setText("Choose your figure:");
    m_first_label->setText("Hight:");
    m_second_label->setText("Radius:");
    setWindowTitle("Creating figures");


    connect(calculate, &QPushButton::clicked, this, &Interface::preCalculate);


}


void Interface::preCalculate() {
    QWidget* window = new QWidget;

    window->setWindowTitle("Calculas");

    l_amount_of_iteration = new QLabel;
    l_amount_of_molecules = new QLabel;


    e_amount_of_iteration = new QLineEdit;
    e_amount_of_molecules = new QLineEdit;


    b_back = new Button("Cancel", this);
    b_next = new Button("Next", this);

    e_amount_of_iteration->setMaxLength(15);
    e_amount_of_molecules->setMaxLength(15);

    QFont font = e_amount_of_iteration->font();
    font.setPointSize(font.pointSize() + 8);
    e_amount_of_iteration->setFont(font);
    e_amount_of_molecules->setFont(font);

    QGridLayout* new_window_layout = new QGridLayout;
    window->setLayout(new_window_layout);
    new_window_layout->setSizeConstraint(QLayout::SetFixedSize);

    new_window_layout->addWidget(l_amount_of_iteration, 0, 0, 1, 1);
    new_window_layout->addWidget(l_amount_of_molecules, 1, 0, 1, 1);
    new_window_layout->addWidget(e_amount_of_iteration, 0, 1, Qt::AlignRight | Qt::AlignBottom);
    new_window_layout->addWidget(e_amount_of_molecules, 1, 1, Qt::AlignRight | Qt::AlignBottom);
    new_window_layout->addWidget(b_back,                5, 0, 1, 1);
    new_window_layout->addWidget(b_next,                5, 1, Qt::AlignRight | Qt::AlignBottom);


    l_amount_of_iteration->setFixedSize(200, 30);
    l_amount_of_molecules->setFixedSize(200, 30);
    e_amount_of_iteration->setFixedSize(100, 30);
    e_amount_of_molecules->setFixedSize(100, 30);
    b_back->setFixedSize(100, 20);
    b_next->setFixedSize(100, 20);

    l_amount_of_iteration->setText("Set amount of iteration:");
    l_amount_of_molecules->setText("Set amount of molecules:");


    window->show();

    connect(b_back, &QPushButton::clicked, window, &QPushButton::close);
    connect(b_next, &QPushButton::clicked, this, &Interface::calculateOfPrecentageMolecules);
}
// окно которое вылезает после нажатия кнопки Next
void Interface::createFinalWindow(double exitMolecules, double amount_mol) {
    QWidget* window = new QWidget;
    QGridLayout* layout = new QGridLayout;
    double percentageOfEjectedMol = (exitMolecules / amount_mol) * 100;
    l_percentage_of_ejected_molecules = new QLabel;
    e_percentage_of_ejected_molecules = new QLineEdit;
    b_ok = new Button("OK", this);

    window->setLayout(layout);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    e_percentage_of_ejected_molecules->setReadOnly(true);
    e_percentage_of_ejected_molecules->setMaxLength(15);

    QFont font = e_percentage_of_ejected_molecules->font();
    font.setPointSize(font.pointSize() + 8);
    e_percentage_of_ejected_molecules->setFont(font);

    layout->addWidget(l_percentage_of_ejected_molecules, 0, 0, 1, 1);
    layout->addWidget(e_percentage_of_ejected_molecules, 0, 1, 1, 1);
    layout->addWidget(b_ok, 1, 0, 1, 1);

    e_percentage_of_ejected_molecules->setText(QString::number(percentageOfEjectedMol));

    connect(b_ok, &QPushButton::clicked, window, &QPushButton::close);
    window->show();
}

// при нажатии кнопки next вызывается эта функция
void Interface::calculateOfPrecentageMolecules() {
    bool f_cond;
    bool s_cond;
    double exitMolecules;
    double d_amount_mol;
    QWidget errorWindow;
    int amount_iter = e_amount_of_iteration->text().toInt(&f_cond);
    int amount_mol = e_amount_of_molecules->text().toInt(&s_cond);
    // проверка на входные значения с полей
    if (!f_cond || !s_cond) {
        QMessageBox::critical(&errorWindow, "Error", "Some values is invalid");
        return;
    }
    if (stack.empty()) {
        QMessageBox::critical(&errorWindow, "Error", "Stack is empty");
        return;
    }
    // процент молекул должен быть double поэтому приводим к double
    exitMolecules = static_cast<double>(calcul.core(amount_mol, amount_iter));
    d_amount_mol = static_cast<double>(amount_mol);
    // вызываем окно, в котором выводится процент вылетевших молекул
    createFinalWindow(exitMolecules, d_amount_mol);
}

//данная функция считывает данные со строчек ввода и
//в зависимости от того какой "сurrent index" выбирает куда записывать эти данные
//и экземпляр какого класса добавлять
void Interface::readingValues() {
    //Disk* disk = new Disk(34, 0);
    //stack.push(disk);
    //vector.push_back(disk);
    QWidget windowError;
    windowError.move(300, 300);
    int index = dropdown->currentIndex();
    bool ok1, ok2;
    int count = -1;
    double val1 = m_first_display_up->text().toDouble(&ok1);
    double val2 = m_first_display_down->text().toDouble(&ok2);
    if (!ok1 || !ok2) {
        QMessageBox::critical(&windowError, "Error", "Empty line edit");
        return;
    }
    QString selected_text = dropdown->itemText(index);
/*
при нажатии клавиши далее происходит добавление дисков-порталов
в начало и конец модели там где это необходимо*/
    if (isBuildingCorrectly(val1, val2, selected_text, windowError)) {
        if (selected_text == "Cylinder") {
            if (wall.indexNumber == -1){calcul.creatingPortal(val2, true);}
            Сylinder* cylinder = new Сylinder(val2, val1);
            stack.push(cylinder);
            vector.push_back(cylinder);

            count++;
            qDebug() << "Cylinder" << Qt::endl;
        } else if (selected_text == "Disk") {
            if ((wall.indexNumber == -1)&&(val2>0)){calcul.creatingPortal(val2, true);}
            Disk* disk = new Disk(val1, val2);
            // здесь мои изменения
            if (vector.size() == 0) {
                disk->location = true;
            } else {
                Сylinder* cylinder = dynamic_cast<Сylinder*>(vector[vector.size() - 1]);
                if (cylinder->radiusOutsideCylinder == disk->radiusOutsideDisk) {
                    disk->location = false;
                } else {
                    disk->location = true;
                }
            }
            stack.push(disk);
            vector.push_back(disk);
            count++;
            qDebug() << "Disk" << Qt::endl;
        } else {
            QMessageBox::critical(&windowError, "Error", selected_text + "is incorrect figure");
        }
    }
}

bool Interface::isBuildingCorrectly(double val1, double val2, const QString selected_text, QWidget &windowError) {

    if (val1 < 0 || val2 < 0) {
        QMessageBox::critical(&windowError, "Error", "Input value is negative");
        return false;
    }
    if (selected_text == "Disk" && val1 < val2) {
        QMessageBox::critical(&windowError, "Error", "Invalid input value");
        return false;
    }
    if (stack.size() == 0) {
        return true;
    }

    QString name = stack.top()->name;
    Wall* topWall = stack.top();
    if (name == "Cylinder") {
        Сylinder* cylinder = dynamic_cast<Сylinder*>(topWall);
        bool a1 = selected_text == "Cylinder";
        bool a2 = val2 == cylinder->radiusOutsideCylinder;
        if (a1 && a2) {
            return true;
        } else if ((selected_text == "Disk" && val1 == cylinder->radiusOutsideCylinder) ||
            (selected_text == "Disk" && val2 == cylinder->radiusOutsideCylinder)) {
               return true;
        } else {
               QMessageBox::critical(&windowError, "Error", "Invalid figure or parametr");
               return false;
        }
    } else if (name == "Disk") {
        Disk* disk = dynamic_cast<Disk*>(topWall);
        if (selected_text == "Cylinder" && (val2 == disk->radiusInsideDisk || val2 == disk->radiusOutsideDisk)) {
            return true;
        } else {
            QMessageBox::critical(&windowError, "Error", "Invalid figure or parametr");
            return false;
        }
    } else {
        QMessageBox::critical(&windowError, "Error", "Invalid figure");
        return false;
    }
}

void Calculate::creatingPortal(int val2, bool flag)
{   if (flag == true) {
        Disk* disk = new Disk(val2, 0);
        disk->portal = true;
        vector.push_back(disk);
    } else {
        if (vector.back()->name == "Cylinder") {
            Сylinder* cylinder = dynamic_cast<Сylinder*>(vector.back());
            int radius = cylinder->radiusOutsideCylinder;
            Disk* disk = new Disk(radius, 0);
            disk->portal = true;
            vector.push_back(disk);
        } else {
            Disk* disk = dynamic_cast<Disk*>(vector.back());
            int radius = disk->radiusInsideDisk;
            if (!radius) {
                return;
            }
            Disk* diskPortal = new Disk(radius, 0);
            diskPortal->portal = true;
            vector.push_back(diskPortal);
        }
    }
}


