QT        += core
CONFIG    += link_pkgconfig
PKGCONFIG += openssl libssl libcrypto

TARGET = aes-test
TEMPLATE = app


SOURCES += \
    main.cpp \
    aestest.cpp

HEADERS  += \
    aestest.h

