#ifndef PLAYSOUND_H
#define PLAYSOUND_H

#include <QObject>
#include <QList>
#include <QAudioBuffer>
#include <QIODevice>
#include <QAudioOutput>

class PlaySound : public QObject
{
    Q_OBJECT

public:
    explicit PlaySound(QByteArray * data, QAudioFormat _format, QObject *parent = 0);
    void play();

private:
    QByteArray *rawdata;
    quint64 iterator;
    quint64 len;
    QIODevice *audioDevice;
    QAudioOutput *audioOutput;
    QAudioFormat format;

signals:

public slots:
    void playSegment();
    void stop();
};

#endif // PLAYSOUND_H
