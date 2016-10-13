#include "playsound.h"
#include <QAudioOutput>
#include <QDebug>
#include <QTimer>

PlaySound::PlaySound(QByteArray * data, QAudioFormat _format, QObject *parent)
    : QObject(parent),
      rawdata(data),
      format(_format),
      len(data->length()),
      iterator(0)
{

}

void PlaySound::play() {
    Q_ASSERT(rawdata != 0 && len !=0);

    qDebug() << "in playsound::play";
    audioOutput = new QAudioOutput(format, this);
    audioDevice =  audioOutput->start();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(playSegment()));
    timer->start(20);
}

void PlaySound::playSegment() {

    if (iterator < len && audioOutput && audioOutput->state() != QAudio::StoppedState) {

        if (audioOutput->bytesFree() >= audioOutput->periodSize()) {

            int multiplier = audioOutput->bytesFree() / audioOutput->periodSize();
            iterator += audioDevice->write(rawdata->data() + iterator, multiplier * audioOutput->periodSize());
        }
    }

}


void PlaySound::stop() {
    audioOutput->stop();
    audioOutput->disconnect(this);
}

