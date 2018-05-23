#ifndef NESTCONFIGURE_H
#define NESTCONFIGURE_H

#include <QObject>

#define CONFG_FILE_PATH "nest_config.ini"

class NestConfigure : public QObject
{
    Q_OBJECT
public:
    NestConfigure();

private:

};

#endif // NESTCONFIGURE_H
