/********************************************************************************
** Form generated from reading UI file 'visualfitting.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VISUALFITTING_H
#define UI_VISUALFITTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_visualFittingClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *visualFittingClass)
    {
        if (visualFittingClass->objectName().isEmpty())
            visualFittingClass->setObjectName(QStringLiteral("visualFittingClass"));
        visualFittingClass->resize(600, 400);
        menuBar = new QMenuBar(visualFittingClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        visualFittingClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(visualFittingClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        visualFittingClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(visualFittingClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        visualFittingClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(visualFittingClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        visualFittingClass->setStatusBar(statusBar);

        retranslateUi(visualFittingClass);

        QMetaObject::connectSlotsByName(visualFittingClass);
    } // setupUi

    void retranslateUi(QMainWindow *visualFittingClass)
    {
        visualFittingClass->setWindowTitle(QApplication::translate("visualFittingClass", "visualFitting", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class visualFittingClass: public Ui_visualFittingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VISUALFITTING_H
