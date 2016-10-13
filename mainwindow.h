#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "decoder.h"

#include <QMainWindow>
#include <QMediaPlayer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString musicFile;
    QString voiceFile;
    QAudioFormat desiredFormat;

    QByteArray *rawdata1, *rawdata2, rawresult;
    Decoder *d1, *d2;


private slots:
    void onDecodingReady();
    void on_loadVoiceButton_clicked();
    void on_loadMusicButton_clicked();

    void on_mixWithGeneratedButton_clicked();

    void on_dial_valueChanged(int value);

    void on_mixSoundsButton_clicked();

private:

    int frequencyHz;
    QMediaPlayer *player;
    Ui::MainWindow *ui;
signals:
    void stopPlay();
};

#endif // MAINWINDOW_H
