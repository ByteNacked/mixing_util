#ifndef MIXER_H
#define MIXER_H

#include <QAudioFormat>
#include <QObject>

class Mixer : public QObject
{
    Q_OBJECT
public:
    explicit Mixer(QAudioFormat &_format, QByteArray &sound1, QByteArray &sound2, QByteArray &_result, QObject *parent = 0, quint64 offset = 0);
private:
   QAudioFormat &format;
   QByteArray &sound1;
   QByteArray &sound2;
   QByteArray &result;

signals:
   void mixingReady();

public slots:

   /* Emit signal when mixing ready */
    void mixSounds();

};

#endif // MIXER_H
