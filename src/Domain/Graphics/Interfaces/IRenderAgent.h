#ifndef IRENDERAGENT_H
#define IRENDERAGENT_H

class GameObject;
class ShaderProgram;
class IRenderAgent
{
public:
    virtual void OnRenderingStarts(GameObject *go,  ShaderProgram *sp) {}
    virtual void OnJustBeforeRendering(GameObject *go, ShaderProgram *sp) {}
    virtual void OnJustAfterRendering(GameObject *go, ShaderProgram *sp) {}
    virtual void OnRenderingEnds(GameObject *go, ShaderProgram *sp) {}

protected:
    IRenderAgent() {}
};

#endif // IRENDERAGENT_H

