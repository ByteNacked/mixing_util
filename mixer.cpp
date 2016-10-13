#include "mixer.h"
#include "utils.h"

#include <qmath.h>
#include <QDebug>

Mixer::Mixer(QAudioFormat &_format,
             QByteArray &_sound1,
             QByteArray &_sound2,
             QByteArray &_result,
             QObject *parent,
             quint64 offset)
    : format(_format),
      sound1(_sound1),
      sound2(_sound2),
      result(_result),
      QObject(parent)
{
    Q_ASSERT(_sound1.size());
    Q_ASSERT(_sound2.size());
    Q_UNUSED(offset);

}

void Mixer::mixSounds() {
    //TODO: should i change it to qMax ??
    int newSize = qMin(sound1.size(), sound2.size());
    result.resize(newSize);

    //TODO: rewrite platform agnostic way ??
    char * psound1 = sound1.data();
    char * psound2 = sound2.data();
    char * presult = result.data();

    int iterator = 0;
    bool isClampedFlag = false;
    int bytesChan = format.sampleSize() / 8;
    Q_ASSERT(bytesChan == 1 || bytesChan == 2);

    //TODO: implement clamping for all branches
    while(newSize) {
        for(int i = 0; i < format.channelCount(); ++i) {
            if(format.sampleSize() == 8 && format.sampleType() == QAudioFormat::UnSignedInt) {
                *(presult + iterator) = *reinterpret_cast<quint8*>(psound1 + iterator) + *reinterpret_cast<quint8*>(psound2 + iterator);
            }

            if(format.sampleSize() == 8 && format.sampleType() == QAudioFormat::SignedInt) {
                *(presult + iterator) = *reinterpret_cast<qint8*>(psound1 + iterator) + *reinterpret_cast<qint8*>(psound2 + iterator);
            }

            if(format.sampleSize() == 16 && format.sampleType() == QAudioFormat::UnSignedInt) {
                *reinterpret_cast<quint16*>(presult + iterator)
                        = *reinterpret_cast<quint16*>(psound1 + iterator) + *reinterpret_cast<quint16*>(psound2 + iterator);
            }

            if(format.sampleSize() == 16 && format.sampleType() == QAudioFormat::SignedInt) {

                qint32 val1 = static_cast<qint32>(*reinterpret_cast<qint16*>(psound1 + iterator));
                qint32 val2 = static_cast<qint32>(*reinterpret_cast<qint16*>(psound2 + iterator));
                /*
                 *  TODO: Reduce generated tone volume in generator not here
                 * TODO: Delete Magic numbers
                 */
                qint32 res = val1 + val2/2;
                res = clamp(res, -32767, 32767);

                *reinterpret_cast<qint16*>(presult + iterator) = static_cast<qint16>(res);

            }

            iterator += bytesChan;
            newSize -= bytesChan;
        }
    }

    if (isClampedFlag) {
        qDebug() << "In Mixer::mixSounds Clamping Happend!!";
    }

    emit mixingReady();
}

