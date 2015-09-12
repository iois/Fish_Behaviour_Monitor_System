#pragma once

#include <QPushButton>

class QLineEdit;
class QString;

class Button_in_LineEdit : public QPushButton
{
public:
	Button_in_LineEdit(const QString &text, QLineEdit *edit);
	~Button_in_LineEdit();
};

