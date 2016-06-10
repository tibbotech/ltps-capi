TEMPLATE		= app
CONFIG			-= app_bundle qt

CONFIG			+= debug_and_release

QMAKE_LFLAGS_DEBUG	+= -rdynamic

QMAKE_CXXFLAGS_RELEASE	+= -fdata-sections -ffunction-sections -feliminate-unused-debug-types -fno-var-tracking-assignments
QMAKE_LFLAGS_RELEASE	+= -Wl,--hash-style=gnu -Wl,--as-needed -Wl,--gc-sections -Wl,--strip-all

CONFIG(debug, release|debug):DEFINES += _DEBUG

INCLUDEPATH		+= include

LIBS			+=

HEADERS			+= \
                        include/ltps.h \
                        include/global.h \
                        include/utilities.h \
                        include/drivers/cpin.h \
                        include/drivers/ci2c.h \
                        include/drivers/ci2c_smbus.h \
                        include/tibbits/i2c/accelerometer.h

SOURCES			+= \
                        src/main.cpp \
                        src/utilities.cpp \
                        src/drivers/cpin.cpp \
                        src/drivers/ci2c.cpp \
                        src/drivers/ci2c_smbus.cpp \
                        src/tibbits/i2c/accelerometer.cpp
