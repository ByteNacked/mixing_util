#include "generator.h"
#include "loadfile.h"
#include "mainwindow.h"
#include "playsound.h"
#include "ui_mainwindow.h"
#include "wavfile.h"
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

    frequencyHz = 600;
    d1 = 0;
    d2 = 0;
    rawdata1 = 0;
    rawdata2 = 0;

    desiredFormat.setChannelCount(2);
    desiredFormat.setCodec("audio/pcm");
    desiredFormat.setSampleType(QAudioFormat::SignedInt);
    desiredFormat.setSampleRate(44100);
    desiredFormat.setSampleSize(16);



    WavFile * wavFile = new WavFile(this);
    wavFile->open("Sound.wav");
    QAudioFormat f = wavFile->fileFormat();

    qDebug() << f.channelCount();
    qDebug() << f.sampleRate();
    qDebug() << f.sampleSize();
    qDebug() << f.codec();



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

    if (d1)
        delete d1;
    d1 = new Decoder(desiredFormat);
    connect(d1, SIGNAL(dataReady()), this, SLOT(onDecodingReady()));
    d1->setFileName(voiceFile);
    d1->start();
}

void MainWindow::on_loadMusicButton_clicked()
{
    //todo emit signal stop play
    LoadFile *lf = new LoadFile(this);
    musicFile =  QString(lf->getFileName());
    ui->musicLineEdit->setText(musicFile);

    if (d2)
        delete d2;
}

void MainWindow::onDecodingReady() {
    Q_ASSERT(d1 != 0);

    if (rawdata1)
        delete rawdata1;

    rawdata1 = d1->getData();

    //QMessageBox msgBox;
    //msgBox.setText("Look like buffer copied!\n");
    //msgBox.exec();

    //PlaySound *p = new PlaySound(rawdata1, desiredFormat, this);
    //p->play();
}


void MainWindow::on_mixWithGeneratedButton_clicked()
{
    //check for memory leak
    Generator *g = new Generator(desiredFormat, 1000000, frequencyHz, this);
    QByteArray *generatedData = g->getData();

    PlaySound *p = new PlaySound(generatedData, desiredFormat, this);
    //fix this
    connect(this, SIGNAL(stopPlay()), p, SLOT(stop()));
    p->play();
}

void MainWindow::on_dial_valueChanged(int value)
{
    emit stopPlay();
    frequencyHz = value;
    ui->current_hz->setText(QString::number(value));
    //on_mixWithGeneratedButton_clicked();
}

void MainWindow::on_mixSoundsButton_clicked()
{
   Q_ASSERT(rawdata1);
   rawresult.clear();

   Generator g(desiredFormat, getDurationInUMsFromLength(desiredFormat, rawdata1->size()), frequencyHz);
   rawdata2 = g.getData();

   Q_ASSERT(rawdata2);

   Mixer mixer(desiredFormat, *rawdata1, *rawdata2, rawresult);
   mixer.mixSounds();

   Q_ASSERT(rawresult.data());

   //Play sounds
    PlaySound *p = new PlaySound(&rawresult, desiredFormat, this);
    p->play();

    WavSaver saver(desiredFormat, rawresult);
    saver.SaveFile(QString("output.wav"));

}
