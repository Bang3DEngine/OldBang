#include "ListLogger.h"

#include "Debug.h"
#include "Color.h"
#include "EditorWindow.h"

int ListLogger::c_iconColumn     = 1;
int ListLogger::c_msgColumn      = 2;
int ListLogger::c_countColumn    = 3;
int ListLogger::c_lineColumn     = 4;
int ListLogger::c_fileNameColumn = 5;

ListLogger::ListLogger(QWidget *parent) : DragDropQTreeWidget()
{
    hideColumn(0); // No arrow

    m_logIcon   = QIcon(":/qss_icons/Icons/LogIcon.png");
    m_warnIcon  = QIcon(":/qss_icons/Icons/WarnIcon.png");
    m_errorIcon = QIcon(":/qss_icons/Icons/ErrorIcon.png");

    EditorWindow *win = EditorWindow::GetInstance();
    m_collapse          = win->buttonCollapse->isChecked();
    m_clearOnPlay       = win->buttonClearOnPlay->isChecked();
    m_autoScroll        = win->buttonAutoScroll->isChecked();
    m_showLogMessages   = win->buttonShowLogMessages->isChecked();
    m_showWarnMessages  = win->buttonShowWarnMessages->isChecked();
    m_showErrorMessages = win->buttonShowErrorMessages->isChecked();

    OnCollapseChanged(m_collapse);

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
    setHeaderHidden(false);
    setHeaderLabels( {"", "", "Message", "Count", "Line", "File name"} );
    header()->setSectionResizeMode(ListLogger::c_iconColumn,  QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(ListLogger::c_msgColumn,   QHeaderView::Stretch);
    header()->setSectionResizeMode(ListLogger::c_countColumn, QHeaderView::Fixed);
    header()->setSectionResizeMode(ListLogger::c_lineColumn,  QHeaderView::Fixed);
    header()->resizeSection(ListLogger::c_countColumn, 50);
    header()->resizeSection(ListLogger::c_lineColumn, 50);
    header()->setStretchLastSection(false);
}

void ListLogger::Clear()
{
    ListLogger *listLogger = ListLogger::GetInstance(); ASSERT(listLogger);
    listLogger->OnClear();
}

void ListLogger::AddLog(const String &str, int line, const String &fileName)
{
    ListLogger *listLogger = ListLogger::GetInstance(); ASSERT(listLogger);
    listLogger->OnAddLog(str, line, fileName);
}

void ListLogger::AddWarn(const String &str, int line, const String &fileName)
{
    ListLogger *listLogger = ListLogger::GetInstance(); ASSERT(listLogger);
    listLogger->OnAddWarn(str, line, fileName);
}

void ListLogger::AddError(const String &str, int line, const String &fileName)
{
    ListLogger *listLogger = ListLogger::GetInstance(); ASSERT(listLogger);
    listLogger->OnAddError(str, line, fileName);
}

void ListLogger::DecorateLastItem(const Color &color)
{
    QTreeWidgetItem *item = topLevelItem(topLevelItemCount() - 1);
    if (item)
    {
        item->setTextColor(c_msgColumn, color.ToQColor());
    }
}

void ListLogger::RefreshList()
{
    List<MessageRow> mrList = m_currentMessageRowList;

    OnClear();
    for (const MessageRow &mr : mrList)
    {
        AddRow(mr.line, mr.msg, mr.fileName, mr.msgType);
    }
}

QTreeWidgetItem *ListLogger::CreateItemFromMessageRow(
        const ListLogger::MessageRow &mr)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    const QIcon *icon;
    if      (mr.msgType == MessageType::Log)   { icon = &m_logIcon;   }
    else if (mr.msgType == MessageType::Warn)  { icon = &m_warnIcon;  }
    else if (mr.msgType == MessageType::Error) { icon = &m_errorIcon; }
    item->setIcon(c_iconColumn, *icon);

    String shortMessage = StringUtils::Elide(mr.msg, 100, true);
    item->setText(c_msgColumn, shortMessage.ToQString());

    item->setText(c_lineColumn, String::ToString(mr.line).ToQString());
    item->setText(c_fileNameColumn, mr.fileName.ToQString());

    return item;
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

void ListLogger::OnAddLog(const String &str, int line, const String &fileName)
{
    AddRow(line, str, fileName, MessageType::Log);
}

void ListLogger::OnAddWarn(const String &str, int line, const String &fileName)
{
    AddRow(line, str, fileName, MessageType::Warn);
}

void ListLogger::OnAddError(const String &str, int line, const String &fileName)
{
    AddRow(line, str, fileName, MessageType::Error);
}

void ListLogger::OnClear()
{
    clear();
    m_messageRowToItem.Clear();
    m_currentCollapsedMsgs.Clear();
    m_currentMessageRowList.Clear();
}

void ListLogger::OnEditorPlay()
{
    if (m_clearOnPlay)
    {
        OnClear();
    }
}

void ListLogger::OnCollapseChanged(bool collapse)
{
    m_collapse = collapse;
    setColumnHidden(ListLogger::c_countColumn, !m_collapse);
    RefreshList();
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
    RefreshList();
}

void ListLogger::OnShowWarnMessagesChanged(bool showWarnMessages)
{
    m_showWarnMessages = showWarnMessages;
    RefreshList();
}

void ListLogger::OnShowErrorMessagesChanged(bool showErrorMessages)
{
    m_showErrorMessages = showErrorMessages;
    RefreshList();
}

void ListLogger::AddRow(int line, const String &msg, const String &fileName,
                        MessageType msgType, bool uniqueMessage)
{
    MessageRow msgRow;
    msgRow.line     = line;
    msgRow.msg      = msg;
    msgRow.fileName = fileName;
    msgRow.msgType  = msgType;

    if (uniqueMessage && m_currentMessageRowList.Contains(msgRow))
    {
        return;
    }

    bool dontAddRow = m_collapse && m_currentCollapsedMsgs.ContainsKey(msgRow);
    if (!m_currentCollapsedMsgs.ContainsKey(msgRow))
    {
        m_currentCollapsedMsgs[msgRow] = 0;
    }

    m_currentCollapsedMsgs[msgRow] += 1;
    m_currentMessageRowList.Add(msgRow);

    bool hasToAddRow = !dontAddRow;
    if ( (!m_showLogMessages   && msgRow.msgType == MessageType::Log  ) ||
         (!m_showWarnMessages  && msgRow.msgType == MessageType::Warn ) ||
         (!m_showErrorMessages && msgRow.msgType == MessageType::Error)
       )
    {
        hasToAddRow = false;
    }

    if (hasToAddRow)
    {
        QTreeWidgetItem *item = CreateItemFromMessageRow(msgRow);
        m_messageRowToItem[msgRow] = item;
        addTopLevelItem(item);

        if (msgRow.msgType == MessageType::Warn)
        {
            DecorateLastItem( Color(1.0f, 1.0f, 0.3f) );
        }
        else if (msgRow.msgType == MessageType::Error)
        {
            DecorateLastItem( Color(1.0f, 0.3f, 0.3f) );
        }
    }

    if (m_collapse)
    {
        // Update count
        QTreeWidgetItem *item = m_messageRowToItem[msgRow];
        if (item)
        {
            int newCount = m_currentCollapsedMsgs[msgRow];
            String newCountStr = String::ToString(newCount);
            item->setText(ListLogger::c_countColumn, newCountStr.ToQString());
        }
    }

    if (m_autoScroll) { scrollToBottom(); }
}


bool ListLogger::MessageRow::operator==(const ListLogger::MessageRow &rhs) const
{
    return line == rhs.line &&
           fileName == rhs.fileName &&
           msg == rhs.msg &&
           msgType == rhs.msgType;
}

bool ListLogger::MessageRow::operator<(const ListLogger::MessageRow &rhs) const
{
    if (line < rhs.line) return true; else if (line > rhs.line) return false;
    else
    {
        int fileNameComp = fileName.compare(rhs.fileName);
        if (fileNameComp < 0) return true; else if (fileNameComp > 0) return false;
        else
        {
            int msgComp = msg.compare(rhs.msg);
            if (msgComp < 0) return true; else if (msgComp > 0) return false;
            else
            {
                return int(msgType) < int(rhs.msgType);
            }
        }
    }
}
