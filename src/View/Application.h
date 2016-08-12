#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <iostream>

class Application final : public QApplication
{
public:
    Application(int& argc, char** argv);

    virtual bool notify(QObject *receiver, QEvent *e) override;
};

#endif // APPLICATION_H
