#include "ListLogger.h"
#include "WindowMain.h"

ListLogger::ListLogger(QWidget *parent) : QListWidget(parent)
{
}

void ListLogger::AddLog(const std::string &str)
{
    QListWidget *listLogger = WindowMain::GetInstance()->widgetLogger;
    listLogger->addItem(QString::fromStdString(str));
    listLogger->scrollToBottom();
}
