TEMPLATE		= app
CONFIG			-= app_bundle qt

CONFIG			+= debug_and_release

QMAKE_LFLAGS_DEBUG	+= -rdynamic

QMAKE_CXXFLAGS_RELEASE	+= -fdata-sections -ffunction-sections -feliminate-unused-debug-types -fno-var-tracking-assignments
QMAKE_LFLAGS_RELEASE	+= -Wl,--hash-style=gnu -Wl,--as-needed -Wl,--gc-sections -Wl,--strip-all

CONFIG(debug, release|debug):DEFINES += _DEBUG

INCLUDEPATH		+= include include/drivers

LIBS			+=

HEADERS			+= \
			include/ltps.h \
			include/global.h \
			include/lutils.h \
			include/drivers/cpin.h \
			include/drivers/Ci2c.h \
			include/drivers/Ci2c_smbus.h \
			include/drivers/gpio.h \
			include/drivers/Cspi.h \
			include/drivers/CWg.h \
			include/drivers/Cuart.h \
			include/tibbits/i2c/adc.h \
			include/tibbits/i2c/dac.h \
			include/tibbits/i2c/pic.h \
			include/tibbits/i2c/light.h \
			include/tibbits/i2c/temperature.h \
			include/tibbits/i2c/humidity.h \
			include/tibbits/i2c/pressure.h \
			include/tibbits/i2c/accelerometer.h \
			include/tibbits/i2c/potentiometer.h \
			include/tibbits/i2c/portextender.h \
			include/tibbits/i2c/adc-4-20.h \
			include/tibbits/i2c/adc-16bits.h \
			include/tibbits/spi/rtc.h

SOURCES			+= \
			src/main.cpp \
			src/lutils.cpp \
			src/drivers/cpin.cpp \
			src/drivers/Ci2c.cpp \
			src/drivers/Ci2c_smbus.cpp \
			src/drivers/gpio.cpp \
			src/drivers/Cspi.cpp \
			src/drivers/CWg.cpp \
			src/drivers/Cuart.cpp \
			src/tibbits/i2c/adc.cpp \
			src/tibbits/i2c/dac.cpp \
			src/tibbits/i2c/pic.cpp \
			src/tibbits/i2c/light.cpp \
			src/tibbits/i2c/temperature.cpp \
			src/tibbits/i2c/humidity.cpp \
			src/tibbits/i2c/pressure.cpp \
			src/tibbits/i2c/accelerometer.cpp \
			src/tibbits/i2c/potentiometer.cpp \
			src/tibbits/i2c/portextender.cpp \
			src/tibbits/i2c/adc-4-20.cpp \
			src/tibbits/i2c/adc-16bits.cpp \
			src/tibbits/spi/rtc.cpp
