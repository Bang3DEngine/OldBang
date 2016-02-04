#ifndef IDABLE_H
#define IDABLE_H

class Idable
{
protected:
    Idable() {}
    GLuint id;

public:
    GLuint GetId() { return id; }
};

#endif // IDABLE_H
