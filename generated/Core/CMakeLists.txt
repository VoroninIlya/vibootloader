file(GLOB CORE_SRCS 
#  ${CMAKE_CURRENT_SOURCE_DIR}/Src/main.c don't inckluded, this main.c is used only in simulator of touchgfx
  ${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32f4xx_hal_msp.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32f4xx_it.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Src/syscalls.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Src/sysmem.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Src/system_stm32f4xx.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Startup/*.s)

get_property(tmp GLOBAL PROPERTY sources_list)

foreach(source_file IN LISTS CORE_SRCS)
   set(tmp "${tmp}${source_file};")
endforeach()

set_property(GLOBAL PROPERTY sources_list "${tmp}")

get_property(tmp GLOBAL PROPERTY include_paths_list)
set(tmp "${tmp}${CMAKE_CURRENT_SOURCE_DIR}/Inc;")
set_property(GLOBAL PROPERTY include_paths_list "${tmp}")
