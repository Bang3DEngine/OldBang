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
#include <QtGui/QTableWidget>
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
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QTableWidget *listRecentProjects;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *buttonLoadRecentProject;
    QSpacerItem *verticalSpacer_3;
    QFrame *line_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *buttonCreateNewProject;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonBrowseProject;

    void setupUi(QMainWindow *SelectProjectWindow)
    {
        if (SelectProjectWindow->objectName().isEmpty())
            SelectProjectWindow->setObjectName(QString::fromUtf8("SelectProjectWindow"));
        SelectProjectWindow->resize(421, 452);
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

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_4->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_4);

        listRecentProjects = new QTableWidget(centralwidget);
        if (listRecentProjects->columnCount() < 2)
            listRecentProjects->setColumnCount(2);
        if (listRecentProjects->rowCount() < 10)
            listRecentProjects->setRowCount(10);
        listRecentProjects->setObjectName(QString::fromUtf8("listRecentProjects"));
        listRecentProjects->setFrameShape(QFrame::StyledPanel);
        listRecentProjects->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listRecentProjects->setAutoScroll(true);
        listRecentProjects->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listRecentProjects->setProperty("showDropIndicator", QVariant(false));
        listRecentProjects->setDragDropOverwriteMode(false);
        listRecentProjects->setAlternatingRowColors(false);
        listRecentProjects->setSelectionMode(QAbstractItemView::SingleSelection);
        listRecentProjects->setSelectionBehavior(QAbstractItemView::SelectRows);
        listRecentProjects->setTextElideMode(Qt::ElideLeft);
        listRecentProjects->setShowGrid(true);
        listRecentProjects->setWordWrap(false);
        listRecentProjects->setCornerButtonEnabled(false);
        listRecentProjects->setRowCount(10);
        listRecentProjects->setColumnCount(2);
        listRecentProjects->horizontalHeader()->setVisible(false);
        listRecentProjects->horizontalHeader()->setCascadingSectionResizes(true);
        listRecentProjects->horizontalHeader()->setDefaultSectionSize(100);
        listRecentProjects->horizontalHeader()->setHighlightSections(true);
        listRecentProjects->horizontalHeader()->setMinimumSectionSize(50);
        listRecentProjects->horizontalHeader()->setStretchLastSection(true);
        listRecentProjects->verticalHeader()->setVisible(false);
        listRecentProjects->verticalHeader()->setDefaultSectionSize(30);
        listRecentProjects->verticalHeader()->setHighlightSections(true);

        verticalLayout->addWidget(listRecentProjects);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);

        buttonLoadRecentProject = new QPushButton(centralwidget);
        buttonLoadRecentProject->setObjectName(QString::fromUtf8("buttonLoadRecentProject"));

        horizontalLayout_5->addWidget(buttonLoadRecentProject);


        verticalLayout->addLayout(horizontalLayout_5);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        line_2 = new QFrame(centralwidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        buttonCreateNewProject = new QPushButton(centralwidget);
        buttonCreateNewProject->setObjectName(QString::fromUtf8("buttonCreateNewProject"));

        horizontalLayout_2->addWidget(buttonCreateNewProject);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        buttonBrowseProject = new QPushButton(centralwidget);
        buttonBrowseProject->setObjectName(QString::fromUtf8("buttonBrowseProject"));

        horizontalLayout_2->addWidget(buttonBrowseProject);


        verticalLayout->addLayout(horizontalLayout_2);

        SelectProjectWindow->setCentralWidget(centralwidget);

        retranslateUi(SelectProjectWindow);

        QMetaObject::connectSlotsByName(SelectProjectWindow);
    } // setupUi

    void retranslateUi(QMainWindow *SelectProjectWindow)
    {
        SelectProjectWindow->setWindowTitle(QApplication::translate("SelectProjectWindow", "Bang - Select project", 0, QApplication::UnicodeUTF8));
        BangLogo->setText(QString());
        label->setText(QApplication::translate("SelectProjectWindow", "Recent Projects", 0, QApplication::UnicodeUTF8));
        buttonLoadRecentProject->setText(QApplication::translate("SelectProjectWindow", "Load", 0, QApplication::UnicodeUTF8));
        buttonCreateNewProject->setText(QApplication::translate("SelectProjectWindow", "New project", 0, QApplication::UnicodeUTF8));
        buttonBrowseProject->setText(QApplication::translate("SelectProjectWindow", "Browse project...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SelectProjectWindow: public Ui_SelectProjectWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTPROJECTWINDOW_H
