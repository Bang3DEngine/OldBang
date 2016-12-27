#ifndef BANG_H
#define BANG_H

// MACROS

    // NONULL
        #define NONULL(x) do { if (x == nullptr) { return; } } while (0);
    //

    // ASSERT
        #define Assert(mustBeTrue, message, returnAction) do{\
            if (!(mustBeTrue)) {\
                Debug_Error("Assertion failed: " << message);\
                returnAction;\
            }\
        } while (0)
    //

//

#endif // BANG_H
