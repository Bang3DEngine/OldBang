#ifndef LISTLOGGER_H
#define LISTLOGGER_H

#include <QColor>
#include <QDropEvent>

#include "List.h"
#include "String.h"
#include "DragDropAgent.h"

class ListLogger : public DragDropQTreeWidget
{
    Q_OBJECT

public:
    explicit ListLogger(QWidget *parent = nullptr);

    static void Clear();
    static void AddLog(const String &str, int line, const String &fileName);
    static void AddWarn(const String &str, int line, const String &fileName);
    static void AddError(const String &str, int line, const String &fileName);

    static ListLogger* GetInstance();
    static void DecorateLastItem(const QColor &color);

    void dropEvent(QDropEvent *e) override;

private:
    static int c_iconColumn;
    static int c_msgColumn;
    static int c_lineColumn;
    static int c_fileNameColumn;

    struct MessageRow
    {
        String msg;
        int line;
        String fileName;
        bool operator==(const MessageRow &rhs) const;
    };
    List<MessageRow> m_currentMessageRowList;

    QIcon m_logIcon;
    QIcon m_warnIcon;
    QIcon m_errorIcon;

    void AddRow(const QIcon &icon, const String &msg,
                int line, const String &fileName, bool uniqueMessage = false);
};

#endif // LISTLOGGER_H
