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

    static const std::string TOKEN_SPACE;

    static bool Contains(const std::string &str, const std::string &whatToFind);
    static float ToFloat(const std::string &str);

    static void TrimLeft(std::string *str);
    static void TrimRight(std::string *str);
    static void Trim(std::string *str);
    static std::string Trim(const std::string &str);

    static std::vector<std::string> Split(const std::string &content, char splitter);
    static std::vector<std::string> SplitTrim(const std::string &content, char splitter);

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

    static std::vector<std::string> BangEnumVariadicStringToNamesArray(const std::string &_va_args_);

    template <class EnumName>
    static std::vector<EnumName> BangEnumVariadicStringToValuesArray(const std::string &_va_args_)
    {
        // We receive something like "Wololo, Apple = 49, Pear=29, Lololo=2193, Banana,Sandwich, Monkey=32"
        // We want this vector: [0, 49, 29, 2193, 2194, 2195, 32]
        std::vector<EnumName> result;
        std::vector<std::string> splitted = SplitTrim(_va_args_, ',');
        int lastValue = 0;
        for (std::string str : splitted)
        {
            Trim(&str);
            std::vector<std::string> equalSplitted = SplitTrim(str, '=');
            int val;
            if (equalSplitted.size() == 2) // Has value
            {
                val = std::atoi(equalSplitted[1].c_str());
            }
            else // Has no value, one more than the last one
            {
                val = lastValue + 1;
            }

            lastValue = val;
            result.push_back(static_cast<EnumName>(val));
        }
        return result;
    }

};

#endif // STRINGUTILS_H
