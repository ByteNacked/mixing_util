#include "decoder.h"


Decoder::Decoder(QAudioFormat desiredFormat, QObject *parent) : QObject(parent)
{
    decoder = new QAudioDecoder(this);
    decoder->setAudioFormat(desiredFormat);

    connect(decoder, SIGNAL(bufferReady()), this, SLOT(readBuffer()));
    connect(decoder, SIGNAL(finished()), this, SLOT(reEmmitFinished()));

}

void Decoder::setFileName(QString &fileName) {
    Q_ASSERT(&decoder);
    decoder->stop();
    qDebug() << "in setfilename";
    decoder->setSourceFilename(fileName);
}


void Decoder::start(QByteArray *_data) {
    Q_ASSERT(_data);
    data = _data;
    qDebug() << "Decoder::start()";
    decoder->start();
}

void Decoder::readBuffer() {
    QAudioBuffer buf(decoder->read());

    data->append(buf.data<char>(), buf.byteCount());
    //qDebug() << "in readBUffer()";
}


void Decoder::reEmmitFinished() {

    emit dataReady(this);
}
