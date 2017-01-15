#include "ListLogger.h"

#include "Debug.h"
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

    // Set headers
    setHeaderHidden(false);
    setHeaderLabels( {"", "", "Message", "Line", "FileName"} );
    header()->setResizeMode(ListLogger::c_iconColumn, QHeaderView::ResizeToContents);
    header()->setResizeMode(ListLogger::c_msgColumn,  QHeaderView::Stretch);
    header()->setResizeMode(ListLogger::c_lineColumn, QHeaderView::Fixed);
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
    listLogger->AddRow(listLogger->m_logIcon, str, line, fileName);
}

void ListLogger::AddWarn(const String &str, int line, const String &fileName)
{
    ListLogger *listLogger = ListLogger::GetInstance(); ASSERT(listLogger);
    listLogger->AddRow(listLogger->m_warnIcon, str, line, fileName);
    DecorateLastItem(QColor::fromRgb(255, 255, 100, 255));
}

void ListLogger::AddError(const String &str, int line, const String &fileName)
{
    ListLogger *listLogger = ListLogger::GetInstance(); ASSERT(listLogger);
    listLogger->AddRow(listLogger->m_errorIcon, str, line, fileName);
    DecorateLastItem(QColor::fromRgb(255, 100, 100, 255));
}

void ListLogger::DecorateLastItem(const QColor &color)
{
    ListLogger *listLogger = ListLogger::GetInstance(); ASSERT(listLogger);

    QTreeWidgetItem *item =
            listLogger->topLevelItem(listLogger->topLevelItemCount() - 1);
    if (item)
    {
        //item->setBackgroundColor(c_msgColumn, color);
        item->setTextColor(c_msgColumn, color);
    }

    listLogger->scrollToBottom();
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

    int shortMessageEnd = std::min(100, int(msg.Length())-1);
    String shortMessage = msg.SubString(0, shortMessageEnd) + "...";

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
