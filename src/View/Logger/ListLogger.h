#ifndef LISTLOGGER_H
#define LISTLOGGER_H

#include <QListWidget>

class ListLogger : public QListWidget
{
    Q_OBJECT

public:
    explicit ListLogger(QWidget *parent = nullptr);

    static void AddLog(const std::string &str);
};

#endif // LISTLOGGER_H
