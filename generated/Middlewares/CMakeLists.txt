file(GLOB CORE_SRCS 
  ${CMAKE_CURRENT_SOURCE_DIR}/ST/STM32_USB_Device_Library/Class/MSC/Src/*.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ST/STM32_USB_Device_Library/Core/Src/*.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Third_Party/FatFs/src/*.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Third_Party/FatFs/src/option/*.c
#  ${CMAKE_CURRENT_SOURCE_DIR}/ST/touchgfx/framework/source/platform/driver/touch/*.cpp
#  ${CMAKE_CURRENT_SOURCE_DIR}/ST/touchgfx/framework/source/platform/hal/simulator/sdl2/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ST/touchgfx/framework/source/touchgfx/containers/clock/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ST/touchgfx/framework/source/touchgfx/containers/progress_indicators/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ST/touchgfx/framework/source/touchgfx/containers/scrollers/*.cpp
  )

get_property(tmp GLOBAL PROPERTY sources_list)

foreach(source_file IN LISTS CORE_SRCS)
   set(tmp "${tmp}${source_file};")
endforeach()

set_property(GLOBAL PROPERTY sources_list "${tmp}")

get_property(tmp GLOBAL PROPERTY include_paths_list)
set(tmp "${tmp}\
${CMAKE_CURRENT_SOURCE_DIR}/ST/STM32_USB_Device_Library/Class/MSC/Inc;\
${CMAKE_CURRENT_SOURCE_DIR}/ST/STM32_USB_Device_Library/Core/Inc;\
${CMAKE_CURRENT_SOURCE_DIR}/ST/touchgfx/framework/include;\
${CMAKE_CURRENT_SOURCE_DIR}/Third_Party/FatFs/src;")

set_property(GLOBAL PROPERTY include_paths_list "${tmp}")