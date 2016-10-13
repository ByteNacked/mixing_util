#ifndef WAVSAVER_H
#define WAVSAVER_H

#include <QAudioFormat>



class WavSaver : public QObject
{
    Q_OBJECT
public:
    explicit WavSaver(QAudioFormat &format, QByteArray &array, QObject *parent = 0);
    void WavSaver::SaveFile(QString &fileName) ;
private:
    QByteArray &array;
    QAudioFormat &format;
signals:

public slots:
};

#endif // WAVSAVER_H
