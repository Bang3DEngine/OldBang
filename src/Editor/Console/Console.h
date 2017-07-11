#ifndef CONSOLE_H
#define CONSOLE_H

#include <queue>
#include <QColor>
#include <QMutex>
#include <QDropEvent>
#include "Bang/WinUndef.h"

#include "Bang/Map.h"
#include "Bang/List.h"
#include "Bang/String.h"
#include "Bang/DragDropQTreeWidget.h"

class QLabel;
class Console : public DragDropQTreeWidget
{
    Q_OBJECT

public:
    typedef long long MessageId;

    explicit Console(QWidget *parent = nullptr);

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

    static Console* GetInstance();

    void dropEvent(QDropEvent *e) override;

    void OnEditorPlay();

    void ClearMessage(MessageId id);

    void OnWindowShown();

private slots:
    void OnClear();

    void OnCollapseChanged(bool collapse);
    void OnClearOnPlayChanged(bool clearOnPlay);
    void OnShowLogMessagesChanged(bool showLogMessages);
    void OnShowWarnMessagesChanged(bool showWarnMessages);
    void OnShowErrorMessagesChanged(bool showErrorMessages);
    void OnSelectionChanged(QTreeWidgetItem *current,
                            QTreeWidgetItem *previous);

    void DecorateLastItem(const Color &color);

private:
    enum MessageType { Log = 0, Warn = 1, Error = 2 };
    const Color MessageTypeColor[3] = { Color(1.0f, 1.0f, 1.0f),
                                        Color(1.0f, 1.0f, 0.3f),
                                        Color(1.0f, 0.3f, 0.3f)};

    struct Message
    {
        MessageId id           = -1;
        int line               =  0;
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

    static int c_rowHeight;
    static int c_iconColumn, c_msgColumn, c_countColumn, c_lineColumn,
               c_fileNameColumn;

    QMutex m_messageQueueMutex;
    std::queue<Message> m_messageQueue;

    MessageId m_latestMessageId = 0;

    Map<MessageId, Message> m_id_to_messages;
    Map<MessageId, QTreeWidgetItem*> m_id_to_item;
    Map<Message, int> m_messageCount;
    Map<Message, QTreeWidgetItem*> m_msg_to_collapsingItem;

    bool m_collapsing         = false;
    bool m_clearOnPlay        = false;
    bool m_autoScroll         = true;
    bool m_showLogMessages    = false;
    bool m_showWarnMessages   = false;
    bool m_showErrorMessages  = false;
    int  m_totalLogMessages   = 0;
    int  m_totalWarnMessages  = 0;
    int  m_totalErrorMessages = 0;
    QLabel *m_completeMessageLabel = nullptr;

    QIcon m_logIcon;
    QIcon m_warnIcon;
    QIcon m_errorIcon;

    void ProcessMessagesQueue();
    MessageId EnqueueMessage(Message &message);

    void UpdateMessagesCountTexts();
    bool MustBeShown(const Message &message) const;
    void RefreshCollapsingAndShowing();

    QTreeWidgetItem *CreateItemFromMessageRow(const Message &mr);

    MessageId AddMessage(const Message &message,
                         int forcedMessageId = -1);

    friend class EditorApplication;
};

#endif // CONSOLE_H
