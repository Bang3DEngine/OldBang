#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <vector>

#include <sstream>
#include <iomanip>
#include <algorithm>

#include "String.h"

class StringUtils
{
private:
    StringUtils() {}

public:

    static const String TOKEN_SPACE;

    static bool Contains(const String &str, const String &whatToFind);
    static float ToFloat(const String &str, bool *ok = nullptr);
    static int ToInt(const String &str, bool *ok = nullptr);
    static String FromFloat(float f, int precision = 6);

    static void TrimLeft(String *str);
    static void TrimRight(String *str);
    static void Trim(String *str);
    static String Trim(const String &str);

    static String Concat(const std::vector<String> &particles, String joiner = "");
    static std::vector<String> Split(const String &content, char splitter);
    static std::vector<String> SplitTrim(const String &content, char splitter);

    /**
     * @brief FormatInspectorLabel
     * @param labelString
     * @return Returns the label formatted. It replaces "_" with spaces.
     */
    static String FormatInspectorLabel(const String &labelString);

    static bool IsNumber(char c);
    static bool IsLetter(char c);
    static bool IsCap(char c);

    /**
     * @brief AddInFrontOfWords
     * Adds the passed particle in Front of every word.
     * For example: ("-I", "file1 dir/file2 dir/file3") is converted to
     * "-Ifile1 -Idir/file2 -Idir/file3"
     */
    static void AddInFrontOfWords(String particle, String *str);
    static void RemoveLineBreaks(String *str);
    static void Replace(String *content, const String &toFind, const String &replaceWithThis);

    static std::vector<String> BangEnumVariadicStringToNamesArray(const String &_va_args_);

    template <class EnumName>
    static std::vector<EnumName> BangEnumVariadicStringToValuesArray(const String &_va_args_)
    {
        // We receive something like "Wololo, Apple = 49, Pear=29, Lololo=2193, Banana,Sandwich, Monkey=32"
        // We want this vector: [0, 49, 29, 2193, 2194, 2195, 32]
        std::vector<EnumName> result;
        std::vector<String> splitted = SplitTrim(_va_args_, ',');
        int lastValue = -1;
        for (String str : splitted)
        {
            Trim(&str);
            std::vector<String> equalSplitted = SplitTrim(str, '=');
            int val;
            if (equalSplitted.size() == 2) // Has value
            {
                val = std::atoi(equalSplitted[1].ToCString());
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
