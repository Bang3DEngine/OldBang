#include "ListLogger.h"

#include "Debug.h"
#include "Color.h"
#include "EditorWindow.h"

int ListLogger::c_iconColumn     = 1;
int ListLogger::c_msgColumn      = 2;
int ListLogger::c_lineColumn     = 3;
int ListLogger::c_fileNameColumn = 4;

ListLogger::ListLogger(QWidget *parent) : DragDropQTreeWidget()
{
    hideColumn(0); // No arrow

    m_logIcon   = QIcon(":/qss_icons/Icons/LogIcon.png");
    m_warnIcon  = QIcon(":/qss_icons/Icons/WarnIcon.png");
    m_errorIcon = QIcon(":/qss_icons/Icons/ErrorIcon.png");

    EditorWindow *win = EditorWindow::GetInstance();
    QObject::connect(win->buttonLoggerClear, SIGNAL( pressed() ),
                     this, SLOT(OnClear()));

    // Set headers
    setHeaderHidden(false);
    setHeaderLabels( {"", "", "Message", "Line", "File name"} );
    header()->setSectionResizeMode(ListLogger::c_iconColumn, QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(ListLogger::c_msgColumn,  QHeaderView::Stretch);
    header()->setSectionResizeMode(ListLogger::c_lineColumn, QHeaderView::Fixed);
    header()->resizeSection(ListLogger::c_lineColumn, 50);
    header()->setStretchLastSection(false);
}

void ListLogger::Clear()
{
    ListLogger *listLogger = ListLogger::GetInstance(); ASSERT(listLogger);
    listLogger->clear();
    listLogger->m_currentMessageRowList.Clear();
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
    scrollToBottom();
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
    AddRow(m_logIcon, str, line, fileName);
}

void ListLogger::OnAddWarn(const String &str, int line, const String &fileName)
{
    AddRow(m_warnIcon, str, line, fileName);
    DecorateLastItem( Color(1.0f, 1.0f, 0.3f) );
}

void ListLogger::OnAddError(const String &str, int line, const String &fileName)
{
    AddRow(m_errorIcon, str, line, fileName);
    DecorateLastItem( Color(1.0f, 0.3f, 0.3f) );
}

void ListLogger::OnClear() const
{
    ListLogger::Clear();
}

void ListLogger::AddRow(const QIcon &icon, const String &msg,
                        int line, const String &fileName, bool uniqueMessage)
{
    MessageRow msgRow;
    msgRow.msg = msg; msgRow.line = line; msgRow.fileName = fileName;
    if (uniqueMessage)
    {
        if (m_currentMessageRowList.Contains(msgRow))
        {
            return;
        }
    }

    // Shorten the message of the new row, it its too long
    const int c_lengthLimit = 100;
    int shortMessageEnd = std::min(c_lengthLimit, int(msg.Length())-1);
    String shortMessage = msg.SubString(0, shortMessageEnd);
    if (msg.Length() >= c_lengthLimit)
    {
        shortMessage += "...";
    }

    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setIcon(c_iconColumn,     icon);
    item->setText(c_msgColumn,      shortMessage.ToQString());
    item->setText(c_lineColumn,     String::ToString(line).ToQString());
    item->setText(c_fileNameColumn, fileName.ToQString());
    addTopLevelItem(item);
    scrollToBottom();

    m_currentMessageRowList.Add(msgRow);
}


bool ListLogger::MessageRow::operator==(const ListLogger::MessageRow &rhs) const
{
    return line == rhs.line && fileName == rhs.fileName && msg == rhs.msg;
}
