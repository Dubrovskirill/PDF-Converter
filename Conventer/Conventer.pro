QT += quick gui core printsupport testlib
CONFIG += c++17

# Настройки путей проекта
INCLUDEPATH += src/core \
               src/viewmodel \
               src/infrastructure

# --- БИБЛИОТЕКА QPDF ---
QPDF_DIR = $$PWD/3rdparty/qpdf
INCLUDEPATH += $$QPDF_DIR/include
LIBS += -L$$QPDF_DIR/lib -lqpdf

# --- БИБЛИОТЕКА POPPLER ---
POPPLER_DIR = $$PWD/3rdparty/poppler
INCLUDEPATH += $$POPPLER_DIR/include/poppler/qt5 \
               $$POPPLER_DIR/include/poppler
LIBS += -L$$POPPLER_DIR/lib -lpoppler-qt5

# --- АВТОМАТИЧЕСКОЕ КОПИРОВАНИЕ DLL ---
QPDF_BIN_WIN = $$replace(QPDF_DIR, /, \\)\\bin
POPPLER_BIN_WIN = $$replace(POPPLER_DIR, /, \\)\\bin
OUT_PWD_WIN = $$replace(OUT_PWD, /, \\)

# Копируем только те DLL, которые не конфликтуют с Qt
# /Exclude не очень удобно использовать в xcopy, поэтому копируем по маскам
copy_dlls.commands = xcopy /Y /I \"$$QPDF_BIN_WIN\\*.dll\" \"$$OUT_PWD_WIN\\debug\\\" && \
                     xcopy /Y /I \"$$POPPLER_BIN_WIN\\lib*.dll\" \"$$OUT_PWD_WIN\\debug\\\" && \
                     xcopy /Y /I \"$$POPPLER_BIN_WIN\\nss*.dll\" \"$$OUT_PWD_WIN\\debug\\\" && \
                     xcopy /Y /I \"$$POPPLER_BIN_WIN\\zlib*.dll\" \"$$OUT_PWD_WIN\\debug\\\" && \
                     xcopy /Y /I \"$$POPPLER_BIN_WIN\\smime3.dll\" \"$$OUT_PWD_WIN\\debug\\\"

# Повторяем для release папки
copy_dlls.commands += && xcopy /Y /I \"$$QPDF_BIN_WIN\\*.dll\" \"$$OUT_PWD_WIN\\release\\\" \
                     && xcopy /Y /I \"$$POPPLER_BIN_WIN\\lib*.dll\" \"$$OUT_PWD_WIN\\release\\\" \
                     && xcopy /Y /I \"$$POPPLER_BIN_WIN\\nss*.dll\" \"$$OUT_PWD_WIN\\release\\\" \
                     && xcopy /Y /I \"$$POPPLER_BIN_WIN\\zlib*.dll\" \"$$OUT_PWD_WIN\\release\\\" \
                     && xcopy /Y /I \"$$POPPLER_BIN_WIN\\smime3.dll\" \"$$OUT_PWD_WIN\\release\\\"

first.depends = $(first) copy_dlls
QMAKE_EXTRA_TARGETS += first copy_dlls

# --- ФАЙЛЫ ПРОЕКТА ---
SOURCES += \
    src/test/TestAsyncTasks.cpp \
    src/test/TestPdfRenderer.cpp \
    src/test/TestQPdfMerger.cpp \
    src/test/TestQtImageConverter.cpp

HEADERS += \
    src/core/QPdfMerger.h \
    src/core/QtImageConverter.h \
    src/core/QtPdfRenderer.h \
    src/core/interfaces/IImageConverter.h \
    src/core/interfaces/IPdfEditor.h \
    src/core/interfaces/IPdfMerger.h \
    src/core/interfaces/IPdfRenderer.h \
    src/infrastructure/BaseTask.h \
    src/infrastructure/ImageToPdfTask.h \
    src/infrastructure/MergeTask.h \
    src/infrastructure/RenderTask.h \
    src/infrastructure/TaskQueueManager.h \
    src/infrastructure/TaskSignals.h

RESOURCES += qml.qrc
QML_IMPORT_PATH = src/gui
