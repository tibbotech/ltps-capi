cmake_minimum_required( VERSION 3.0 )

project( LTPS )

set( CMAKE_SYSTEM_NAME Linux )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -W -Wall -fdata-sections -ffunction-sections -feliminate-unused-debug-types -fno-var-tracking-assignments" )
set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--hash-style=gnu -Wl,--as-needed -Wl,--gc-sections" )
set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--hash-style=gnu -Wl,--as-needed -Wl,--gc-sections" )

if( CMAKE_BUILD_TYPE MATCHES Debug )
	set( CMAKE_BUILD_TYPE Debug )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -rdynamic" )
else()
	set( CMAKE_BUILD_TYPE Release )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--strip-all" )
	set( CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Wl,--strip-all" )
endif()

find_package( Doxygen )

if( DOXYGEN_FOUND )
	set( doxyfile ${PROJECT_BINARY_DIR}/Doxyfile )
	configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY )
  	add_custom_target ( doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating doxygen documentation" VERBATIM
	)
else( DOXYGEN_FOUND )
	message( FATAL_ERROR "Doxygen not found" )
endif()

set ( SOURCES
#		include/ltps.h
		include/global.h
		include/lutils.h
		include/drivers/cpin.h
		include/drivers/Ci2c.h
		include/drivers/Ci2c_smbus.h
		include/drivers/gpio.h
		include/drivers/Cspi.h
		include/drivers/Cuart.h
		include/tibbits/i2c/adc.h
		include/tibbits/i2c/dac.h
		include/tibbits/i2c/pic.h
		include/tibbits/i2c/light.h
		include/tibbits/i2c/temperature.h
		include/tibbits/i2c/humidity.h
		include/tibbits/i2c/pressure.h
		include/tibbits/i2c/accelerometer.h
		include/tibbits/i2c/potentiometer.h
		include/tibbits/i2c/portextender.h
#		src/main.cpp
		src/lutils.cpp
		src/drivers/cpin.cpp
		src/drivers/Ci2c.cpp
		src/drivers/Ci2c_smbus.cpp
		src/drivers/gpio.cpp
		src/drivers/Cspi.cpp
		src/drivers/Cuart.cpp
		src/tibbits/i2c/adc.cpp
		src/tibbits/i2c/dac.cpp
		src/tibbits/i2c/pic.cpp
		src/tibbits/i2c/light.cpp
		src/tibbits/i2c/temperature.cpp
		src/tibbits/i2c/humidity.cpp
		src/tibbits/i2c/pressure.cpp
		src/tibbits/i2c/accelerometer.cpp
		src/tibbits/i2c/potentiometer.cpp
		src/tibbits/i2c/portextender.cpp
    )

include_directories( include include/drivers )

add_custom_target ( Docs ALL DEPENDS doc )

#add_executable( ltps ${SOURCES} )

add_library ( Ltps SHARED ${SOURCES} )
add_library ( LtpsStatic STATIC ${SOURCES} )

set_target_properties( Ltps PROPERTIES SOVERSION "0.1" )

install( TARGETS Ltps DESTINATION /usr/lib )
install( TARGETS LtpsStatic DESTINATION /usr/lib )
 
install( DIRECTORY include/tibbits/i2c/ DESTINATION /usr/include/ltps/capi )
install( DIRECTORY include/drivers/ DESTINATION /usr/include/ltps/capi )

install( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs/man/man3 DESTINATION /usr/share/man )
install( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs/html DESTINATION /usr/share/docs/ltps/capi )

install( FILES LICENSE DESTINATION /usr/share/docs/ltps )

