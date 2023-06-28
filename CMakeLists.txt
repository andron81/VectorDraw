cmake_minimum_required( VERSION 3.16 )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

project( VectorDraw CXX )

find_package( Qt6 REQUIRED COMPONENTS Core Gui Widgets )

add_executable( ${CMAKE_PROJECT_NAME}
	VectorDraw.cpp
	VectorDraw.hpp
	VectorDraw_pch.hpp )

target_precompile_headers ( ${CMAKE_PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:<${CMAKE_PROJECT_NAME}_pch.hpp$<ANGLE-R>>" )
target_include_directories( ${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR} )
target_link_options       ( ${CMAKE_PROJECT_NAME} PRIVATE -mconsole )
target_link_libraries     ( ${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets )
set_target_properties     ( ${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR} )
