#include "wavsaver.h"

#include <QFile>
#include <QDebug>


struct chunk
{
    char        id[4];
    quint32     size;
};

struct RIFFHeader
{
    chunk       descriptor;     // "RIFF"
    char        type[4];        // "WAVE"
};

struct WAVEHeader
{
    chunk       descriptor;
    quint16     audioFormat;
    quint16     numChannels;
    quint32     sampleRate;
    quint32     byteRate;
    quint16     blockAlign;
    quint16     bitsPerSample;
};

struct DATAHeader
{
    chunk       descriptor;
};

struct CombinedHeader
{
    RIFFHeader  riff;
    WAVEHeader  wave;
};

WavSaver::WavSaver(QAudioFormat &_format, QByteArray &_array, QObject *parent)
    : format(_format),
      data(_array),
      QObject(parent)
{
   Q_ASSERT(data.data());
}

void WavSaver::saveFile(QString fileName) {
    qDebug() << "WavSaver::saveFile  " << fileName << " Started.";
    //add header
    CombinedHeader header = {};

    //RIFF chunk descriptor
    memcpy(&header.riff.descriptor.id, "RIFF", 4);
    memcpy(&header.riff.type, "WAVE", 4);
    header.riff.descriptor.size = sizeof(CombinedHeader) + sizeof(DATAHeader) + data.size();

    //fmt sub-chunk
    memcpy(&header.wave.descriptor.id, "fmt ", 4);
    //16 for PCM.  rest of bytes in this header (16 bytes)
    header.wave.descriptor.size = 16;
    //PCM = 1 (Linear quantization)
    header.wave.audioFormat = 1;
    header.wave.numChannels = static_cast<quint16>(format.channelCount());
    header.wave.sampleRate = static_cast<quint32>(format.sampleRate());
    int byteRate = format.sampleRate() * format.channelCount() * format.sampleSize()/8;
    header.wave.byteRate = static_cast<quint32>(byteRate);
    header.wave.blockAlign = static_cast<quint16>(format.channelCount() * format.sampleSize()/8);
    header.wave.bitsPerSample = static_cast<quint16>(format.sampleSize());


    //data subchunk
    DATAHeader dataHeader = {};
    memcpy(&dataHeader.descriptor.id, "data", 4);
    dataHeader.descriptor.size = data.size();

    //Assemble file
    qDebug() << "WavSaver::saveFile Data is ready, assembling file";
    QByteArray file;
    file.append((char *)&header, sizeof(header));
    file.append((char *)&dataHeader, sizeof(dataHeader));
    file.append(data);

    qDebug() << "WavSaver::saveFile Starting write on disk";
    //write on disk
    QFile f(fileName);
    if (!f.open(QIODevice::WriteOnly)) {
        qDebug() << "WavSaver::saveFile file= " << fileName;
        qDebug() << "Cannot open file!" << fileName;

        return;
    }
    int bytesWritten = 0;
    if ((bytesWritten = f.write(file)) == -1)
        qDebug() << "WavSaver::saveFile Error cannot write in file";
    qDebug() << "WavSaver::SaveFile  File" << fileName << " succesfully written, bytes: " << bytesWritten;
    f.close();
}










