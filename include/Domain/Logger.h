#ifndef LOGGER_H
#define LOGGER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "glm/gtc/quaternion.hpp"
#include "glm/glm.hpp"
#include "Time.h"

//OSTREAM OPERATORS //////////////////////////////////
std::ostream& operator<<(std::ostream &log, const char *str);
std::ostream& operator<<(std::ostream &log, const std::string &str);
std::ostream& operator<<(std::ostream &log, const glm::vec2 &v);
std::ostream& operator<<(std::ostream &log, const glm::vec3 &v);
std::ostream& operator<<(std::ostream &log, const glm::vec4 &v);
std::ostream& operator<<(std::ostream &log, const glm::quat &q);
std::ostream& operator<<(std::ostream &log, const glm::mat4 &v);


#define _Logger_Suffix \
       "     at file:'" << __FILE__ << ", in function " << __FUNCTION__ << \
       ", at line number " << __LINE__ << std::endl

#define Logger_Log(x) do{\
    using std::operator<<;\
    std::ostringstream log;\
    log << x;\
    std::cerr << "[   LOG   ]: " << log.str() << _Logger_Suffix;\
    log.flush();\
} while(0)

#define Logger_Warning(x) do{\
    using std::operator<<;\
    std::ostringstream log;\
    log << x;\
    std::cerr << "[ WARNING ]: " << log.str() << _Logger_Suffix;\
    log.flush();\
} while(0)

#define Logger_Error(x) do{\
    using std::operator<<;\
    std::ostringstream log;\
    log << x;\
    std::cerr << "[  ERROR  ]: " << log.str() << _Logger_Suffix;\
    log.flush();\
} while(0)

#endif //LOGGER_H
