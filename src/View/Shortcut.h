#ifndef SHORTCUT_H
#define SHORTCUT_H

#include <QShortcut>
#include <QKeySequence>

#include "Bang/String.h"

typedef QKeySequence KSeq;

class Shortcut : public QShortcut
{
public:
    Shortcut(QWidget *parent,
             const QKeySequence &keySequence, const String &slot);
};

class LocalShortcut : public Shortcut
{
public:
    LocalShortcut(QWidget *parent,
                  const QKeySequence &keySequence, const String &slot);
};

class GlobalShortcut : public Shortcut
{
public:
    GlobalShortcut(QWidget *parent,
                   const QKeySequence &keySequence, const String &slot);
};

#endif // SHORTCUT_H
