#ifndef VISUALFITTING_H
#define VISUALFITTING_H

#include <QtWidgets/QMainWindow>
#include "ui_visualfitting.h"

class visualFitting : public QMainWindow
{
	Q_OBJECT

public:
	visualFitting(QWidget *parent = 0);
	~visualFitting();

private:
	Ui::visualFittingClass ui;
};

#endif // VISUALFITTING_H
