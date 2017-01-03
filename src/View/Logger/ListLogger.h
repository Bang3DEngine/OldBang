#ifndef LISTLOGGER_H
#define LISTLOGGER_H

#include <QColor>
#include <QDropEvent>

#include "String.h"
#include "DragDropAgent.h"

class ListLogger : public DragDropQListWidget
{
    Q_OBJECT

private:
     static QListWidget *s_listLogger;

public:
    explicit ListLogger(QWidget *parent = nullptr);

    static void AddLog(const String &str);
    static void AddWarn(const String &str);
    static void AddError(const String &str);

    static ListLogger* GetInstance();
    static void DecorateLastItem(const QColor &color);

    void dropEvent(QDropEvent *e) override;
};

#endif // LISTLOGGER_H
