#ifndef PLAYSOUND_H
#define PLAYSOUND_H

#include <QObject>
#include <QList>
#include <QAudioBuffer>
#include <QIODevice>
#include <QAudioOutput>
#include <QTimer>

class PlaySound : public QObject
{
    Q_OBJECT

public:
    explicit PlaySound(QAudioFormat _format, QObject *parent = 0);
    void play(QByteArray *_data);
private:
    QByteArray *data;
    quint64 iterator;
    quint64 len;
    QIODevice *audioDevice;
    QAudioOutput *audioOutput;
    QAudioFormat format;
    QTimer *timer;

signals:

public slots:
    void playSegment();
    void stop();
};

#endif // PLAYSOUND_H
