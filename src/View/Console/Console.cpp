#include "Bang/Console.h"

#include <QLabel>
#include <QScrollBar>
#include "Bang/WinUndef.h"

#include "Bang/Debug.h"
#include "Bang/Color.h"
#include "Bang/EditorWindow.h"

int Console::c_rowHeight      = 30;

int Console::c_iconColumn     = 1;
int Console::c_msgColumn      = 2;
int Console::c_countColumn    = 3;
int Console::c_lineColumn     = 4;
int Console::c_fileNameColumn = 5;

Console::Console(QWidget *parent) : DragDropQTreeWidget()
{
    m_logIcon   = QIcon(":/qss_icons/Icons/LogIcon.png");
    m_warnIcon  = QIcon(":/qss_icons/Icons/WarnIcon.png");
    m_errorIcon = QIcon(":/qss_icons/Icons/ErrorIcon.png");
}

void Console::Clear()
{
    Console *console = Console::GetInstance(); ENSURE(console);
    console->OnClear();
}

Console::MessageId Console::AddLog(const String &str, int line,
                                   const String &fileName,
                                   bool persistent)
{
	Console *console = Console::GetInstance(); if (!console) { return -1; }
    Message m(line, str, fileName, persistent, MessageType::Log);
    return console->EnqueueMessage(m);
}

Console::MessageId Console::AddWarn(const String &str, int line,
                                    const String &fileName,
                                    bool persistent)
{
    Console *console = Console::GetInstance(); if (!console) { return -1; }
    Message m(line, str, fileName, persistent, MessageType::Warn);
    return console->EnqueueMessage(m);
}

Console::MessageId Console::AddError(const String &str, int line,
                                     const String &fileName,
                                     bool persistent)
{
    Console *console = Console::GetInstance(); if (!console) { return -1; }
    Message m(line, str, fileName, persistent, MessageType::Error);
    return console->EnqueueMessage(m);
}

void Console::DecorateLastItem(const Color &color)
{
    QTreeWidgetItem *item = topLevelItem(topLevelItemCount() - 1);
    if (item)
    {
        item->setTextColor(c_msgColumn, color.ToQColor());
    }
}

void Console::ProcessMessagesQueue()
{
    while (!m_messageQueue.empty())
    {
        const Message &msg = m_messageQueue.front();
        AddMessage(msg);
        m_messageQueue.pop();
    }
}

Console::MessageId Console::EnqueueMessage(Console::Message &message)
{
    QMutexLocker locker(&m_messageQueueMutex);
    message.id = ++m_latestMessageId;
    m_messageQueue.push(message);
    return message.id;
}

void Console::UpdateMessagesCountTexts()
{
    EditorWindow *win = EditorWindow::GetInstance();
    win->buttonShowLogMessages->setText( String::ToString(m_totalLogMessages)
                                         .ToQString() );
    win->buttonShowWarnMessages->setText( String::ToString(m_totalWarnMessages)
                                          .ToQString() );
    win->buttonShowErrorMessages->setText(String::ToString(m_totalErrorMessages)
                                           .ToQString() );
}

bool Console::MustBeShown(const Console::Message &message) const
{
    if ((message.type == MessageType::Log   && !m_showLogMessages)   ||
        (message.type == MessageType::Warn  && !m_showWarnMessages)  ||
        (message.type == MessageType::Error && !m_showErrorMessages)
       )
    {
        return false;
    }
    return true;
}

void Console::RefreshCollapsingAndShowing()
{
    if (!m_collapsing)
    {
        for (const auto& msgId_msg : m_id_to_messages)
        {
            MessageId msgId = msgId_msg.first;
            const Message &msg= msgId_msg.second;
            QTreeWidgetItem *item = m_id_to_item[msgId];
            item->setHidden( !MustBeShown(msg) );
        }
    }
    else
    {
        for (const auto& msgId_msg : m_id_to_messages)
        {
            const Message &msg = msgId_msg.second;
            MessageId msgId = msgId_msg.first;
            QTreeWidgetItem *item = m_id_to_item[msgId];
            bool hide = !MustBeShown(msg) ||
                        item != m_msg_to_collapsingItem[msg];
            item->setHidden(hide);
        }

        // Update count texts
        for (const auto& msg_count : m_messageCount)
        {
            const Message &msg = msg_count.first;
            QTreeWidgetItem *item = m_msg_to_collapsingItem[msg];
            int count = m_messageCount[msg];
            item->setText(c_countColumn, String::ToString(count).ToQString());
        }
    }
}

QTreeWidgetItem *Console::CreateItemFromMessageRow(const Message &mr)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    const QIcon *icon;
    if      (mr.type == MessageType::Log)   { icon = &m_logIcon;   }
    else if (mr.type == MessageType::Warn)  { icon = &m_warnIcon;  }
    else if (mr.type == MessageType::Error) { icon = &m_errorIcon; }
    item->setIcon(c_iconColumn, *icon);
    item->setSizeHint(0, QSize(item->sizeHint(0).width(), c_rowHeight));
    item->setSizeHint(1, QSize(item->sizeHint(1).width(), c_rowHeight));
    item->setSizeHint(2, QSize(item->sizeHint(2).width(), c_rowHeight));
    item->setSizeHint(3, QSize(item->sizeHint(3).width(), c_rowHeight));
    item->setSizeHint(4, QSize(item->sizeHint(4).width(), c_rowHeight));
    item->setSizeHint(5, QSize(item->sizeHint(5).width(), c_rowHeight));

    String shortMessage = mr.msg; // .ElideRight(mr.msg, 100);
    item->setText(c_msgColumn, shortMessage.ToQString());
    item->setText(c_lineColumn, String::ToString(mr.line).ToQString());
    item->setText(c_fileNameColumn, mr.fileName.ToQString());
    return item;
}

Console::MessageId Console::AddMessage(const Message &msg,
                                             int forcedMsgId)
{
    QScrollBar *scroll = verticalScrollBar();
    m_autoScroll = (scroll->value() == scroll->maximum());

    QTreeWidgetItem *item = CreateItemFromMessageRow(msg);
    addTopLevelItem(item);
    DecorateLastItem( MessageTypeColor[msg.type] );
    bool mustCollapse = m_collapsing && m_messageCount.ContainsKey(msg);
    bool hideItem = !MustBeShown(msg) || mustCollapse;
    item->setHidden(hideItem);

    MessageId messageId = forcedMsgId >= 0 ? forcedMsgId : msg.id;
    m_id_to_messages.Set(messageId, msg);
    m_id_to_item.Set(messageId, item);

    if (!m_messageCount.ContainsKey(msg))
    {
        m_messageCount.Set(msg, 0);
    }
    m_messageCount[msg]++;

    if (!m_msg_to_collapsingItem.ContainsKey(msg))
    {
        // The item to collapse into is the first one found
        m_msg_to_collapsingItem.Set(msg, item);
    }

    // Update collapse count text
    if (m_collapsing)
    {
        int count = m_messageCount[msg];
        QTreeWidgetItem *collapsingItem = m_msg_to_collapsingItem[msg];
        if (collapsingItem)
        {
            collapsingItem->setText(c_countColumn,
                                    String::ToString(count).ToQString());
        }
    }

    // Update show toolButtons text
    if      (msg.type == MessageType::Log)   { ++m_totalLogMessages; }
    else if (msg.type == MessageType::Warn)  { ++m_totalWarnMessages; }
    else if (msg.type == MessageType::Error) { ++m_totalErrorMessages; }
    UpdateMessagesCountTexts();

    if (m_autoScroll)
    {
        scrollToBottom();
    }

    return messageId;
}

Console *Console::GetInstance()
{
    return EditorWindow::GetInstance() ?
           EditorWindow::GetInstance()->console : nullptr;
}

void Console::dropEvent(QDropEvent *e)
{
    e->ignore();
}

void Console::OnClear()
{
    clear();
    m_totalLogMessages   = 0;
    m_totalWarnMessages  = 0;
    m_totalErrorMessages = 0;

    Map<MessageId, Message> persistentMessages;
    for (const auto &pair : m_id_to_messages)
    {
        const Message &msg = pair.second;
        if (msg.persistent)
        {
            int messageId = pair.first;
            persistentMessages.Set(messageId, msg);
        }
    }
    m_id_to_item.Clear();
    m_messageCount.Clear();
    m_id_to_messages.Clear();
    m_msg_to_collapsingItem.Clear();

    // Restore persistent messages
    for (const auto &pair : persistentMessages)
    {
        AddMessage(pair.second, pair.first);
    }

    m_completeMessageLabel->setText("");
    UpdateMessagesCountTexts();
}

void Console::OnEditorPlay()
{
    if (m_clearOnPlay)
    {
        OnClear();
    }
}

void Console::ClearMessage(Console::MessageId id)
{
    //Debug_Log("ClearMessage: " << m_id_to_messages);
    if (m_id_to_messages.ContainsKey(id))
    {
        const Message &msg = m_id_to_messages[id];
        if      (msg.type == MessageType::Log)   { --m_totalLogMessages; }
        else if (msg.type == MessageType::Warn)  { --m_totalWarnMessages; }
        else if (msg.type == MessageType::Error) { --m_totalErrorMessages; }
        UpdateMessagesCountTexts();

        QTreeWidgetItem *item = m_id_to_item[id];
        if (item) { delete item; }

        m_id_to_item.Remove(id);
        m_id_to_messages.Remove(id);
        if (m_messageCount.ContainsKey(msg))
        {
            m_messageCount[msg]--;
            if (m_messageCount[msg] <= 0)
            {
                m_messageCount.Remove(msg);
                m_msg_to_collapsingItem.Remove(msg);
            }
        }
    }
}

void Console::OnWindowShown()
{
    hideColumn(0); // No arrow

    EditorWindow *win = EditorWindow::GetInstance();
    m_collapsing        = win->buttonCollapse->isChecked();
    m_clearOnPlay       = win->buttonClearOnPlay->isChecked();
    m_showLogMessages   = win->buttonShowLogMessages->isChecked();
    m_showWarnMessages  = win->buttonShowWarnMessages->isChecked();
    m_showErrorMessages = win->buttonShowErrorMessages->isChecked();

    m_completeMessageLabel = win->labelConsoleCompleteMessage;
    m_completeMessageLabel->setText("");

    OnCollapseChanged(m_collapsing);

    QObject::connect(this,
                SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
                SLOT(  OnSelectionChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    QObject::connect(win->buttonConsoleClear, SIGNAL( pressed() ),
                     this, SLOT(OnClear()));
    QObject::connect(win->buttonCollapse, SIGNAL(clicked(bool)),
                     this, SLOT(OnCollapseChanged(bool)));
    QObject::connect(win->buttonClearOnPlay, SIGNAL(clicked(bool)),
                     this, SLOT(OnClearOnPlayChanged(bool)));
    QObject::connect(win->buttonShowLogMessages, SIGNAL(clicked(bool)),
                     this, SLOT(OnShowLogMessagesChanged(bool)));
    QObject::connect(win->buttonShowWarnMessages, SIGNAL(clicked(bool)),
                     this, SLOT(OnShowWarnMessagesChanged(bool)));
    QObject::connect(win->buttonShowErrorMessages, SIGNAL(clicked(bool)),
                     this, SLOT(OnShowErrorMessagesChanged(bool)));

    // Set headers
    setHeaderLabels( {"", "", "Message", "Count", "Line", "File name"} );
    header()->setSectionResizeMode(Console::c_iconColumn,
                                   QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(Console::c_msgColumn,
                                   QHeaderView::Stretch);
    header()->setSectionResizeMode(Console::c_countColumn,
                                   QHeaderView::Fixed);
    header()->setSectionResizeMode(Console::c_lineColumn,
                                   QHeaderView::Fixed);
    header()->resizeSection(Console::c_countColumn, 50);
    header()->resizeSection(Console::c_lineColumn, 50);
}

void Console::OnCollapseChanged(bool collapse)
{
    m_collapsing = collapse;
    setColumnHidden(Console::c_countColumn, !m_collapsing);
    RefreshCollapsingAndShowing();
}

void Console::OnClearOnPlayChanged(bool clearOnPlay)
{
    m_clearOnPlay = clearOnPlay;
}

void Console::OnShowLogMessagesChanged(bool showLogMessages)
{
    m_showLogMessages = showLogMessages;
    RefreshCollapsingAndShowing();
}

void Console::OnShowWarnMessagesChanged(bool showWarnMessages)
{
    m_showWarnMessages = showWarnMessages;
    RefreshCollapsingAndShowing();
}

void Console::OnShowErrorMessagesChanged(bool showErrorMessages)
{
    m_showErrorMessages = showErrorMessages;
    RefreshCollapsingAndShowing();
}

void Console::OnSelectionChanged(QTreeWidgetItem *current,
                                    QTreeWidgetItem *previous)
{
    if (current)
    {
        String msg = String( current->text(Console::c_msgColumn) );
        m_completeMessageLabel->setText( msg.ToQString() );
    }
    else
    {
        m_completeMessageLabel->setText("");
    }
}

bool Console::Message::operator==(const Console::Message &rhs) const
{
    return type == rhs.type &&
           line == rhs.line &&
           fileName == rhs.fileName &&
           msg == rhs.msg;
}

bool Console::Message::operator<(const Console::Message &rhs) const
{
    if (int(type) < int(rhs.type)) return true;
    else if (int(type) > int(rhs.type)) return false;
    else
    {
        if (line < rhs.line) return true;
        else if (line > rhs.line) return false;
        else
        {
            int fileNameComp = fileName.compare(rhs.fileName);
            if (fileNameComp < 0) return true;
            else if (fileNameComp > 0) return false;
            else
            {
                int msgComp = msg.compare(rhs.msg);
                return msgComp < 0;
            }
        }
    }
}
