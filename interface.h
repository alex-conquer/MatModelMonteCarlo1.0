#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QMainWindow>
#include <QDialog>
#include "button.h"

class Interface: public QWidget {

    Q_OBJECT
public:
    Interface();
    QComboBox* dropdown = new QComboBox();
    bool isBuildingCorrectly(double val1, double val2, QString selected_text, QWidget& windowError);
    void createFinalWindow(double exitMolecules, double amount_mol);

private:

    QString    m_pending_operation;
    QLineEdit* m_first_display_up;
    QLineEdit* m_first_display_down;
    QLabel*    m_figure;
    QLabel*    m_first_label;
    QLabel*    m_second_label;
    QLabel* l_amount_of_iteration;
    QLabel* l_amount_of_molecules;
    QLabel* l_percentage_of_ejected_molecules;
    QLineEdit* e_amount_of_iteration;
    QLineEdit* e_amount_of_molecules;
    QLineEdit* e_percentage_of_ejected_molecules;
    Button* b_back;
    Button* b_next;
    Button* b_ok;

private slots:
    void readingValues();
    void calculateOfPrecentageMolecules();
    void preCalculate();
};

class CenteredWindow: public QWidget {
public:
    CenteredWindow(QWidget* parent = nullptr);
};




#endif // INTERFACE_H
