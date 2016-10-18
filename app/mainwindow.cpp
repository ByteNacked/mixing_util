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


//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

const qint64 BufferDurationUs       = 10 * 1000000;
const int    NotifyIntervalMs       = 100;
//-----------------------------------------------------------------------------



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    spectrograph(0),
    spectrumAnalyser(0),\
    spectrumPosition(0)

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

    //Boolean flags
    saveOutputFlag = false;
    isTestTonePlaying = false;
    mixWithFile = false;

    //Spectrograph init
    spectrumBufferLength = SpectrumLengthSamples * (format.sampleSize() / 8) * format.channelCount();
    spectrograph.setParams(30, 1, 1000);
    spectrograph.show();

    specTimer = new QTimer(this);
    connect(specTimer, SIGNAL(timeout()), SLOT(calculateSpectrum()));

    //Connections
    connect(this, SIGNAL(stopPlay()), p, SLOT(stop()));
    connect(d, SIGNAL(dataReady(Decoder *)), this, SLOT(onDecodingReady(Decoder *)));
    CHECKED_CONNECT(&spectrumAnalyser,
                    SIGNAL(spectrumChanged(FrequencySpectrum)),
                    this,
                    SLOT(spectrumChanged(FrequencySpectrum)));
    CHECKED_CONNECT(this,
            SIGNAL(spectrumChanged(FrequencySpectrum, int)),
            &spectrograph,
            SLOT(spectrumChanged(FrequencySpectrum)));
    CHECKED_CONNECT(&spectrograph,
                    SIGNAL(infoMessage(QString,int)),
                    &spectrograph,
                    SLOT(setWindowTitle(QString)));
}

MainWindow::~MainWindow()
{
    delete buffer1;
    delete buffer2;

    delete ui;

    qDebug() << "MainWindow Destructor";
    spectrograph.hide();
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

    specTimer->start(500);
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
   specTimer->stop();
   spectrumPosition = 0;
   p->stop();
}

void MainWindow::calculateSpectrum()
{
#ifdef DISABLE_SPECTRUM
#else
    quint64 position = p->getPosition();

//    Q_ASSERT(position + spectrumBufferLength <= bufferPosition + dataLength);
    Q_ASSERT(0 == spectrumBufferLength % 2); // constraint of FFT algorithm

    // QThread::currentThread is marked 'for internal use only', but
    // we're only using it for debug output here, so it's probably OK :)
    qDebug() << "Engine::calculateSpectrum" << QThread::currentThread()
                 << "pos" << position << "len" << spectrumBufferLength
                 << "spectrumAnalyser.isReady" << spectrumAnalyser.isReady();

    if (spectrumAnalyser.isReady()) {
        spectrumBuffer = QByteArray::fromRawData(p->getData() + position - spectrumPosition,
                                                   spectrumBufferLength);
        spectrumPosition = position;
        spectrumAnalyser.calculate(spectrumBuffer, format);
    }
#endif
}

void MainWindow::spectrumChanged(const FrequencySpectrum &spectrum)
{
    qDebug() << "Engine::spectrumChanged" << "pos"; // << m_spectrumPosition;
    emit spectrumChanged(/*spectrumPosition, spectrumBufferLength,*/ spectrum, 0);
}
