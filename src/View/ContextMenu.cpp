#include "ContextMenu.h"

ContextMenu::ContextMenu(QWidget *widget)
{
    widget->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

    connect(widget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(OnCustomContextMenuRequested(QPoint)));
}
