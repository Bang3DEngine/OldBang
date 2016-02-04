#ifndef CANVAS_H
#define CANVAS_H

#include "../Domain/VAO.h"
#include "../Domain/Shader.h"
#include "../Domain/ShaderProgram.h"
#include "../Domain/MeshRenderer.h"

#include <vector>
#include <string>
#include <QGLWidget>

class Canvas : public QGLWidget
{
    Q_OBJECT

private:
    class DrawRequest
    {
    public:
        const MeshRenderer *renderer;
        const Transform *transform;
        DrawRequest(const MeshRenderer *r,
                    const Transform *t) : renderer(r), transform(t) { }
    };

    mutable std::vector<DrawRequest> drawRequests;

public:

    explicit Canvas(QWidget *parent = 0);

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void Draw(const MeshRenderer *renderer, const Transform *t) const;

signals:

public slots:

};

#endif // CANVAS_H
