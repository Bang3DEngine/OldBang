#include "Shortcut.h"

Shortcut::Shortcut(QWidget *parent,
                   const QKeySequence &keySequence, const String &slot)
    : QShortcut(keySequence, parent)
{
    setAutoRepeat(false);
    connect(this, SIGNAL(activated()),            parent, slot.ToCString());
    connect(this, SIGNAL(activatedAmbiguously()), parent, slot.ToCString());
}

LocalShortcut::LocalShortcut(QWidget *parent,
                          const QKeySequence &keySequence, const String &slot)
    : Shortcut(parent, keySequence, slot)
{
    setContext(Qt::ShortcutContext::WidgetWithChildrenShortcut);
}

GlobalShortcut::GlobalShortcut(QWidget *parent,
                           const QKeySequence &keySequence, const String &slot)
    : Shortcut(parent, keySequence, slot)
{
    setContext(Qt::ShortcutContext::ApplicationShortcut);
}
