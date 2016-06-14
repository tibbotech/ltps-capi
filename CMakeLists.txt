cmake_minimum_required( VERSION 2.8 )

project( LTPSDEMO )

set( CMAKE_SYSTEM_NAME Linux )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -W -Wall -fdata-sections -ffunction-sections -feliminate-unused-debug-types -fno-var-tracking-assignments" )
set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--hash-style=gnu -Wl,--as-needed -Wl,--gc-sections" )

if( CMAKE_BUILD_TYPE MATCHES Debug)
	set( CMAKE_BUILD_TYPE Debug )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -rdynamic" )
else()
	set( CMAKE_BUILD_TYPE Release )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--strip-all" )
endif()

set ( SOURCES
		include/ltps.h
		include/global.h
		include/utilities.h
		include/drivers/cpin.h
		include/drivers/ci2c.h
		include/drivers/ci2c_smbus.h
		include/tibbits/i2c/accelerometer.h
		include/tibbits/i2c/adc.h
		include/tibbits/i2c/dac.h
		src/main.cpp
		src/utilities.cpp
		src/drivers/cpin.cpp
		src/drivers/ci2c.cpp
		src/drivers/ci2c_smbus.cpp
		src/tibbits/i2c/accelerometer.cpp
		src/tibbits/i2c/adc.cpp
		src/tibbits/i2c/dac.cpp
    )

include_directories( include )

add_executable( ltps_demo ${SOURCES} )

