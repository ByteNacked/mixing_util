#include "playsound.h"
#include <QDebug>

PlaySound::PlaySound(QAudioFormat _format, QObject *parent)
    : QObject(parent),
      format(_format),
      iterator(0)
{
    audioOutput = new QAudioOutput(format, this);
    timer = 0;
}

void PlaySound::play(QByteArray *_data) {
    data = _data;
    len = data->length();
    Q_ASSERT(_data);
    Q_ASSERT(data->size() != 0);
    Q_ASSERT(len !=0);
    Q_ASSERT(audioOutput);

    if(audioOutput->state() == QAudio::ActiveState)
        audioOutput->stop();
    qDebug() << "in playsound::play";
    audioDevice =  audioOutput->start();


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(playSegment()));
    timer->start(20);
}

void PlaySound::playSegment() {

    if (iterator < len && audioOutput && audioOutput->state() != QAudio::StoppedState) {

        if (audioOutput->bytesFree() >= audioOutput->periodSize()) {

            int multiplier = audioOutput->bytesFree() / audioOutput->periodSize();
            iterator += audioDevice->write(data->data() + iterator, multiplier * audioOutput->periodSize());
        }
    }

}


void PlaySound::stop() {
    timer->disconnect();
    //if (timer) delete timer;

    audioOutput->stop();
}

