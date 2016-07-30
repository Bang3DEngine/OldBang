#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

class StringUtils
{
private:
    StringUtils() {}

public:

    static bool Contains(const std::string &str, const std::string &whatToFind);
    static float ToFloat(const std::string &str);

    static void TrimLeft(std::string *str);
    static void TrimRight(std::string *str);
    static void Trim(std::string *str);

    static std::vector<std::string> Split(const std::string &content, char splitter);

    /**
     * @brief FormatInspectorLabel
     * @param labelString
     * @return Returns the label formatted. It replaces "_" with spaces.
     */
    static std::string FormatInspectorLabel(const std::string &labelString);

    static bool IsNumber(char c);
    static bool IsLetter(char c);
    static bool IsCap(char c);

    /**
     * @brief AddInFrontOfWords
     * Adds the passed particle in front of every word.
     * For example: ("-I", "file1 dir/file2 dir/file3") is converted to
     * "-Ifile1 -Idir/file2 -Idir/file3"
     */
    static void AddInFrontOfWords(std::string particle, std::string *str);
    static void RemoveLineBreaks(std::string *str);
    static void Replace(std::string *content, const std::string &toFind, const std::string &replaceWithThis);
};

#endif // STRINGUTILS_H
