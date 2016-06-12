#include "ListLogger.h"
#include "WindowMain.h"

QListWidget *ListLogger::listLogger = nullptr;

ListLogger::ListLogger(QWidget *parent) : QListWidget(parent)
{
}

void ListLogger::AddLog(const std::string &str)
{
    listLogger = ListLogger::GetInstance();

    listLogger->addItem(QString::fromStdString("LOG:\t" + str));
    DecorateLastItem(QColor::fromRgb(255, 255, 255, 255));
}

void ListLogger::AddWarn(const std::string &str)
{
    listLogger = ListLogger::GetInstance();

    listLogger->addItem(QString::fromStdString("WARNING:\t" + str));
    DecorateLastItem(QColor::fromRgb(255, 255, 200, 255));
}

void ListLogger::AddError(const std::string &str)
{
    listLogger = ListLogger::GetInstance();

    listLogger->addItem(QString::fromStdString("ERROR:\t" + str));
    DecorateLastItem(QColor::fromRgb(255, 200, 200, 255));
}

ListLogger *ListLogger::GetInstance()
{
    return WindowMain::GetInstance()->listLogger;
}

void ListLogger::DecorateLastItem(const QColor &color)
{
    listLogger = ListLogger::GetInstance();

    QListWidgetItem *item = listLogger->item(listLogger->count()-1);
    if(item != nullptr)
    {
        item->setBackgroundColor(color);
    }

    listLogger->scrollToBottom();
}
