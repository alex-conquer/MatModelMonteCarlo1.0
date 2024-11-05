#include "button.h"

Button::Button(const QString& text, QWidget* parent): QPushButton(parent) {
    setText(text);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

QSize Button::sizeHint() const {
    QSize size = QPushButton::sizeHint();
    size.rheight() += 30;
    size.rwidth() = qMax(size.width(), size.height());
    return size;
}
