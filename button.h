#ifndef BUTTON_H
#define BUTTON_H
#include <QPushButton>

class Button: public QPushButton {
    Q_OBJECT
public:
    explicit Button(const QString& text, QWidget* parent = nullptr);
    virtual QSize sizeHint() const override;

};

#endif // BUTTON_H
