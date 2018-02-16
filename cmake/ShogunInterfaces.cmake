MACRO(GENERATE_INTERFACE_TARGET INTERFACE_NAME INTERFACE_DIR INTERFACE_LIBRARIES)

get_target_property(ShogunIncludes shogun::shogun INTERFACE_INCLUDE_DIRECTORIES)
INCLUDE_DIRECTORIES(${ShogunIncludes})

# set compiler SWIG generated cxx compiler flags
SET(CMAKE_CXX_FLAGS ${SWIG_CXX_COMPILER_FLAGS})
# unset any release or distribution flags
# we don't want them when compiling SWIG generated source
SET(CMAKE_CXX_FLAGS_RELEASE "")
SET(CMAKE_CXX_FLAGS_DISTRIBUTION "")
SET(CMAKE_CXX_FLAGS_DEBUG "")

if(${INTERFACE_NAME} STREQUAL "python")
	SET(PREPEND_TARGET "_")
endif()

SET(INTERFACE_FILES)
FILE(GLOB_RECURSE INTERFACE_FILES ${COMMON_INTERFACE_SRC_DIR}/*.i)
FILE(GLOB_RECURSE CUSTOM_INTERFACE_FILES ${INTERFACE_DIR}/*.i)
LIST(APPEND INTERFACE_FILES ${CUSTOM_INTERFACE_FILES})
FOREACH(file ${INTERFACE_FILES})
	get_filename_component(fname ${file} NAME)
	list(APPEND INTERFACE_FILES ${fname})
	ADD_CUSTOM_COMMAND(OUTPUT ${fname}
		DEPENDS ${file}
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different ${file} ${fname}
		COMMENT ""
	)
ENDFOREACH()

SET(INTERFACE_TARGET interface_${INTERFACE_NAME})
SET(INTERFACE_TARGET_SRC ${INTERFACE_TARGET}_src)

ADD_CUSTOM_TARGET(${INTERFACE_TARGET_SRC}
	DEPENDS ${INTERFACE_FILES}
	COMMENT "copying SWIG files")

INCLUDE(${SWIG_USE_FILE})
SET_SOURCE_FILES_PROPERTIES(shogun.i PROPERTIES CPLUSPLUS ON)
IF(DEFINED TARGET_SWIGFLAGS)
	SET_PROPERTY(SOURCE shogun.i PROPERTY SWIG_FLAGS ${TARGET_SWIGFLAGS})
ENDIF()
SET(SWIG_MODULE_${INTERFACE_NAME}_EXTRA_DEPS ${INTERFACE_FILES})
SWIG_ADD_MODULE(${INTERFACE_TARGET} ${INTERFACE_NAME} shogun.i sg_print_functions.cpp)
SET(INTERFACE_REAL_NAME ${SWIG_MODULE_interface_${INTERFACE_NAME}_REAL_NAME})

IF (WIN32)
    SWIG_LINK_LIBRARIES(${INTERFACE_TARGET} shogun::shogun-static)
ELSE ()
    SWIG_LINK_LIBRARIES(${INTERFACE_TARGET} shogun::shogun)
ENDIF ()
IF (NOT "${INTERFACE_LIBRARIES}" STREQUAL "NO")
    SWIG_LINK_LIBRARIES(${INTERFACE_TARGET} ${INTERFACE_LIBRARIES})
ENDIF()

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
	#	    message(STATUS "${_variableName}=${${_variableName}}")
		#	endforeach()

SET_TARGET_PROPERTIES(${INTERFACE_REAL_NAME} PROPERTIES OUTPUT_NAME ${PREPEND_TARGET}shogun)
ADD_DEPENDENCIES(${INTERFACE_REAL_NAME} ${INTERFACE_TARGET_SRC})

#ADD_CUSTOM_COMMAND(TARGETS ${PREPEND_TARGET}interface_${INTERFACE_NAME}
#				   POST_BUILD
#				   COMMAND ${PYTHON_EXECUTABLE}
#				   ARGS ${CMAKE_SOURCE_DIR}/src/.scrub_docstrings.py )

IF(DOXYGEN_FOUND)
	configure_file(${COMMON_INTERFACE_SRC_DIR}/shogun.doxy.in shogun.doxy)

	ADD_CUSTOM_COMMAND(
	OUTPUT    shogun_doxygen
	COMMAND   ${DOXYGEN_EXECUTABLE}
	ARGS	  shogun.doxy
	DEPENDS   shogun::shogun
	COMMENT   "Generating doxygen doc"
	)

	ADD_CUSTOM_COMMAND(
	OUTPUT    shogun_doxygen.i
	COMMAND   ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/src/.doxy2swig.py
	ARGS	  --quiet --no-function-definition shogun_doxygen/xml/index.xml shogun_doxygen.i
	DEPENDS   shogun_doxygen
	)
	ADD_CUSTOM_TARGET(${INTERFACE_NAME}_doxy2swig DEPENDS shogun_doxygen.i)
    ADD_DEPENDENCIES(${INTERFACE_REAL_NAME} ${INTERFACE_NAME}_doxy2swig)
ELSE()
	#TODO add scrubing
ENDIF()

# Make sure all interfaces are build single-threaded to reduce
# excessive memory consumption during build.
IF(SWIG_SINGLE_THREADED)
	FOREACH(SG_INTERFACE_TARGET ${SG_INTERFACE_TARGETS})
		ADD_DEPENDENCIES(${INTERFACE_REAL_NAME}
			${SG_INTERFACE_TARGET})
	ENDFOREACH(SG_INTERFACE_TARGET ${SG_INTERFACE_TARGETS})
	SET(SG_INTERFACE_TARGETS
		"${INTERFACE_REAL_NAME};${SG_INTERFACE_TARGETS}"
		CACHE STRING "List of modular-interfaces beeing build." FORCE)
ENDIF(SWIG_SINGLE_THREADED)

CONFIGURE_FILE(${COMMON_INTERFACE_SRC_DIR}/swig_config.h.in swig_config.h)

UNSET(INTERFACE_TARGET)
UNSET(INTERFACE_TARGET_SRC)
UNSET(INTERFACE_REAL_NAME)
UNSET(INTERFACE_FILES)

ENDMACRO()
