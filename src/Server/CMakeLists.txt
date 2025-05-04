cmake_minimum_required(VERSION 3.26)
project(ADC_Demo_Server)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(adecc_cmake_tools_settings.cmake)

# define output directory and set it for same points
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/out/${PLATFORM}/${CMAKE_BUILD_TYPE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# Define the list of IDL files
set(IDL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../IDL/Organization.idl" )


include(adecc_tao_settings.cmake)
add_custom_target(generate_idl_files ALL DEPENDS ${CLIENT_GENERATED_SOURCES} ${SERVER_GENERATED_SOURCES})

set(PROJECT_SOURCES Server.cpp 
                    Organization_i.cpp Organization_i.h
                    EmployeeData.h Employee_i.cpp Employee_i.h
						  ${SERVER_GENERATED_SOURCES})

# qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})


message(STATUS ${OUTPUT_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

include_directories(${CMAKE_SOURCE_DIR})

# tao specific
add_dependencies(${PROJECT_NAME} generate_idl_files)
target_link_libraries(${PROJECT_NAME} PRIVATE ${ACE_LIBRARIES} ${TAO_LIBRARIES}) 
target_include_directories(${PROJECT_NAME} PRIVATE ${ACE_INCLUDE_DIR} ${TAO_INCLUDE_DIR})


