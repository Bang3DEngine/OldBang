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
    typedef unsigned long long MessageId;

    explicit ListLogger(QWidget *parent = nullptr);

    static void Clear();
    static MessageId AddLog(const String &str, int line,
                            const String &fileName,
                            bool persistent = false);
    static MessageId AddWarn(const String &str, int line,
                             const String &fileName,
                             bool persistent = false);
    static MessageId AddError(const String &str, int line,
                              const String &fileName,
                              bool persistent = false);

    static ListLogger* GetInstance();

    void dropEvent(QDropEvent *e) override;

    void OnEditorPlay();

    void ClearMessage(MessageId id);

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

    enum MessageType { Log = 0, Warn = 1, Error = 2 };
    const Color MessageTypeColor[3] = { Color(1.0f, 1.0f, 1.0f),
                                        Color(1.0f, 1.0f, 0.3f),
                                        Color(1.0f, 0.3f, 0.3f)};

    struct Message
    {
        int line               = 0;
        String msg             = "";
        String fileName        = "";
        bool persistent        = false;
        MessageType type       = MessageType::Log;
        bool operator==(const Message &rhs) const;
        bool operator<(const Message &rhs) const;

        Message() {}
        Message(int _line, const String &_msg, const String &_fileName,
                bool _persistent, MessageType _msgType) :
            line(_line), msg(_msg), fileName(_fileName), persistent(_persistent),
            type(_msgType)
        {
        }
    };

    MessageId m_latestMessageId = 0;
    Map<MessageId, Message> m_currentMessages;
    Map<Message, MessageId> m_currentMessagesIds;
    Map<Message, int> m_collapsedMsgsCount;
    Map<Message, QTreeWidgetItem*> m_messageToItem;

    bool m_collapse          = false;
    bool m_clearOnPlay       = false;
    bool m_autoScroll        = false;
    bool m_showLogMessages   = false;
    bool m_showWarnMessages  = false;
    bool m_showErrorMessages = false;
    int  m_totalLogMessages   = 0;
    int  m_totalWarnMessages  = 0;
    int  m_totalErrorMessages = 0;

    QIcon m_logIcon;
    QIcon m_warnIcon;
    QIcon m_errorIcon;

    void RefreshList();

    QTreeWidgetItem *CreateItemFromMessageRow(const Message &mr);

    MessageId AddMessage(const Message &message, int forcedMessageId = -1);
};

#endif // LISTLOGGER_H
