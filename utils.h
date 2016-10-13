#ifndef UTILS_H
#define UTILS_H

#include <qmath.h>
#include <QAudioFormat>

qint32 clamp(qint32 val, qint32 Min, qint32 Max);
qint64 getDurationInUMsFromLength(QAudioFormat &format, qint64 lengthInBytes);
qint64 getLengthFromDurationUms(QAudioFormat &format, qint64 durationUMs);


#endif // UTILS_H
