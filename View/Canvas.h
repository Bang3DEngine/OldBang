#ifndef CANVAS_H
#define CANVAS_H

#include "../Domain/Shader.h"
#include <QGLWidget>

class Canvas : public QGLWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = 0);

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

signals:

public slots:

};

#endif // CANVAS_H
