#include "ListLogger.h"
#include "WindowMain.h"

QListWidget *ListLogger::s_listLogger = nullptr;

ListLogger::ListLogger(QWidget *parent) : QListWidget(parent)
{
}

void ListLogger::AddLog(const std::string &str)
{
    s_listLogger = ListLogger::GetInstance();

    s_listLogger->addItem(QString::fromStdString("LOG:\t" + str));
    s_listLogger->scrollToBottom();
}

void ListLogger::AddWarn(const std::string &str)
{
    s_listLogger = ListLogger::GetInstance();

    s_listLogger->addItem(QString::fromStdString("WARNING:\t" + str));
    DecorateLastItem(QColor::fromRgb(255, 255, 200, 255));
}

void ListLogger::AddError(const std::string &str)
{
    s_listLogger = ListLogger::GetInstance();

    s_listLogger->addItem(QString::fromStdString("ERROR:\t" + str));
    DecorateLastItem(QColor::fromRgb(255, 200, 200, 255));
}

ListLogger *ListLogger::GetInstance()
{
    return WindowMain::GetInstance()->listLogger;
}

void ListLogger::DecorateLastItem(const QColor &color)
{
    s_listLogger = ListLogger::GetInstance();
    QColor inverse(255 - color.red(),
                   255 - color.green(),
                   255 - color.blue());

    QListWidgetItem *item = s_listLogger->item(s_listLogger->count()-1);
    if (item )
    {
        item->setBackgroundColor(color);
        item->setTextColor(inverse);
    }

    s_listLogger->scrollToBottom();
}
