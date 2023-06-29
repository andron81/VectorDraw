cmake_minimum_required( VERSION 3.16 )

find_program( CCACHE_PROGRAM ccache )
if( CCACHE_PROGRAM )
	message( STATUS " * Using ccache." )
	set( CMAKE_C_COMPILER_LAUNCHER ${CCACHE} )
	set( CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE} )
endif()

find_program( LLD_PROGRAM lld )
if( LLD_PROGRAM )
	message( STATUS " * Using lld." )
	add_link_options( -fuse-ld=lld )
endif()

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

project( VectorDraw CXX )

message( STATUS " -------------------------------------------------------------------------------------------" )
message( STATUS "     Compiler: ${CMAKE_CXX_COMPILER_ID}" )
message( STATUS "   Build type: ${CMAKE_BUILD_TYPE}" )
message( STATUS " Target arch.: ${CMAKE_SYSTEM_PROCESSOR}" )
message( STATUS " -------------------------------------------------------------------------------------------" )

set( QT_PKG Qt5 )

#set( CMAKE_PREFIX_PATH "F:/Qt-static/lib/cmake" )

if( CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64" )
 find_package( ${QT_PKG} REQUIRED COMPONENTS Core Gui Widgets HINTS "C:/qt/5.12.2/mingw73_64" "F:/Qt-static" )
else()
 find_package( ${QT_PKG} REQUIRED COMPONENTS Core Gui Widgets HINTS "C:/qt/5.12.2/mingw73_32" )
endif()

message( STATUS "QT: ${${QT_PKG}_DIR}" )

add_executable( ${CMAKE_PROJECT_NAME}
	settings.hpp
	VectorDraw.cpp
	VectorDraw.hpp
	VectorDraw_pch.hpp )

if( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
 if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	target_link_options( ${CMAKE_PROJECT_NAME} PRIVATE -static -mconsole )
 else()
	target_link_options( ${CMAKE_PROJECT_NAME} PRIVATE -static -s )
 endif()
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" )
 if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	target_link_options( ${CMAKE_PROJECT_NAME} PRIVATE /subsystem:console )
 endif()
endif()

target_precompile_headers ( ${CMAKE_PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:<${CMAKE_PROJECT_NAME}_pch.hpp$<ANGLE-R>>" )
target_include_directories( ${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR} )
target_link_libraries     ( ${CMAKE_PROJECT_NAME} PRIVATE ${QT_PKG}::Core ${QT_PKG}::Gui ${QT_PKG}::Widgets )
set_target_properties     ( ${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR} )
