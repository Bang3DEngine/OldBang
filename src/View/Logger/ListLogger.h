#ifndef LISTLOGGER_H
#define LISTLOGGER_H

#include <QListWidget>

class ListLogger : public QListWidget
{
    Q_OBJECT

private:
     static QListWidget *s_listLogger;

public:
    explicit ListLogger(QWidget *parent = nullptr);

    static void AddLog(const std::string &str);
    static void AddWarn(const std::string &str);
    static void AddError(const std::string &str);

    static ListLogger* GetInstance();
    static void DecorateLastItem(const QColor &color);
};

#endif // LISTLOGGER_H
