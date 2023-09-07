cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD                11)
set(CMAKE_C_STANDARD_REQUIRED       ON)
set(CMAKE_C_EXTENSIONS              ON)
set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            ON)
set(PROJ_PATH                       ${CMAKE_CURRENT_SOURCE_DIR})
message("Build type: "              ${CMAKE_BUILD_TYPE})

#
# Core project settings
#
project(vibootloader)
enable_language(C CXX ASM)
enable_testing()

# Set linker script
set(linker_script_SRC               ${PROJ_PATH}/generated/gcc/STM32F429ZITX_FLASH.ld)
set(EXECUTABLE                      vibootloader)

set_property(GLOBAL PROPERTY sources_list)
set_property(GLOBAL PROPERTY include_paths_list)
set_property(GLOBAL PROPERTY tst_sources_list)
set_property(GLOBAL PROPERTY tst_include_paths_list)

#
# Symbols definition
#
set(symbols_SYMB
    # Put here your symbols (preprocessor defines), one in each line
    # Encapsulate them with double quotes for safety purpose
    USE_FULL_ASSERT 
    USE_HAL_DRIVER 
    STM32F429xx
)

add_subdirectory(${PROJ_PATH}/generated)
add_subdirectory(${PROJ_PATH}/modules/unity)
add_subdirectory(${PROJ_PATH}/modules/vistmachine)
add_subdirectory(${PROJ_PATH}/modules/viflashdrv)
add_subdirectory(${PROJ_PATH}/modules/vistm32helpers)

get_property(sources GLOBAL PROPERTY sources_list)
get_property(includes GLOBAL PROPERTY include_paths_list)
get_property(tst_sources GLOBAL PROPERTY tst_sources_list)
get_property(tst_includes GLOBAL PROPERTY tst_include_paths_list)

# Executable files
add_executable(${EXECUTABLE} ${PROJ_PATH}/main/main.c ${sources})

# Include paths
target_include_directories(${EXECUTABLE} PRIVATE ${includes})

# Project symbols
target_compile_definitions(${EXECUTABLE} PRIVATE ${symbols_SYMB})

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    # Full debug configuration
    -Og -g3 -ggdb
)

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
    -T${linker_script_SRC}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    --specs=nosys.specs
    -u _printf_float                # STDIO float formatting support
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -Wl,--print-memory-usage
)

target_link_libraries(${EXECUTABLE} PRIVATE "${PROJ_PATH}/generated/Middlewares/ST/touchgfx/lib/core/cortex_m4f/gcc/libtouchgfx-float-abi-hard.a")
target_link_libraries(${EXECUTABLE} PRIVATE vistmachine viflashdrv)

# Execute post-build to print size
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
)

# Convert output to hex and binary
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex
)

# Convert to bin file -> add conditional check?
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin
)

#
# Test project settings
#
project(tst_vibootloader)

#include(CodeCoverage)
include(CTest)

enable_language(C CXX ASM)

set(TST_EXECUTABLE tst_vibootloader)
set_property(GLOBAL PROPERTY sources_list)
set_property(GLOBAL PROPERTY include_paths_list)

#
# Symbols definition
#
#set(symbols_SYMB
    # Put here your symbols (preprocessor defines), one in each line
    # Encapsulate them with double quotes for safety purpose
#)

# Executable files
add_executable(${TST_EXECUTABLE} ${PROJ_PATH}/tests/main/main.c ${tst_sources})

# Include paths
target_include_directories(${TST_EXECUTABLE} PRIVATE ${tst_includes})

# Compilation definition information
target_compile_definitions(${TST_EXECUTABLE} PUBLIC
    WIN32
    _DEBUG
    CONSOLE
)

# Compiler options
target_compile_options(${TST_EXECUTABLE} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

target_link_libraries(${TST_EXECUTABLE} unity)
add_test(NAME tst_vibootloader COMMAND tst_vibootloader)


