#include "generator.h"
#include "loadfile.h"
#include "mainwindow.h"
#include "playsound.h"
#include "ui_mainwindow.h"
#include "mixer.h"
#include "utils.h"
#include "wavsaver.h"

#include <QMediaPlayer>
#include <QMessageBox>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    format.setChannelCount(2);
    format.setCodec("audio/pcm");
    format.setSampleType(QAudioFormat::SignedInt);
    format.setSampleRate(44100);
    format.setSampleSize(16);

    p = new PlaySound(format, this);
    d = new Decoder(format);

    frequencyHz = 600;
    buffer1 = new QByteArray;
    buffer2 = new QByteArray;

    saveOutputFlag = false;
    isTestTonePlaying = false;
    mixWithFile = false;


    connect(this, SIGNAL(stopPlay()), p, SLOT(stop()));
    connect(d, SIGNAL(dataReady()), this, SLOT(onDecodingReady()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadVoiceButton_clicked()
{
    //todo emit signal stop play
    LoadFile *lf = new LoadFile(this);
    voiceFile =  QString(lf->getFileName());
    ui->voiceLineEdit->setText(voiceFile);

    if(buffer1) buffer1->clear();
    d->setFileName(voiceFile);
    d->start(buffer1);
}

void MainWindow::on_loadMusicButton_clicked()
{
    //todo emit signal stop play
    LoadFile lf;
    musicFile =  QString(lf.getFileName());
    ui->musicLineEdit->setText(musicFile);

    if(buffer2) buffer2->clear();
    d->setFileName(musicFile);
    d->start(buffer2);
}

void MainWindow::onDecodingReady() {
    Q_ASSERT(d != 0);

    qDebug() << "MainWindow::onDecodingReady Decoding ready!";
    //QMessageBox msgBox;
    //msgBox.setText("Look like buffer copied!\n");
    //msgBox.exec();

}


void MainWindow::on_mixWithGeneratedButton_clicked()
{
    Generator g(format, 1000000, frequencyHz, this);
    //TODO: MEMORY LEAK
    QByteArray *generatedData = g.getData();

    playBuffer(generatedData);
    QThread::msleep(100);
}

void MainWindow::on_dial_valueChanged(int value)
{
    emit stopPlay();
    frequencyHz = value;
    ui->current_hz->setText(QString::number(value));
}

void MainWindow::playBuffer(QByteArray *buffer) {

    p->play(buffer);
}

void MainWindow::on_mixSoundsButton_clicked()
{
   Q_ASSERT(buffer1);
   resultBuffer.clear();

   if(!mixWithFile) {
       Generator g(format, getDurationInUMsFromLength(format, buffer1->size()), frequencyHz);
       buffer2 = g.getData();
   }

   Q_ASSERT(buffer2);

   Mixer mixer(format, *buffer1, *buffer2, resultBuffer);
   mixer.mixSounds();

   Q_ASSERT(resultBuffer.data());
   Q_ASSERT(resultBuffer.size() != 0);

   //Play sounds
    playBuffer(&resultBuffer);

    WavSaver saver(format, resultBuffer);
    saver.saveFile(QString("output.wav"));
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    mixWithFile = checked;
}

void MainWindow::on_stopPlayback_clicked()
{
   p->stop();
}
