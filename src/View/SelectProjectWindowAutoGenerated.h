/********************************************************************************
** Form generated from reading UI file 'SelectProjectWindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTPROJECTWINDOW_H
#define UI_SELECTPROJECTWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SelectProjectWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *BangLogo;
    QSpacerItem *verticalSpacer;
    QFrame *line;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *buttonCreateNewProject;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonLoadProject;

    void setupUi(QMainWindow *SelectProjectWindow)
    {
        if (SelectProjectWindow->objectName().isEmpty())
            SelectProjectWindow->setObjectName(QString::fromUtf8("SelectProjectWindow"));
        SelectProjectWindow->resize(421, 244);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Bang/BangIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
        SelectProjectWindow->setWindowIcon(icon);
        SelectProjectWindow->setWindowOpacity(1);
        centralwidget = new QWidget(SelectProjectWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        BangLogo = new QLabel(centralwidget);
        BangLogo->setObjectName(QString::fromUtf8("BangLogo"));
        BangLogo->setMaximumSize(QSize(150, 150));
        BangLogo->setPixmap(QPixmap(QString::fromUtf8(":/Bang/BangLogo.png")));
        BangLogo->setScaledContents(true);
        BangLogo->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(BangLogo);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        line = new QFrame(centralwidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        buttonCreateNewProject = new QPushButton(centralwidget);
        buttonCreateNewProject->setObjectName(QString::fromUtf8("buttonCreateNewProject"));

        horizontalLayout_2->addWidget(buttonCreateNewProject);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        buttonLoadProject = new QPushButton(centralwidget);
        buttonLoadProject->setObjectName(QString::fromUtf8("buttonLoadProject"));

        horizontalLayout_2->addWidget(buttonLoadProject);


        verticalLayout->addLayout(horizontalLayout_2);

        SelectProjectWindow->setCentralWidget(centralwidget);

        retranslateUi(SelectProjectWindow);

        QMetaObject::connectSlotsByName(SelectProjectWindow);
    } // setupUi

    void retranslateUi(QMainWindow *SelectProjectWindow)
    {
        SelectProjectWindow->setWindowTitle(QApplication::translate("SelectProjectWindow", "Bang - Select project", 0, QApplication::UnicodeUTF8));
        BangLogo->setText(QString());
        buttonCreateNewProject->setText(QApplication::translate("SelectProjectWindow", "Create new project", 0, QApplication::UnicodeUTF8));
        buttonLoadProject->setText(QApplication::translate("SelectProjectWindow", "Load project", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SelectProjectWindow: public Ui_SelectProjectWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTPROJECTWINDOW_H
