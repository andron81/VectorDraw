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

find_package( Qt6 REQUIRED COMPONENTS Core Gui Widgets )

qt_standard_project_setup()

qt_add_executable( ${CMAKE_PROJECT_NAME}
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
target_link_libraries     ( ${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets )
set_target_properties     ( ${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR} )

qt_generate_deploy_app_script(
	TARGET ${CMAKE_PROJECT_NAME}
	OUTPUT_SCRIPT deploy_script
	NO_UNSUPPORTED_PLATFORM_ERROR )

install( SCRIPT ${deploy_script} )

install( TARGETS ${CMAKE_PROJECT_NAME}
	BUNDLE  DESTINATION .
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} )

set( CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME} )
set( CPACK_PACKAGE_VERSION 0.1 )
set( CPACK_PACKAGE_DIRECTORY ${CMAKE_SOURCE_DIR} )
include( CPack )
