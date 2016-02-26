/********************************************************************************
** Form generated from reading UI file 'WindowMain.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOWMAIN_H
#define UI_WINDOWMAIN_H

#include <ListInspector.h>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Canvas.h"
#include "TreeHierarchy.h"
#include "WindowEventManager.h"

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
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_3;
    WindowEventManager *windowEventManager;
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
    TreeHierarchy *widgetTreeHierarchy;
    QDockWidget *dockInspector;
    QWidget *dockWidgetContents_7;
    QVBoxLayout *verticalLayout_9;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelInspectorEntityName;
    ListInspector *widgetListInspector;
    QDockWidget *dockExplorer;
    QWidget *dockWidgetContents_5;
    QVBoxLayout *verticalLayout_7;
    QSplitter *splitter;
    QPushButton *buttonCreatePrefab;
    QTreeView *widgetTreeExplorer;

    void setupUi(QMainWindow *WindowMain)
    {
        if (WindowMain->objectName().isEmpty())
            WindowMain->setObjectName(QStringLiteral("WindowMain"));
        WindowMain->setWindowModality(Qt::NonModal);
        WindowMain->resize(531, 402);
        actionOpen_project = new QAction(WindowMain);
        actionOpen_project->setObjectName(QStringLiteral("actionOpen_project"));
        actionSave_project = new QAction(WindowMain);
        actionSave_project->setObjectName(QStringLiteral("actionSave_project"));
        actionMaterial = new QAction(WindowMain);
        actionMaterial->setObjectName(QStringLiteral("actionMaterial"));
        actionMesh = new QAction(WindowMain);
        actionMesh->setObjectName(QStringLiteral("actionMesh"));
        actionShaderProgram = new QAction(WindowMain);
        actionShaderProgram->setObjectName(QStringLiteral("actionShaderProgram"));
        actionTexture2D = new QAction(WindowMain);
        actionTexture2D->setObjectName(QStringLiteral("actionTexture2D"));
        actionProject_Settings = new QAction(WindowMain);
        actionProject_Settings->setObjectName(QStringLiteral("actionProject_Settings"));
        actionCreate_from_prefab = new QAction(WindowMain);
        actionCreate_from_prefab->setObjectName(QStringLiteral("actionCreate_from_prefab"));
        centralwidget = new QWidget(WindowMain);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        windowEventManager = new WindowEventManager(centralwidget);
        windowEventManager->setObjectName(QStringLiteral("windowEventManager"));

        verticalLayout_3->addWidget(windowEventManager);


        verticalLayout->addLayout(verticalLayout_3);

        canvas = new Canvas(centralwidget);
        canvas->setObjectName(QStringLiteral("canvas"));
        canvas->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(canvas->sizePolicy().hasHeightForWidth());
        canvas->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(canvas);

        WindowMain->setCentralWidget(centralwidget);
        menubar = new QMenuBar(WindowMain);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 531, 25));
        menubar->setNativeMenuBar(false);
        menuAssets = new QMenu(menubar);
        menuAssets->setObjectName(QStringLiteral("menuAssets"));
        menuCreate = new QMenu(menuAssets);
        menuCreate->setObjectName(QStringLiteral("menuCreate"));
        menuEntity = new QMenu(menubar);
        menuEntity->setObjectName(QStringLiteral("menuEntity"));
        menuProject = new QMenu(menubar);
        menuProject->setObjectName(QStringLiteral("menuProject"));
        WindowMain->setMenuBar(menubar);
        statusbar = new QStatusBar(WindowMain);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        WindowMain->setStatusBar(statusbar);
        dockHierarchy = new QDockWidget(WindowMain);
        dockHierarchy->setObjectName(QStringLiteral("dockHierarchy"));
        dockHierarchy->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockHierarchy->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QStringLiteral("dockWidgetContents_4"));
        verticalLayout_8 = new QVBoxLayout(dockWidgetContents_4);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        widgetTreeHierarchy = new TreeHierarchy(dockWidgetContents_4);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        widgetTreeHierarchy->setHeaderItem(__qtreewidgetitem);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem(widgetTreeHierarchy);
        QTreeWidgetItem *__qtreewidgetitem2 = new QTreeWidgetItem(__qtreewidgetitem1);
        new QTreeWidgetItem(__qtreewidgetitem2);
        new QTreeWidgetItem(__qtreewidgetitem1);
        QTreeWidgetItem *__qtreewidgetitem3 = new QTreeWidgetItem(widgetTreeHierarchy);
        QTreeWidgetItem *__qtreewidgetitem4 = new QTreeWidgetItem(__qtreewidgetitem3);
        QTreeWidgetItem *__qtreewidgetitem5 = new QTreeWidgetItem(__qtreewidgetitem4);
        new QTreeWidgetItem(__qtreewidgetitem5);
        new QTreeWidgetItem(__qtreewidgetitem5);
        new QTreeWidgetItem(__qtreewidgetitem4);
        new QTreeWidgetItem(__qtreewidgetitem4);
        widgetTreeHierarchy->setObjectName(QStringLiteral("widgetTreeHierarchy"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widgetTreeHierarchy->sizePolicy().hasHeightForWidth());
        widgetTreeHierarchy->setSizePolicy(sizePolicy1);
        widgetTreeHierarchy->setContextMenuPolicy(Qt::CustomContextMenu);
        widgetTreeHierarchy->setAcceptDrops(true);
        widgetTreeHierarchy->setDragEnabled(true);
        widgetTreeHierarchy->setDragDropMode(QAbstractItemView::DragDrop);
        widgetTreeHierarchy->setDefaultDropAction(Qt::MoveAction);
        widgetTreeHierarchy->setAlternatingRowColors(false);
        widgetTreeHierarchy->setSelectionMode(QAbstractItemView::ExtendedSelection);
        widgetTreeHierarchy->setSelectionBehavior(QAbstractItemView::SelectRows);
        widgetTreeHierarchy->setIndentation(10);
        widgetTreeHierarchy->setRootIsDecorated(true);
        widgetTreeHierarchy->setAnimated(true);
        widgetTreeHierarchy->header()->setVisible(false);

        verticalLayout_4->addWidget(widgetTreeHierarchy);


        verticalLayout_8->addLayout(verticalLayout_4);

        dockHierarchy->setWidget(dockWidgetContents_4);
        WindowMain->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockHierarchy);
        dockInspector = new QDockWidget(WindowMain);
        dockInspector->setObjectName(QStringLiteral("dockInspector"));
        dockInspector->setFloating(false);
        dockInspector->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockInspector->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidgetContents_7 = new QWidget();
        dockWidgetContents_7->setObjectName(QStringLiteral("dockWidgetContents_7"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(dockWidgetContents_7->sizePolicy().hasHeightForWidth());
        dockWidgetContents_7->setSizePolicy(sizePolicy2);
        dockWidgetContents_7->setContextMenuPolicy(Qt::DefaultContextMenu);
        verticalLayout_9 = new QVBoxLayout(dockWidgetContents_7);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        widget = new QWidget(dockWidgetContents_7);
        widget->setObjectName(QStringLiteral("widget"));

        verticalLayout_9->addWidget(widget);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        labelInspectorEntityName = new QLabel(dockWidgetContents_7);
        labelInspectorEntityName->setObjectName(QStringLiteral("labelInspectorEntityName"));

        verticalLayout_2->addWidget(labelInspectorEntityName);

        widgetListInspector = new ListInspector(dockWidgetContents_7);
        new QListWidgetItem(widgetListInspector);
        new QListWidgetItem(widgetListInspector);
        new QListWidgetItem(widgetListInspector);
        new QListWidgetItem(widgetListInspector);
        new QListWidgetItem(widgetListInspector);
        new QListWidgetItem(widgetListInspector);
        new QListWidgetItem(widgetListInspector);
        new QListWidgetItem(widgetListInspector);
        widgetListInspector->setObjectName(QStringLiteral("widgetListInspector"));
        widgetListInspector->setDefaultDropAction(Qt::MoveAction);
        widgetListInspector->setAlternatingRowColors(true);
        widgetListInspector->setMovement(QListView::Snap);
        widgetListInspector->setFlow(QListView::TopToBottom);
        widgetListInspector->setViewMode(QListView::ListMode);

        verticalLayout_2->addWidget(widgetListInspector);

        verticalLayout_2->setStretch(1, 1);

        verticalLayout_9->addLayout(verticalLayout_2);

        dockInspector->setWidget(dockWidgetContents_7);
        WindowMain->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockInspector);
        dockExplorer = new QDockWidget(WindowMain);
        dockExplorer->setObjectName(QStringLiteral("dockExplorer"));
        dockExplorer->setMinimumSize(QSize(420, 130));
        dockExplorer->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContents_5 = new QWidget();
        dockWidgetContents_5->setObjectName(QStringLiteral("dockWidgetContents_5"));
        verticalLayout_7 = new QVBoxLayout(dockWidgetContents_5);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        splitter = new QSplitter(dockWidgetContents_5);
        splitter->setObjectName(QStringLiteral("splitter"));
        sizePolicy2.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy2);
        splitter->setOrientation(Qt::Horizontal);
        splitter->setChildrenCollapsible(true);
        buttonCreatePrefab = new QPushButton(splitter);
        buttonCreatePrefab->setObjectName(QStringLiteral("buttonCreatePrefab"));
        sizePolicy2.setHeightForWidth(buttonCreatePrefab->sizePolicy().hasHeightForWidth());
        buttonCreatePrefab->setSizePolicy(sizePolicy2);
        buttonCreatePrefab->setDefault(true);
        splitter->addWidget(buttonCreatePrefab);
        widgetTreeExplorer = new QTreeView(splitter);
        widgetTreeExplorer->setObjectName(QStringLiteral("widgetTreeExplorer"));
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
        QObject::connect(menubar, SIGNAL(activated(int)), windowEventManager, SLOT(_NotifyMenuBarItemClicked(int)));
        QObject::connect(widgetTreeHierarchy, SIGNAL(customContextMenuRequested(QPoint)), widgetTreeHierarchy, SLOT(OnCustomContextMenuRequested(QPoint)));
        QObject::connect(widgetTreeHierarchy, SIGNAL(itemSelectionChanged()), widgetTreeHierarchy, SLOT(_NotifyHierarchyItemSelectionChanged()));

        QMetaObject::connectSlotsByName(WindowMain);
    } // setupUi

    void retranslateUi(QMainWindow *WindowMain)
    {
        WindowMain->setWindowTitle(QApplication::translate("WindowMain", "Bang Editor", 0));
        actionOpen_project->setText(QApplication::translate("WindowMain", "Open project", 0));
        actionSave_project->setText(QApplication::translate("WindowMain", "Save project", 0));
        actionMaterial->setText(QApplication::translate("WindowMain", "Material", 0));
        actionMesh->setText(QApplication::translate("WindowMain", "Mesh", 0));
        actionShaderProgram->setText(QApplication::translate("WindowMain", "ShaderProgram", 0));
        actionTexture2D->setText(QApplication::translate("WindowMain", "Texture2D", 0));
        actionProject_Settings->setText(QApplication::translate("WindowMain", "Project Settings", 0));
        actionCreate_from_prefab->setText(QApplication::translate("WindowMain", "Create from prefab", 0));
        menuAssets->setTitle(QApplication::translate("WindowMain", "Assets", 0));
        menuCreate->setTitle(QApplication::translate("WindowMain", "Create", 0));
        menuEntity->setTitle(QApplication::translate("WindowMain", "Entities", 0));
        menuProject->setTitle(QApplication::translate("WindowMain", "Project", 0));
        dockHierarchy->setWindowTitle(QApplication::translate("WindowMain", "Hierarchy", 0));

        const bool __sortingEnabled = widgetTreeHierarchy->isSortingEnabled();
        widgetTreeHierarchy->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem = widgetTreeHierarchy->topLevelItem(0);
        ___qtreewidgetitem->setText(0, QApplication::translate("WindowMain", "aaaaaa", 0));
        QTreeWidgetItem *___qtreewidgetitem1 = ___qtreewidgetitem->child(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("WindowMain", "bbbbbb", 0));
        QTreeWidgetItem *___qtreewidgetitem2 = ___qtreewidgetitem1->child(0);
        ___qtreewidgetitem2->setText(0, QApplication::translate("WindowMain", "cccccc", 0));
        QTreeWidgetItem *___qtreewidgetitem3 = ___qtreewidgetitem->child(1);
        ___qtreewidgetitem3->setText(0, QApplication::translate("WindowMain", "dddddd", 0));
        QTreeWidgetItem *___qtreewidgetitem4 = widgetTreeHierarchy->topLevelItem(1);
        ___qtreewidgetitem4->setText(0, QApplication::translate("WindowMain", "xxxxxxxx", 0));
        QTreeWidgetItem *___qtreewidgetitem5 = ___qtreewidgetitem4->child(0);
        ___qtreewidgetitem5->setText(0, QApplication::translate("WindowMain", "wwwwwwwwwwwww", 0));
        QTreeWidgetItem *___qtreewidgetitem6 = ___qtreewidgetitem5->child(0);
        ___qtreewidgetitem6->setText(0, QApplication::translate("WindowMain", "eeeeeeeeeeeeeee", 0));
        QTreeWidgetItem *___qtreewidgetitem7 = ___qtreewidgetitem6->child(0);
        ___qtreewidgetitem7->setText(0, QApplication::translate("WindowMain", "rrrrrrrrrrrrrrrrrr", 0));
        QTreeWidgetItem *___qtreewidgetitem8 = ___qtreewidgetitem6->child(1);
        ___qtreewidgetitem8->setText(0, QApplication::translate("WindowMain", "wwwwwwww", 0));
        QTreeWidgetItem *___qtreewidgetitem9 = ___qtreewidgetitem5->child(1);
        ___qtreewidgetitem9->setText(0, QApplication::translate("WindowMain", "qqqqqqqqqqqq", 0));
        QTreeWidgetItem *___qtreewidgetitem10 = ___qtreewidgetitem5->child(2);
        ___qtreewidgetitem10->setText(0, QApplication::translate("WindowMain", "qqqqqqqqqqqqqssssssssssssss", 0));
        widgetTreeHierarchy->setSortingEnabled(__sortingEnabled);

        dockInspector->setWindowTitle(QApplication::translate("WindowMain", "Inspector", 0));
        labelInspectorEntityName->setText(QApplication::translate("WindowMain", "Entity Name", 0));

        const bool __sortingEnabled1 = widgetListInspector->isSortingEnabled();
        widgetListInspector->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = widgetListInspector->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("WindowMain", "a", 0));
        QListWidgetItem *___qlistwidgetitem1 = widgetListInspector->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("WindowMain", "v", 0));
        QListWidgetItem *___qlistwidgetitem2 = widgetListInspector->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("WindowMain", "f", 0));
        QListWidgetItem *___qlistwidgetitem3 = widgetListInspector->item(3);
        ___qlistwidgetitem3->setText(QApplication::translate("WindowMain", "g", 0));
        QListWidgetItem *___qlistwidgetitem4 = widgetListInspector->item(4);
        ___qlistwidgetitem4->setText(QApplication::translate("WindowMain", "h", 0));
        QListWidgetItem *___qlistwidgetitem5 = widgetListInspector->item(5);
        ___qlistwidgetitem5->setText(QApplication::translate("WindowMain", "New Item", 0));
        QListWidgetItem *___qlistwidgetitem6 = widgetListInspector->item(6);
        ___qlistwidgetitem6->setText(QApplication::translate("WindowMain", "j", 0));
        QListWidgetItem *___qlistwidgetitem7 = widgetListInspector->item(7);
        ___qlistwidgetitem7->setText(QApplication::translate("WindowMain", "j", 0));
        widgetListInspector->setSortingEnabled(__sortingEnabled1);

        dockExplorer->setWindowTitle(QApplication::translate("WindowMain", "Explorer", 0));
        buttonCreatePrefab->setText(QApplication::translate("WindowMain", "Test: Create prefab", 0));
    } // retranslateUi

};

namespace Ui {
    class WindowMain: public Ui_WindowMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOWMAIN_H
