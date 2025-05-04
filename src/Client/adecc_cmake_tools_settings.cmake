
if(NOT DEFINED PROJECT_NAME OR PROJECT_NAME STREQUAL "")
    message(FATAL_ERROR "Die Variable PROJECT_NAME ist nicht gesetzt! Bitte definieren bevor diese Datei included wird.")
endif()

# define the plattform about system name and make plattform spezific settings
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(PLATFORM "Windows")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${OUTPUT_DIR}")
    add_definitions(-D_WIN32_WINNT=0x0A00) # Windows 10 / Server 2019, for Windows 7 is 0x0601
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PLATFORM "Linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(PLATFORM "macOS")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(PLATFORM "Android")
endif()

macro(clone_repository REPRO_PATH REPRO_NAME REPRO_URL)
   if(DEFINED ${REPRO_PATH})
      if(NOT EXISTS ${${REPRO_PATH}})
         message("Cloning ${REPRO_NAME} repository ...")
         execute_process(
            COMMAND git clone ${REPRO_URL} ${${REPRO_PATH}} 
            RESULT_VARIABLE GIT_CLONE_RESULT 
            )
         if(NOT ${GIT_CLONE_RESULT} EQUAL "0")
            message(FATAL_ERROR "Failed to clone ${REPRO_NAME} repository. Aborting.")
         endif()
      endif()
   else()
      message(WARNING, "${REPRO_PATH} is not defined. Skipping repository cloning.")
   endif()
endmacro()


macro(add_include_directory_if_exists TARGET INCLUDE_PATH) 
   if(DEFINED ${INCLUDE_PATH})
      if(EXISTS ${${INCLUDE_PATH}})
         target_include_directories(${PROJECT_NAME} PUBLIC ${${INCLUDE_PATH}})
      else()
         message(WARNING "Include directory ${${INCLUDE_PATH}} does not exists.")
      endif()
   endif()
endmacro()

# extract the hostname for individual settings
execute_process(COMMAND hostname OUTPUT_VARIABLE HOSTNAME OUTPUT_STRIP_TRAILING_WHITESPACE)
string(STRIP "${HOSTNAME}" TRIMMED_HOSTNAME)
message(STATUS "Hostname: ${HOSTNAME}")
