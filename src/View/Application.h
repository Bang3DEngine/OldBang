#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>

#include <QApplication>
#include <QKeyEvent>

class ShortcutManager;
class Application final : public QApplication
{
friend class ShortcutManager;
friend class IShortcutListener;

public:
    Application(int& argc, char** argv);

    static Application *GetInstance();
    virtual bool notify(QObject *receiver, QEvent *e) override;
};

#endif // APPLICATION_H
