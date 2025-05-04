# attention, befor this included IDL_FILES must be set in CmakeList.txt

if(NOT DEFINED PROJECT_NAME OR PROJECT_NAME STREQUAL "")
    message(FATAL_ERROR "Die Variable PROJECT_NAME ist nicht gesetzt! Bitte definieren bevor diese Datei included wird.")
endif()


if(NOT DEFINED IDL_FILES OR IDL_FILES STREQUAL "")
    message(FATAL_ERROR "Die Variable IDL_FILES ist nicht gesetzt! Bitte definieren bevor diese Datei included wird.")
endif()

message(STATUS "processing setting file for TAO")

set (IDL_OUTPUT_DIR ${CMAKE_BINARY_DIR}/out/IDL)

function(process_idl_files)
   foreach(IDL_FILE ${IDL_FILES})
      get_filename_component(IDL_NAME ${IDL_FILE} NAME_WE)
      message(STATUS "IDL:  ${IDL_FILE}")
        add_custom_command(
            OUTPUT ${IDL_OUTPUT_DIR}/${IDL_NAME}C.cpp
                   ${IDL_OUTPUT_DIR}/${IDL_NAME}C.h
                   ${IDL_OUTPUT_DIR}/${IDL_NAME}S.cpp
                   ${IDL_OUTPUT_DIR}/${IDL_NAME}S.h
                   ${IDL_OUTPUT_DIR}/${IDL_NAME}I.cpp
                   ${IDL_OUTPUT_DIR}/${IDL_NAME}I.h
            COMMAND tao_idl -o ${IDL_OUTPUT_DIR} ${IDL_FILE}
            DEPENDS ${IDL_FILE}
        )

      list(APPEND SERVER_GENERATED_SOURCES ${IDL_OUTPUT_DIR}/${IDL_NAME}S.cpp)
      list(APPEND SERVER_GENERATED_HEADERS ${IDL_OUTPUT_DIR}/${IDL_NAME}S.h)
      list(APPEND SERVER_GENERATED_SOURCES ${IDL_OUTPUT_DIR}/${IDL_NAME}C.cpp)
      list(APPEND SERVER_GENERATED_HEADERS ${IDL_OUTPUT_DIR}/${IDL_NAME}C.h)
      list(APPEND CLIENT_GENERATED_SOURCES ${IDL_OUTPUT_DIR}/${IDL_NAME}C.cpp)
      list(APPEND CLIENT_GENERATED_HEADERS ${IDL_OUTPUT_DIR}/${IDL_NAME}C.h)
   endforeach()

   set (SERVER_GENERATED_SOURCES ${SERVER_GENERATED_SOURCES} PARENT_SCOPE)
   set (SERVER_GENERATED_HEADERS ${SERVER_GENERATED_HEADERS} PARENT_SCOPE)
   set (CLIENT_GENERATED_SOURCES ${CLIENT_GENERATED_SOURCES} PARENT_SCOPE)
   set (CLIENT_GENERATED_HEADERS ${CLIENT_GENERATED_HEADERS} PARENT_SCOPE)
endfunction()

process_idl_files()

if(WIN32)
   message(STATUS "Build for Windows")
   set(ACE_INCLUDE_DIR "D:\\Projekte\\Build\\ACE_wrappers")
   set(TAO_INCLUDE_DIR "D:\\Projekte\\Build\\ACE_wrappers\\TAO" "D:\\Projekte\\Build\\ACE_wrappers\\TAO\\orbsvcs")
   set(ACE_LIB_DIR "D:\\Projekte\\Build\\ACE_wrappers\\lib")
   set(TAO_LIB_DIR "D:\\Projekte\\Build\\ACE_wrappers\\lib")
   set(ACE_LIBRARIES ACE)
   set(TAO_LIBRARIES TAO TAO_AnyTypeCode TAO_PortableServer TAO_CosNaming)
elseif(UNIX)
   if("${HOSTNAME}" STREQUAL "Ubuntu1")
      set(ACE_INCLUDE_DIR "/usr/local/include")
      set(TAO_INCLUDE_DIR "/usr/local/include")
   elseif("${HOSTNAME}" STREQUAL "raspberrypi")
      message(STATUS "Build for RaspberryPi (at home)")
      set(ACE_INCLUDE_DIR "/home/vhillmann/Downloads/ACE_Wrappers")
      set(TAO_INCLUDE_DIR "/home/vhillmann/Downloads/ACE_Wrappers/TAO" "/home/vhillmann/Downloads/ACE_Wrappers/TAO/orbsvcs")
   elseif("${HOSTNAME}" STREQUAL "raspberrypi2")
      message(STATUS "Build for RaspberryPi2 (at office)")
      set(ACE_INCLUDE_DIR "/home/vhillmann/build/ACE_Wrappers")
      set(TAO_INCLUDE_DIR "/home/vhillmann/build/ACE_Wrappers/TAO" "/home/vhillmann/build/ACE_Wrappers/TAO/orbsvcs")
   endif()
   set(ACE_LIB_DIR "/usr/local/lib/")
   set(TAO_LIB_DIR "/usr/local/lib/")
   set(ACE_LIBRARIES ACE)
   set(TAO_LIBRARIES TAO TAO_AnyTypeCode TAO_PortableServer TAO_CosNaming)
endif()

add_definitions(-D_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS)
include_directories(${ACE_INCLUDE_DIR} ${TAO_INCLUDE_DIR} ${IDL_OUTPUT_DIR})
link_directories(${ACE_LIB_DIR} ${TAO_LIB_DIR})


# target_link_libraries(${PROJECT_NAME} PRIVATE ${ACE_LIBRARIES} ${TAO_LIBRARIES}) 

# target_include_directories(${PROJECT_NAME} PRIVATE ${ACE_INCLUDE_DIR} ${TAO_INCLUDE_DIR})

