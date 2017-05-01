#include "Regex.h"

RegexWC::RegexWC(const String &pattern, bool caseSensitive)
    : QRegExp(pattern.ToQString(),
              (caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive),
              QRegExp::PatternSyntax::WildcardUnix)
{
}

bool RegexWC::Matches(const String &str) const
{
    exactMatch(str.ToQString());
    return matchedLength() > 0;
}

bool RegexWC::MatchesExactly(const String &str) const
{
    return exactMatch(str.ToQString());
}
