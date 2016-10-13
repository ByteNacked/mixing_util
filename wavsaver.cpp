#include "wavsaver.h"

#include <QFile>


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
      array(_array),
      QObject(parent)
{
   Q_ASSERT(array.data());
}

void WavSaver::SaveFile(QString &fileName) {
    //add header

    CombinedHeader header = {};

    //Byte order LittleEndian
    memcpy(&header.riff.descriptor.id, "RIFF", 4);
    //memcpy(&header.riff.descriptor.id, "RIFX", 4);
    memcpy(&header.riff.type, "WAVE", 4);
    memcpy(&header.wave.descriptor.id, "fmt ", 4);
    //TODO: Double check this
    memcpy(&header.wave.audioFormat, "\0\0", 2);

    header.wave.descriptor.size = sizeof(WAVEHeader);

    DATAHeader dataHeader = {};

    header.wave.bitsPerSample = 16;
    header.wave.numChannels  = 2;
    header.wave.sampleRate = 44100;

    //assemle file
    QByteArray file;
    file.append((char *)&header, sizeof(header));
    file.append((char *)&dataHeader, sizeof(header));

    file.append(array);

    //write on disk
    QFile f(fileName);
    f.open(QIODevice::WriteOnly);
    f.write(file);
    f.close();

}










