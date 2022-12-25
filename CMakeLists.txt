cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(editor C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(editor
	main.c
	editor.c
	keyinput.c
	../usbkb/usbkeyboard.c
	../usbkb/hid_virtual_keytable.c
	../lcd-lib/ili9341_spi.c
	../lcd-lib/graphlib.c
	../lcd-lib/fontdata.c
	../lcd-lib/ff.c
	../lcd-lib/pico-sdmm.c
)
target_include_directories(editor PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Pull in basic dependencies
target_link_libraries(editor pico_stdlib tinyusb_host tinyusb_board hardware_spi pico_multicore)

# create map/bin/hex file etc.
pico_add_extra_outputs(editor)
