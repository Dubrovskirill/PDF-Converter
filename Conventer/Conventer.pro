QT += quick
CONFIG += c++17
QT += quick gui core printsupport
QT += testlib
QT += quick gui core printsupport testlib
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += C:/msys64/mingw64/include/poppler/qt5
LIBS += -LC:/msys64/mingw64/lib -lpoppler-qt5

INCLUDEPATH += src/core \
               src/viewmodel


SOURCES += \
    # main.cpp \
    src/core/PopplerPdfProcessor.cpp \
    src/test/TestPdfProcessor.cpp

HEADERS += \
    src/core/IPdfProcessor.h \
    src/core/PopplerPdfProcessor.h \
    src/core/interfaces/IPdfBuilder.h \
    src/core/interfaces/IPdfEditor.h \
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
