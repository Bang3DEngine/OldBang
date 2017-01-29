#ifndef LISTLOGGER_H
#define LISTLOGGER_H

#include <QColor>
#include <QDropEvent>

#include "Map.h"
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

    void dropEvent(QDropEvent *e) override;

    void OnEditorPlay();

private slots:
    void OnClear();

    void OnCollapseChanged(bool collapse);
    void OnClearOnPlayChanged(bool clearOnPlay);
    void OnAutoScrollChanged(bool autoScroll);
    void OnShowLogMessagesChanged(bool showLogMessages);
    void OnShowWarnMessagesChanged(bool showWarnMessages);
    void OnShowErrorMessagesChanged(bool showErrorMessages);

    void DecorateLastItem(const Color &color);

private:
    static int c_iconColumn;
    static int c_msgColumn;
    static int c_countColumn;
    static int c_lineColumn;
    static int c_fileNameColumn;

    bool m_collapse          = false;
    bool m_clearOnPlay       = false;
    bool m_autoScroll        = false;
    bool m_showLogMessages   = false;
    bool m_showWarnMessages  = false;
    bool m_showErrorMessages = false;

    enum MessageType { Log = 0, Warn = 1, Error = 2 };
    struct MessageRow
    {
        int line;
        String msg;
        String fileName;
        MessageType msgType;
        bool operator==(const MessageRow &rhs) const;
        bool operator<(const MessageRow &rhs) const;
    };
    List<MessageRow> m_currentMessageRowList;
    Map<MessageRow, int> m_currentCollapsedMsgs;
    Map<MessageRow, QTreeWidgetItem*> m_messageRowToItem;

    QIcon m_logIcon;
    QIcon m_warnIcon;
    QIcon m_errorIcon;

    void RefreshList();

    QTreeWidgetItem *CreateItemFromMessageRow(const MessageRow &mr);

    void OnAddLog(const String &str, int line, const String &fileName);
    void OnAddWarn(const String &str, int line, const String &fileName);
    void OnAddError(const String &str, int line, const String &fileName);
    void AddRow(int line, const String &msg,
                const String &fileName, MessageType msgType,
                bool uniqueMessage = false);
};

#endif // LISTLOGGER_H
