#ifndef IGLBINDABLE_H
#define IGLBINDABLE_H

#include "Bang.h"

#include <GL/glew.h>
#include <iostream>
#include <stack>

#include "Map.h"

class IGLBindable
{

private:
    /**
     * @brief latestBoundId  {GL target: stack of GL_bound_id}
     * There can be several targets for the same bindable, and they must
     * be separated in this map.
     */
    mutable Map< GLenum, std::stack<GLint> > m_target_To_BoundIdsStack;

protected:
    IGLBindable();

    /**
     * @brief PreBind. Must use before GL Bindings to keep track of the context. It pushes
     *        into a stack the current binding for bindTarget, to be able to restore it
     *        later using PreUnBind.
     * @param bindTarget glGet target to be saved.
     *        For example: GL_ARRAY_BUFFER or GL_VERTEX_ARRAY_BINDING
     */
    void PreBind(GLenum bindTarget) const;

    /**
     * @brief PreUnBind. Must use when unbinding GL objects. This will return the id
     *        of the last object bound to bindTarget. Will return 0 if no object existed.
     * @param bindTarget glGet target to be unbound.
     * @return Returns the id that must be bound to openGL to restore the context.
     */
    GLint PreUnBind(GLenum bindTarget) const;

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;
};

#endif // IGLBINDABLE_H
