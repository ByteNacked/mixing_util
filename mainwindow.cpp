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
    //////////////////////////////////////////////
    //////////   INNER FORMAT ////////////////////
    format.setChannelCount(2);
    format.setCodec("audio/pcm");
    format.setSampleType(QAudioFormat::SignedInt);
    format.setSampleRate(44100);
    format.setSampleSize(16);
    /////////////////////////////////////////////

    p = new PlaySound(format, this);
    d = new Decoder(format, this);

    //Started freq
    frequencyHz = 600;
    buffer1 = new QByteArray;
    buffer2 = new QByteArray;

    saveOutputFlag = false;
    isTestTonePlaying = false;
    mixWithFile = false;


    connect(this, SIGNAL(stopPlay()), p, SLOT(stop()));
    connect(d, SIGNAL(dataReady(Decoder *)), this, SLOT(onDecodingReady(Decoder *)));
}

MainWindow::~MainWindow()
{
    delete buffer1;
    delete buffer2;

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

    //MIX with file ON Set checkbox and boolean
    mixWithFile = true;
    ui->checkBox->setCheckState(Qt::Checked);

    if(buffer2) buffer2->clear();
    d->setFileName(musicFile);
    d->start(buffer2);
}

void MainWindow::onDecodingReady(Decoder *ptr) {
    Q_ASSERT(ptr);

    qDebug() << "MainWindow::onDecodingReady Decoding ready!  " << ptr;

    //QMessageBox msgBox;
    //msgBox.setText("Look like buffer copied!\n");
    //msgBox.exec();

}


void MainWindow::on_mixWithGeneratedButton_clicked()
{
    //TODO: MEMORY LEAK
    QByteArray *generatedData = new QByteArray;
    Generator g(format, *generatedData, 1000000, frequencyHz, this);

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
   Q_ASSERT(buffer2);

   resultBuffer.clear();

   if(!mixWithFile) {
       QByteArray genBuf;
       Generator g(format, genBuf, getDurationInUMsFromLength(format, buffer1->size()), frequencyHz);
       Mixer mixer(format, *buffer1, genBuf, resultBuffer);
       mixer.mixSounds();
   }
   else {
       Mixer mixer(format, *buffer1, *buffer2, resultBuffer);
       mixer.mixSounds();
   }

   Q_ASSERT(resultBuffer.data());
   Q_ASSERT(resultBuffer.size() != 0);

   //Play sounds
    playBuffer(&resultBuffer);

    WavSaver saver(format, resultBuffer);
    saver.saveFile(QString("output.wav"));
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    qDebug() << "MainWIndow::checkbox_clicked";
    mixWithFile = checked;
}

void MainWindow::on_stopPlayback_clicked()
{
   p->stop();
}
