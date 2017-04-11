#ifndef BANG_H
#define BANG_H

typedef unsigned int  uint;
typedef unsigned char byte;

#define DELETE    (x) if (x) { delete   x; }
#define DELETE_ARR(x) if (x) { delete[] x; }

#endif // BANG_H
