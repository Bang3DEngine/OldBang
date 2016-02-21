/********************************************************************************
** Form generated from reading UI file 'WindowMain.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOWMAIN_H
#define UI_WINDOWMAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "Canvas.h"

QT_BEGIN_NAMESPACE

class Ui_WindowMain
{
public:
    QAction *actionOpen_project;
    QAction *actionSave_project;
    QAction *actionMaterial;
    QAction *actionMesh;
    QAction *actionShaderProgram;
    QAction *actionTexture2D;
    QAction *actionProject_Settings;
    QAction *actionCreate_from_prefab;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    Canvas *canvas;
    QMenuBar *menubar;
    QMenu *menuAssets;
    QMenu *menuCreate;
    QMenu *menuEntity;
    QMenu *menuProject;
    QStatusBar *statusbar;
    QDockWidget *dockHierarchy;
    QWidget *dockWidgetContents_4;
    QVBoxLayout *verticalLayout_8;
    QVBoxLayout *verticalLayout_4;
    QTreeView *widgetTreeHierarchy_2;
    QDockWidget *dockInspector;
    QWidget *dockWidgetContents_7;
    QVBoxLayout *verticalLayout_9;
    QVBoxLayout *verticalLayout_2;
    QListWidget *widgetListInspector;
    QDockWidget *dockExplorer;
    QWidget *dockWidgetContents_5;
    QVBoxLayout *verticalLayout_7;
    QSplitter *splitter;
    QPushButton *buttonCreatePrefab;
    QTreeView *widgetTreeExplorer;

    void setupUi(QMainWindow *WindowMain)
    {
        if (WindowMain->objectName().isEmpty())
            WindowMain->setObjectName(QString::fromUtf8("WindowMain"));
        WindowMain->resize(718, 592);
        actionOpen_project = new QAction(WindowMain);
        actionOpen_project->setObjectName(QString::fromUtf8("actionOpen_project"));
        actionSave_project = new QAction(WindowMain);
        actionSave_project->setObjectName(QString::fromUtf8("actionSave_project"));
        actionMaterial = new QAction(WindowMain);
        actionMaterial->setObjectName(QString::fromUtf8("actionMaterial"));
        actionMesh = new QAction(WindowMain);
        actionMesh->setObjectName(QString::fromUtf8("actionMesh"));
        actionShaderProgram = new QAction(WindowMain);
        actionShaderProgram->setObjectName(QString::fromUtf8("actionShaderProgram"));
        actionTexture2D = new QAction(WindowMain);
        actionTexture2D->setObjectName(QString::fromUtf8("actionTexture2D"));
        actionProject_Settings = new QAction(WindowMain);
        actionProject_Settings->setObjectName(QString::fromUtf8("actionProject_Settings"));
        actionCreate_from_prefab = new QAction(WindowMain);
        actionCreate_from_prefab->setObjectName(QString::fromUtf8("actionCreate_from_prefab"));
        centralwidget = new QWidget(WindowMain);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        canvas = new Canvas(centralwidget);
        canvas->setObjectName(QString::fromUtf8("canvas"));
        canvas->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(canvas->sizePolicy().hasHeightForWidth());
        canvas->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(canvas);

        WindowMain->setCentralWidget(centralwidget);
        menubar = new QMenuBar(WindowMain);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 718, 25));
        menubar->setNativeMenuBar(false);
        menuAssets = new QMenu(menubar);
        menuAssets->setObjectName(QString::fromUtf8("menuAssets"));
        menuCreate = new QMenu(menuAssets);
        menuCreate->setObjectName(QString::fromUtf8("menuCreate"));
        menuEntity = new QMenu(menubar);
        menuEntity->setObjectName(QString::fromUtf8("menuEntity"));
        menuProject = new QMenu(menubar);
        menuProject->setObjectName(QString::fromUtf8("menuProject"));
        WindowMain->setMenuBar(menubar);
        statusbar = new QStatusBar(WindowMain);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        WindowMain->setStatusBar(statusbar);
        dockHierarchy = new QDockWidget(WindowMain);
        dockHierarchy->setObjectName(QString::fromUtf8("dockHierarchy"));
        dockHierarchy->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockHierarchy->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QString::fromUtf8("dockWidgetContents_4"));
        verticalLayout_8 = new QVBoxLayout(dockWidgetContents_4);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        widgetTreeHierarchy_2 = new QTreeView(dockWidgetContents_4);
        widgetTreeHierarchy_2->setObjectName(QString::fromUtf8("widgetTreeHierarchy_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widgetTreeHierarchy_2->sizePolicy().hasHeightForWidth());
        widgetTreeHierarchy_2->setSizePolicy(sizePolicy1);
        widgetTreeHierarchy_2->setAcceptDrops(true);

        verticalLayout_4->addWidget(widgetTreeHierarchy_2);


        verticalLayout_8->addLayout(verticalLayout_4);

        dockHierarchy->setWidget(dockWidgetContents_4);
        WindowMain->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockHierarchy);
        dockInspector = new QDockWidget(WindowMain);
        dockInspector->setObjectName(QString::fromUtf8("dockInspector"));
        dockInspector->setFloating(false);
        dockInspector->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockInspector->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidgetContents_7 = new QWidget();
        dockWidgetContents_7->setObjectName(QString::fromUtf8("dockWidgetContents_7"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(dockWidgetContents_7->sizePolicy().hasHeightForWidth());
        dockWidgetContents_7->setSizePolicy(sizePolicy2);
        dockWidgetContents_7->setContextMenuPolicy(Qt::DefaultContextMenu);
        verticalLayout_9 = new QVBoxLayout(dockWidgetContents_7);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        widgetListInspector = new QListWidget(dockWidgetContents_7);
        widgetListInspector->setObjectName(QString::fromUtf8("widgetListInspector"));

        verticalLayout_2->addWidget(widgetListInspector);

        verticalLayout_2->setStretch(0, 1);

        verticalLayout_9->addLayout(verticalLayout_2);

        dockInspector->setWidget(dockWidgetContents_7);
        WindowMain->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockInspector);
        dockExplorer = new QDockWidget(WindowMain);
        dockExplorer->setObjectName(QString::fromUtf8("dockExplorer"));
        dockExplorer->setMinimumSize(QSize(420, 121));
        dockExplorer->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetContents_5 = new QWidget();
        dockWidgetContents_5->setObjectName(QString::fromUtf8("dockWidgetContents_5"));
        verticalLayout_7 = new QVBoxLayout(dockWidgetContents_5);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        splitter = new QSplitter(dockWidgetContents_5);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        sizePolicy2.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy2);
        splitter->setOrientation(Qt::Horizontal);
        splitter->setChildrenCollapsible(true);
        buttonCreatePrefab = new QPushButton(splitter);
        buttonCreatePrefab->setObjectName(QString::fromUtf8("buttonCreatePrefab"));
        sizePolicy2.setHeightForWidth(buttonCreatePrefab->sizePolicy().hasHeightForWidth());
        buttonCreatePrefab->setSizePolicy(sizePolicy2);
        buttonCreatePrefab->setDefault(true);
        splitter->addWidget(buttonCreatePrefab);
        widgetTreeExplorer = new QTreeView(splitter);
        widgetTreeExplorer->setObjectName(QString::fromUtf8("widgetTreeExplorer"));
        sizePolicy2.setHeightForWidth(widgetTreeExplorer->sizePolicy().hasHeightForWidth());
        widgetTreeExplorer->setSizePolicy(sizePolicy2);
        widgetTreeExplorer->setAcceptDrops(true);
        splitter->addWidget(widgetTreeExplorer);

        verticalLayout_7->addWidget(splitter);

        dockExplorer->setWidget(dockWidgetContents_5);
        WindowMain->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockExplorer);

        menubar->addAction(menuProject->menuAction());
        menubar->addAction(menuEntity->menuAction());
        menubar->addAction(menuAssets->menuAction());
        menuAssets->addAction(menuCreate->menuAction());
        menuCreate->addAction(actionMaterial);
        menuCreate->addAction(actionMesh);
        menuCreate->addAction(actionShaderProgram);
        menuCreate->addAction(actionTexture2D);
        menuEntity->addAction(actionCreate_from_prefab);
        menuProject->addAction(actionOpen_project);
        menuProject->addAction(actionSave_project);
        menuProject->addAction(actionProject_Settings);

        retranslateUi(WindowMain);
        QObject::connect(buttonCreatePrefab, SIGNAL(clicked()), canvas, SLOT(OnTopKekPressed()));

        QMetaObject::connectSlotsByName(WindowMain);
    } // setupUi

    void retranslateUi(QMainWindow *WindowMain)
    {
        WindowMain->setWindowTitle(QApplication::translate("WindowMain", "Bang Editor", 0, QApplication::UnicodeUTF8));
        actionOpen_project->setText(QApplication::translate("WindowMain", "Open project", 0, QApplication::UnicodeUTF8));
        actionSave_project->setText(QApplication::translate("WindowMain", "Save project", 0, QApplication::UnicodeUTF8));
        actionMaterial->setText(QApplication::translate("WindowMain", "Material", 0, QApplication::UnicodeUTF8));
        actionMesh->setText(QApplication::translate("WindowMain", "Mesh", 0, QApplication::UnicodeUTF8));
        actionShaderProgram->setText(QApplication::translate("WindowMain", "ShaderProgram", 0, QApplication::UnicodeUTF8));
        actionTexture2D->setText(QApplication::translate("WindowMain", "Texture2D", 0, QApplication::UnicodeUTF8));
        actionProject_Settings->setText(QApplication::translate("WindowMain", "Project Settings", 0, QApplication::UnicodeUTF8));
        actionCreate_from_prefab->setText(QApplication::translate("WindowMain", "Create from prefab", 0, QApplication::UnicodeUTF8));
        menuAssets->setTitle(QApplication::translate("WindowMain", "Assets", 0, QApplication::UnicodeUTF8));
        menuCreate->setTitle(QApplication::translate("WindowMain", "Create", 0, QApplication::UnicodeUTF8));
        menuEntity->setTitle(QApplication::translate("WindowMain", "Entities", 0, QApplication::UnicodeUTF8));
        menuProject->setTitle(QApplication::translate("WindowMain", "Project", 0, QApplication::UnicodeUTF8));
        dockHierarchy->setWindowTitle(QApplication::translate("WindowMain", "Hierarchy", 0, QApplication::UnicodeUTF8));
        dockInspector->setWindowTitle(QApplication::translate("WindowMain", "Inspector", 0, QApplication::UnicodeUTF8));
        dockExplorer->setWindowTitle(QApplication::translate("WindowMain", "Explorer", 0, QApplication::UnicodeUTF8));
        buttonCreatePrefab->setText(QApplication::translate("WindowMain", "Test: Create prefab", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WindowMain: public Ui_WindowMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOWMAIN_H
