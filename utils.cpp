#include "utils.h"


qint32 clamp(qint32 val, qint32 Min, qint32 Max)
{
   if(val > Max) val = Max;
   if(val < Min) val = Min;

   return val;
}


qint64 getDurationInUMsFromLength(QAudioFormat &format, qint64 lengthInBytes) {

   qint64 duration = lengthInBytes / (format.sampleRate() * format.channelCount() * (format.sampleSize() / 8)) * 100000;

   return duration;
}


qint64 getLengthFromDurationUms(QAudioFormat &format, qint64 durationUMs) {

    qint64 length = (format.sampleRate() * format.channelCount() * (format.sampleSize() / 8))
                        * durationUMs / 100000;


    return length;
}
