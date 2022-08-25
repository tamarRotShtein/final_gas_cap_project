QT -= gui

TEMPLATE = lib
DEFINES += GAS_CAM_API_LIBRARY

CONFIG += c++11

LIBS += -L/usr/lib/x86_64-linux-gnu  -lavcodec -lavformat -lavutil -lavcodec -ljpeg
INCLUDEPATH += /usr/include/x86_64-linux-gnu
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    capture.c \
    encode.c \
    gas_cam_api.c \
    handle_stages.c \
    queue.c \
    rgb_converter.c \
    save_snapshot.c \
    stage.c \
    yuv_convertor.c

HEADERS += \
    capture.h \
    encode.h \
    gas_cam_api.h \
    handle_stages.h \
    queue.h \
    record_t.h \
    rgb_converter.h \
    save_snapshot.h \
    snapshot_t.h \
    stage.h \
    yuv_convertor.h \
    jpeglib.h \
    avcodec.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
