#ifndef RAWAUDIODATA_H
#define RAWAUDIODATA_H

#include <QObject>
#include <QAudioDecoder>
#include <QByteArray>

class Decoder : public QObject
{
    Q_OBJECT
public:
    explicit Decoder(QAudioFormat desiredFormat, QObject *parent = 0);
    QByteArray * getData();
    void setFileName(QString &fileName);
    void start();
private:
    QAudioDecoder *decoder;
    QByteArray * data;
signals:
    void dataReady();
public slots:
    void readBuffer();
};

#endif // RAWAUDIODATA_H
