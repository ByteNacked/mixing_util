#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "decoder.h"
#include "playsound.h"
#include "spectrum/frequencyspectrum.h"
#include "spectrum/spectrograph.h"
#include "spectrum/spectrumanalyser.h"

#include <QMainWindow>
#include <QMediaPlayer>
#include <QThread>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onDecodingReady(Decoder *ptr);
    void on_loadVoiceButton_clicked();
    void on_loadMusicButton_clicked();
    void on_mixWithGeneratedButton_clicked();
    void on_dial_valueChanged(int value);
    void on_mixSoundsButton_clicked();
    void on_checkBox_clicked(bool checked);
    void on_stopPlayback_clicked();

    void calculateSpectrum();
    void spectrumChanged(const FrequencySpectrum &);

private:

    void playBuffer(QByteArray *buffer);

private:

    QString musicFile;
    QString voiceFile;
    QAudioFormat format;
    QByteArray *buffer1, *buffer2, resultBuffer;
    PlaySound *p;
    Decoder *d;

    int frequencyHz;
    QMediaPlayer *player;
    Ui::MainWindow *ui;

    bool saveOutputFlag;
    bool isTestTonePlaying;
    bool mixWithFile;

    Spectrograph spectrograph;
    SpectrumAnalyser spectrumAnalyser;
    quint64 spectrumBufferLength;
    QByteArray spectrumBuffer;
    quint64 spectrumPosition;
    QTimer *specTimer;


signals:
    void stopPlay();
    void spectrumChanged(/*qint64 position, qint64 length,*/ const FrequencySpectrum &spectrum, int fakeParam);
};

#endif // MAINWINDOW_H
