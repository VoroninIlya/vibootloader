file(GLOB CORE_SRCS 
  ${CMAKE_CURRENT_SOURCE_DIR}/BSP/Components/ili9341/*.c
  ${CMAKE_CURRENT_SOURCE_DIR}/BSP/Components/stmpe811/*.c
  ${CMAKE_CURRENT_SOURCE_DIR}/STM32F4xx_HAL_Driver/Src/*.c)

get_property(tmp GLOBAL PROPERTY sources_list)

foreach(source_file IN LISTS CORE_SRCS)
   set(tmp "${tmp}${source_file};")
endforeach()

set_property(GLOBAL PROPERTY sources_list "${tmp}")

get_property(tmp GLOBAL PROPERTY include_paths_list)
set(tmp "${tmp}\
${CMAKE_CURRENT_SOURCE_DIR}/BSP;\
${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Include;\
${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Device/ST/STM32F4xx/Include;\
${CMAKE_CURRENT_SOURCE_DIR}/STM32F4xx_HAL_Driver/Inc;")
set_property(GLOBAL PROPERTY include_paths_list "${tmp}")