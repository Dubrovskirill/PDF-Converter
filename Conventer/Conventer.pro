QT += quick gui core printsupport testlib
CONFIG += c++17

# Настройки QPDF
QPDF_DIR = $$PWD/3rdparty/qpdf
INCLUDEPATH += $$QPDF_DIR/include
LIBS += -L$$QPDF_DIR/lib -lqpdf

# --- АВТОМАТИЧЕСКОЕ КОПИРОВАНИЕ DLL ---
QPDF_BIN = $$PWD/3rdparty/qpdf/bin
# Преобразуем пути в формат Windows (с обратными слешами)
WIN_BIN = $$replace(QPDF_BIN, /, \\)
WIN_OUT = $$replace(OUT_PWD, /, \\)

# Команда xcopy: /Y (без подтверждения), /I (если папки нет - создать)
# Копируем и в корень билда, и в папки debug/release, где обычно сидят тесты
copy_dlls.commands = xcopy /Y /I \"$$WIN_BIN\\*.dll\" \"$$WIN_OUT\\\" && \
                     xcopy /Y /I \"$$WIN_BIN\\*.dll\" \"$$WIN_OUT\\debug\\\" && \
                     xcopy /Y /I \"$$WIN_BIN\\*.dll\" \"$$WIN_OUT\\release\\\"

first.depends = $(first) copy_dlls
export(first.depends)
export(copy_dlls.commands)
QMAKE_EXTRA_TARGETS += first copy_dlls
# ---------------------------------------

INCLUDEPATH += src/core \
               src/viewmodel

SOURCES += \
    # main.cpp \
    src/test/TestQPdfMerger.cpp \
    src/test/TestQtImageConverter.cpp

HEADERS += \
    src/core/QPdfMerger.h \
    src/core/QtImageConverter.h \
    src/core/interfaces/IImageConverter.h \
    src/core/interfaces/IPdfEditor.h \
    src/core/interfaces/IPdfMerger.h \
    src/core/interfaces/IPdfRenderer.h

RESOURCES += qml.qrc
QML_IMPORT_PATH = src/gui
