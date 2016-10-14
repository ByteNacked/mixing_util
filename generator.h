#ifndef GENERATOR_H
#define GENERATOR_H

#include <QObject>
#include <QAudioFormat>

class Generator : public QObject
{
    Q_OBJECT
public:
    Generator(const QAudioFormat &format,
                     qint64 durationUs,
                     int sampleRate,
                     QObject *parent = 0);
    ~Generator();
    QByteArray * getData();
private:
    void generateData(const QAudioFormat &format, qint64 durationUs, int sampleRate);
    qint64 m_pos;
    QByteArray *m_buffer;
signals:

public slots:
};

#endif // GENERATOR_H
