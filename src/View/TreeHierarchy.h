#ifndef TREEHIERARCHY_H
#define TREEHIERARCHY_H

#include <GL/glew.h>

#include <list>
#include <vector>
#include <string>
#include <chrono>
#include <QTimer>
#include <QTreeWidget>
#include <QApplication>

#include "Stage.h"

class TreeHierarchy : public QTreeWidget
{
public:
    explicit TreeHierarchy(QWidget *parent = 0);
    void Fill(Stage *currentStage);
};

#endif // TREEHIERARCHY_H
