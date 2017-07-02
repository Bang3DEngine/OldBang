#ifndef REGEX_H
#define REGEX_H

#include <QRegExp>
#include "Bang/WinUndef.h"

#include "Bang/String.h"

class RegexWC : public QRegExp
{
public:
    RegexWC(const String &pattern, bool caseSensitive = true);

    bool Matches(const String &str) const;
    bool MatchesExactly(const String &str) const;
};

#endif // REGEX_H
