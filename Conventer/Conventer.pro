QT += quick
CONFIG += c++17
QT += quick gui core printsupport
QT += testlib
QT += quick gui core printsupport testlib
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += src/core \
               src/viewmodel


SOURCES += \
    # main.cpp \
    src/test/TestQtImageConverter.cpp

HEADERS += \
    src/core/QtImageConverter.h \
    src/core/interfaces/IImageConverter.h \
    src/core/interfaces/IPdfEditor.h \
    src/core/interfaces/IPdfMerger.h \
    src/core/interfaces/IPdfRenderer.h

RESOURCES += qml.qrc

QML_IMPORT_PATH = src/gui



# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
