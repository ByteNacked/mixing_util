#include "loadfile.h"

#include <QFileDialog>

LoadFile::LoadFile(QObject *parent) : QObject(parent)
{
    fileName = QFileDialog::getOpenFileName(qobject_cast<QWidget *>(parent),
                                            tr("Open Image"),
                                            QString("../wav/"),
                                            "Sounds (*.wav *.mp3);; All (*.*)"
                                            );
}

QString LoadFile::getFileName() {

    return fileName;
}
