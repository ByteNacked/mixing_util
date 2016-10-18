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
    void setFileName(QString &fileName);
    void start(QByteArray *_data);
private:
    QAudioDecoder *decoder;
    QByteArray * data;
signals:
    void dataReady(Decoder *);
public slots:
    void readBuffer();
    void reEmmitFinished();
};

#endif // RAWAUDIODATA_H
