#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>

#include <QApplication>
#include <QKeyEvent>

class Application final : public QApplication
{
public:
    Application(int& argc, char** argv);

    virtual bool notify(QObject *receiver, QEvent *e) override;
};

#endif // APPLICATION_H
