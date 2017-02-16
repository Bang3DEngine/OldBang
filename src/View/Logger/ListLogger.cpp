#include "ListLogger.h"

#include <QLabel>

#include "Debug.h"
#include "Color.h"
#include "StringUtils.h"
#include "EditorWindow.h"

int ListLogger::c_rowHeight      = 30;

int ListLogger::c_iconColumn     = 1;
int ListLogger::c_msgColumn      = 2;
int ListLogger::c_countColumn    = 3;
int ListLogger::c_lineColumn     = 4;
int ListLogger::c_fileNameColumn = 5;

ListLogger::ListLogger(QWidget *parent) : DragDropQTreeWidget()
{
    m_logIcon   = QIcon(":/qss_icons/Icons/LogIcon.png");
    m_warnIcon  = QIcon(":/qss_icons/Icons/WarnIcon.png");
    m_errorIcon = QIcon(":/qss_icons/Icons/ErrorIcon.png");
}

void ListLogger::Clear()
{
    ListLogger *listLogger = ListLogger::GetInstance(); ASSERT(listLogger);
    listLogger->OnClear();
}

ListLogger::MessageId ListLogger::AddLog(
        const String &str, int line,
        const String &fileName,
        bool persistent)
{
    ListLogger *listLogger = ListLogger::GetInstance();
    ASSERT(listLogger, "", -1);
    Message m(line, str, fileName, persistent, MessageType::Log);
    return listLogger->EnqueueMessage(m);
}

ListLogger::MessageId ListLogger::AddWarn(
        const String &str, int line,
        const String &fileName,
        bool persistent)
{
    ListLogger *listLogger = ListLogger::GetInstance();
    ASSERT(listLogger, "", -1);
    Message m(line, str, fileName, persistent, MessageType::Warn);
    return listLogger->EnqueueMessage(m);
}

ListLogger::MessageId ListLogger::AddError(
        const String &str, int line,
        const String &fileName,
        bool persistent)
{
    ListLogger *listLogger = ListLogger::GetInstance();
    ASSERT(listLogger, "", -1);
    Message m(line, str, fileName, persistent, MessageType::Error);
    return listLogger->EnqueueMessage(m);
}

void ListLogger::DecorateLastItem(const Color &color)
{
    QTreeWidgetItem *item = topLevelItem(topLevelItemCount() - 1);
    if (item)
    {
        item->setTextColor(c_msgColumn, color.ToQColor());
    }
}

void ListLogger::ProcessMessagesQueue()
{
    while (!m_messageQueue.empty())
    {
        const Message &msg = m_messageQueue.front();
        AddMessage(msg);
        m_messageQueue.pop();
    }
}

ListLogger::MessageId ListLogger::EnqueueMessage(ListLogger::Message &message)
{
    QMutexLocker locker(&m_messageQueueMutex);
    message.id = ++m_latestMessageId;
    m_messageQueue.push(message);
}

void ListLogger::UpdateMessagesCountTexts()
{
    EditorWindow *win = EditorWindow::GetInstance();
    win->buttonShowLogMessages->setText( String::ToString(m_totalLogMessages)
                                         .ToQString() );
    win->buttonShowWarnMessages->setText( String::ToString(m_totalWarnMessages)
                                          .ToQString() );
    win->buttonShowErrorMessages->setText(String::ToString(m_totalErrorMessages)
                                           .ToQString() );
}

bool ListLogger::MustBeShown(const ListLogger::Message &message) const
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

void ListLogger::RefreshCollapsingAndShowing()
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

QTreeWidgetItem *ListLogger::CreateItemFromMessageRow(const Message &mr)
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

    String shortMessage = mr.msg; // StringUtils::Elide(mr.msg, 100, true);
    item->setText(c_msgColumn, shortMessage.ToQString());
    item->setText(c_lineColumn, String::ToString(mr.line).ToQString());
    item->setText(c_fileNameColumn, mr.fileName.ToQString());
    return item;
}

ListLogger::MessageId ListLogger::AddMessage(const Message &msg,
                                             int forcedMsgId)
{
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
            collapsingItem->setText(c_countColumn, String::ToString(count)
                                                   .ToQString());
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

ListLogger *ListLogger::GetInstance()
{
    return EditorWindow::GetInstance() ?
           EditorWindow::GetInstance()->listLogger : nullptr;
}

void ListLogger::dropEvent(QDropEvent *e)
{
    e->ignore();
}

void ListLogger::OnClear()
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

void ListLogger::OnEditorPlay()
{
    if (m_clearOnPlay)
    {
        OnClear();
    }
}

void ListLogger::ClearMessage(ListLogger::MessageId id)
{
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

void ListLogger::OnWindowShown()
{
    hideColumn(0); // No arrow

    EditorWindow *win = EditorWindow::GetInstance();
    m_collapsing          = win->buttonCollapse->isChecked();
    m_clearOnPlay       = win->buttonClearOnPlay->isChecked();
    m_autoScroll        = win->buttonAutoScroll->isChecked();
    m_showLogMessages   = win->buttonShowLogMessages->isChecked();
    m_showWarnMessages  = win->buttonShowWarnMessages->isChecked();
    m_showErrorMessages = win->buttonShowErrorMessages->isChecked();

    m_completeMessageLabel = win->labelLoggerCompleteMessage;
    m_completeMessageLabel->setText("");

    OnCollapseChanged(m_collapsing);

    QObject::connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*,
                                                     QTreeWidgetItem*)),
                     this, SLOT(OnSelectionChanged(QTreeWidgetItem*,
                                                   QTreeWidgetItem*)));
    QObject::connect(win->buttonLoggerClear, SIGNAL( pressed() ),
                     this, SLOT(OnClear()));
    QObject::connect(win->buttonCollapse, SIGNAL(clicked(bool)),
                     this, SLOT(OnCollapseChanged(bool)));
    QObject::connect(win->buttonClearOnPlay, SIGNAL(clicked(bool)),
                     this, SLOT(OnClearOnPlayChanged(bool)));
    QObject::connect(win->buttonAutoScroll, SIGNAL(clicked(bool)),
                     this, SLOT(OnAutoScrollChanged(bool)));
    QObject::connect(win->buttonShowLogMessages, SIGNAL(clicked(bool)),
                     this, SLOT(OnShowLogMessagesChanged(bool)));
    QObject::connect(win->buttonShowWarnMessages, SIGNAL(clicked(bool)),
                     this, SLOT(OnShowWarnMessagesChanged(bool)));
    QObject::connect(win->buttonShowErrorMessages, SIGNAL(clicked(bool)),
                     this, SLOT(OnShowErrorMessagesChanged(bool)));

    // Set headers
    setHeaderLabels( {"", "", "Message", "Count", "Line", "File name"} );
    header()->setSectionResizeMode(ListLogger::c_iconColumn,
                                   QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(ListLogger::c_msgColumn,
                                   QHeaderView::Stretch);
    header()->setSectionResizeMode(ListLogger::c_countColumn,
                                   QHeaderView::Fixed);
    header()->setSectionResizeMode(ListLogger::c_lineColumn,
                                   QHeaderView::Fixed);
    header()->resizeSection(ListLogger::c_countColumn, 50);
    header()->resizeSection(ListLogger::c_lineColumn, 50);
}

void ListLogger::OnCollapseChanged(bool collapse)
{
    m_collapsing = collapse;
    setColumnHidden(ListLogger::c_countColumn, !m_collapsing);
    RefreshCollapsingAndShowing();
}

void ListLogger::OnClearOnPlayChanged(bool clearOnPlay)
{
    m_clearOnPlay = clearOnPlay;
}

void ListLogger::OnAutoScrollChanged(bool autoScroll)
{
    m_autoScroll = autoScroll;
    if (m_autoScroll) { scrollToBottom(); }
}

void ListLogger::OnShowLogMessagesChanged(bool showLogMessages)
{
    m_showLogMessages = showLogMessages;
    RefreshCollapsingAndShowing();
}

void ListLogger::OnShowWarnMessagesChanged(bool showWarnMessages)
{
    m_showWarnMessages = showWarnMessages;
    RefreshCollapsingAndShowing();
}

void ListLogger::OnShowErrorMessagesChanged(bool showErrorMessages)
{
    m_showErrorMessages = showErrorMessages;
    RefreshCollapsingAndShowing();
}

void ListLogger::OnSelectionChanged(QTreeWidgetItem *current,
                                    QTreeWidgetItem *previous)
{
    if (current)
    {
        String msg = String( current->text(ListLogger::c_msgColumn) );
        m_completeMessageLabel->setText( msg.ToQString() );
    }
    else
    {
        m_completeMessageLabel->setText("");
    }
}

bool ListLogger::Message::operator==(const ListLogger::Message &rhs) const
{
    return type == rhs.type &&
           line == rhs.line &&
           fileName == rhs.fileName &&
           msg == rhs.msg;
}

bool ListLogger::Message::operator<(const ListLogger::Message &rhs) const
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
