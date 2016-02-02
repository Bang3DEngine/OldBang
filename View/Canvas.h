#ifndef CANVAS_H
#define CANVAS_H

#include <QGLWidget>

class Canvas : public QGLWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = 0);

    void initializeGL()
    {
        // Set up the rendering context, define display lists etc.:
        glClearColor(0.0, 1.0, 0.0, 1.0);
        glEnable(GL_DEPTH_TEST);
    }

    void resizeGL(int w, int h)
    {
        glViewport(0, 0, (GLint)w, (GLint)h);
    }

    void paintGL()
    {
        // draw the scene:
        glClear(GL_COLOR_BUFFER_BIT);
    }
signals:

public slots:

};

#endif // CANVAS_H
