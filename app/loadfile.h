#ifndef LOADFILE_H
#define LOADFILE_H

#include <QObject>

class LoadFile : public QObject
{
    Q_OBJECT
public:
    explicit LoadFile(QObject *parent = 0);
    QString getFileName();
private:
    QString fileName;

signals:

public slots:
};

#endif // LOADFILE_H
