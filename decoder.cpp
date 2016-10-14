#include "decoder.h"


Decoder::Decoder(QAudioFormat desiredFormat, QObject *parent) : QObject(parent)
{
    decoder = new QAudioDecoder(this);
    decoder->setAudioFormat(desiredFormat);

    connect(decoder, SIGNAL(bufferReady()), this, SLOT(readBuffer()));
    connect(decoder, SIGNAL(finished()), this, SIGNAL(dataReady()));

}

void Decoder::setFileName(QString &fileName) {
    Q_ASSERT(&decoder);
    qDebug() << "in setfilename";
    decoder->setSourceFilename(fileName);
}


void Decoder::start(QByteArray *_data) {
    data = _data;
    qDebug() << "in start()";
    decoder->start();
}

QByteArray *Decoder::getData()
{
    qDebug() << "in getData";
    return data;
}

void Decoder::readBuffer() {
    QAudioBuffer buf(decoder->read());

    data->append(buf.data<char>(), buf.byteCount());
    //qDebug() << "in readBUffer()";
}
