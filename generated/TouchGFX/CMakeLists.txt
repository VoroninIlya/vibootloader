file(GLOB CORE_SRCS 
  ${CMAKE_CURRENT_SOURCE_DIR}/App/*.c
  ${CMAKE_CURRENT_SOURCE_DIR}/generated/fonts/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/generated/gui_generated/src/common/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/generated/gui_generated/src/main_screen/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/generated/images/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/generated/texts/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/gui/src/common/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/gui/src/main_screen/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/gui/src/model/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/target/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/target/generated/*.cpp)

get_property(tmp GLOBAL PROPERTY sources_list)

foreach(source_file IN LISTS CORE_SRCS)
   set(tmp "${tmp}${source_file};")
endforeach()

set_property(GLOBAL PROPERTY sources_list "${tmp}")

get_property(tmp GLOBAL PROPERTY include_paths_list)
set(tmp "${tmp}\
${CMAKE_CURRENT_SOURCE_DIR}/App;\
${CMAKE_CURRENT_SOURCE_DIR}/generated/fonts/include;\
${CMAKE_CURRENT_SOURCE_DIR}/generated/gui_generated/include;\
${CMAKE_CURRENT_SOURCE_DIR}/generated/images/include;\
${CMAKE_CURRENT_SOURCE_DIR}/generated/texts/include;\
${CMAKE_CURRENT_SOURCE_DIR}/gui/include;\
${CMAKE_CURRENT_SOURCE_DIR}/target;\
${CMAKE_CURRENT_SOURCE_DIR}/target/generated;")

set_property(GLOBAL PROPERTY include_paths_list "${tmp}")