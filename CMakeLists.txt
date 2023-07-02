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

# If QT variable is set, than CMake searches only in that path.
if( DEFINED QT )
	message( STATUS " * Manually specified QT path: ${QT}" )
	find_package( Qt6 REQUIRED COMPONENTS Core Gui Widgets PrintSupport PATHS ${QT} NO_DEFAULT_PATH )
else()
	find_package( Qt6 REQUIRED COMPONENTS Core Gui Widgets PrintSupport )
endif()

message( STATUS " -------------------------------------------------------------------------------------------" )
message( STATUS "     Compiler: ${CMAKE_CXX_COMPILER_ID}" )
message( STATUS "   Build type: ${CMAKE_BUILD_TYPE}" )
message( STATUS "           Qt: ${Qt6_DIR}" )
message( STATUS " -------------------------------------------------------------------------------------------" )

# This command sets project-wide defaults for a typical Qt application.
qt_standard_project_setup()

# It is a wrapper around the built-in add_executable() command, and provides additional logic to automatically handle things
# like linking of Qt plugins in static Qt builds, platform-specific customization of library names, and so on.
qt_add_executable( ${CMAKE_PROJECT_NAME}
	settings.hpp
	view.hpp
	painter.hpp
	util.hpp
	VectorDraw.cpp
	VectorDraw.hpp
	VectorDraw_pch.hpp )

if( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
 if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	target_link_options( ${CMAKE_PROJECT_NAME} PRIVATE -mconsole )
 else()
	target_link_options( ${CMAKE_PROJECT_NAME} PRIVATE -s )
 endif()
 set_target_properties ( ${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR} )

elseif( CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" )
 if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	target_link_options( ${CMAKE_PROJECT_NAME} PRIVATE /subsystem:console )
 else()
	#target_link_options( ${CMAKE_PROJECT_NAME} PRIVATE /MT )
 endif()

endif()

target_precompile_headers ( ${CMAKE_PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:<${CMAKE_PROJECT_NAME}_pch.hpp$<ANGLE-R>>" )
target_include_directories( ${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR} )
target_link_libraries     ( ${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::PrintSupport )
