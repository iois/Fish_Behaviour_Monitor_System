#include "Button_in_LineEdit.h"

#include<QtWidgets\qlineedit.h>
#include<QtWidgets\qlayout.h>



Button_in_LineEdit::Button_in_LineEdit(const QString &text, QLineEdit *edit) : QPushButton(text, edit)
{ 
	QSize size = QSize(40, edit->sizeHint().height());

	setMinimumSize(size);

	setMaximumSize(size); // ���ð�ť�Ĵ�СΪͼƬ�Ĵ�С

	setFocusPolicy(Qt::NoFocus); // �õ�����ʱ������ʾ���߿�

	setFlat(true);

	setText(text);

	setCursor(QCursor(Qt::PointingHandCursor));


	QHBoxLayout *buttonLayout = new QHBoxLayout();

	buttonLayout->setContentsMargins(0, 0, 0, 0);

	buttonLayout->addStretch();

	buttonLayout->addWidget(this);

	edit->setLayout(buttonLayout);


	// ������������ļ�����������������������ڱ������ڰ�ť��

	edit->setTextMargins(0, 1, size.width(), 1);


	// ����style sheet


	// Ϊ�˷������, ��� style sheet д��������, ʵ�ʹ�����Ӧ�÷ŵ�ר�õ�style sheet��, �����޸�

	QString qss = QString(".SearchButton {background: gray; color: white; border: 1 solid gray;min-width: 40px;}")

		+ QString(".SearchButton:hover {background: black; color: white; border: 1 solid black;}")

		+ QString(".SearchButton:pressed {background: white;color: black;}");

	setStyleSheet(qss);
}


Button_in_LineEdit::~Button_in_LineEdit()
{
}
