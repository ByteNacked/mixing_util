#-------------------------------------------------
#
# Project created by QtCreator 2016-10-09T04:20:45
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mixing_util
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    loadfile.cpp \
    mixer.cpp \
    playsound.cpp \
    generator.cpp \
    decoder.cpp \
    utils.cpp \
    wavsaver.cpp \
    spectrum/frequencyspectrum.cpp \
    spectrum/spectrograph.cpp \
    spectrum/spectrumanalyser.cpp

HEADERS  += mainwindow.h \
    loadfile.h \
    mixer.h \
    playsound.h \
    generator.h \
    decoder.h \
    utils.h \
    wavsaver.h \
    spectrum/frequencyspectrum.h \
    spectrum/spectrograph.h \
    spectrum/spectrum.h \
    spectrum/spectrumanalyser.h

FORMS    += mainwindow.ui
CONFIG   += c++11

fftreal_dir = ../3rdparty/fftreal

INCLUDEPATH += $${fftreal_dir}

# Dynamic linkage against FFTReal DLL
!contains(DEFINES, DISABLE_FFT) {
    macx {
        # Link to fftreal framework
        LIBS += -F$${fftreal_dir}
        LIBS += -framework fftreal
    } else {
        LIBS += -L..$${mixing_util_build_dir}
        LIBS += -lfftreal
    }
}

#target.path = $$[QT_INSTALL_EXAMPLES]/multimedia/spectrum
INSTALLS += target

CONFIG += install_ok  # Do not cargo-cult this!

# Deployment

DESTDIR = ..$${spectrum_build_dir}
macx {
    !contains(DEFINES, DISABLE_FFT) {
        # Relocate fftreal.framework into spectrum.app bundle
        framework_dir = ../spectrum.app/Contents/Frameworks
        framework_name = fftreal.framework/Versions/1/fftreal
        QMAKE_POST_LINK = \
            mkdir -p $${framework_dir} &&\
            rm -rf $${framework_dir}/fftreal.framework &&\
            cp -R $${fftreal_dir}/fftreal.framework $${framework_dir} &&\
            install_name_tool -id @executable_path/../Frameworks/$${framework_name} \
                                $${framework_dir}/$${framework_name} &&\
            install_name_tool -change $${framework_name} \
                                @executable_path/../Frameworks/$${framework_name} \
                                ../spectrum.app/Contents/MacOS/spectrum
    }
} else {
    linux-g++*: {
        # Provide relative path from application to fftreal library
        QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
    }
}
