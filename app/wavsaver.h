#ifndef WAVSAVER_H
#define WAVSAVER_H

#include <QAudioFormat>



class WavSaver : public QObject
{
    Q_OBJECT
public:
    explicit WavSaver(QAudioFormat &format, QByteArray &data, QObject *parent = 0);
    void saveFile(QString fileName) ;
private:
    QByteArray &data;
    QAudioFormat &format;
signals:

public slots:
};

#endif // WAVSAVER_H
