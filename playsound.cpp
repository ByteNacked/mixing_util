#include "playsound.h"
#include <QDebug>

PlaySound::PlaySound(QAudioFormat _format, QObject *parent)
    : QObject(parent),
      format(_format),
      iterator(0)
{
    audioOutput = new QAudioOutput(format, this);
    timer = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(playSegment()));
}

void PlaySound::play(QByteArray *_data) {
    data = _data;
    len = data->length();
    iterator = 0;
    Q_ASSERT(_data);
    Q_ASSERT(data->size() != 0);
    Q_ASSERT(len !=0);
    Q_ASSERT(audioOutput);

   //Stoping prev state
    stop();

    qDebug() << "in playsound::play";
    audioDevice =  audioOutput->start();

    timer->start(100);
}

void PlaySound::playSegment() {

    //qDebug() << iterator << " --- len  " << len;
    if (iterator < len && audioOutput && audioOutput->state() != QAudio::StoppedState) {

        if (audioOutput->bytesFree() >= audioOutput->periodSize()) {

            int multiplier = audioOutput->bytesFree() / audioOutput->periodSize();
            quint64 size = qMin<quint64>(multiplier * audioOutput->periodSize(), len  - iterator);
            iterator += audioDevice->write(data->data() + iterator, size);
        }
    }

    if (iterator >= len) {
        qDebug() << "PlaySound::playSegment stopping";
        this->stop();
    }

}


void PlaySound::stop() {

    timer->stop();
    //if (timer) delete timer;
    if (audioOutput->state() == QAudio::ActiveState)
        audioOutput->stop();
}

