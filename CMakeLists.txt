cmake_minimum_required (VERSION 3.0)

project (Data-Analysis)
enable_language (CXX)
set (CMAKE_CXX_STANDARD 11)

######################################## macro for Visual Studio  ===========================
macro(configure_link_flags)
  set(MSVC_C_CXX_FLAGS
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_MINSIZEREL
    CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_RELWITHDEBINFO
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_MINSIZEREL
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_CXX_FLAGS_RELWITHDEBINFO
  )
  if(${VCPKG_TARGET_TRIPLET} MATCHES "static")
    message(STATUS
      "MSVC: static link"
    )
    foreach(flag ${MSVC_C_CXX_FLAGS})
      if(${flag} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
      endif()
    endforeach()
  else()
    message(STATUS
      "MSVC: dynamic link"
    )
    foreach(flag ${MSVC_C_CXX_FLAGS})
      if(${flag} MATCHES "/MT")
        string(REGEX REPLACE "/MT" "/MD" ${flag} "${${flag}}")
      endif()
    endforeach()
  endif()
endmacro()


if(MSVC)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_definitions(-DBOOST_NO_RVALUE_REFERENCES)
  add_definitions(-DNOMINMAX)
  configure_link_flags()
endif()

if (APPLE)
	add_definitions(-Wno-deprecated-declarations)
endif()


# make sure that the default is a RELEASE
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RELEASE)
endif()


### Set output directories on a per-configuration base
# Single configuration
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
# Multi configuration
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/bin )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

###########################################################################################

set (HPP_DIR ${CMAKE_SOURCE_DIR}/lib) # directory of hpp
set (TST_DIR ${CMAKE_SOURCE_DIR}/tst) # runtest directory
include_directories(${HPP_DIR})

find_package(OpenMP REQUIRED)
if (OpenMP_CXX_VERSION_MAJOR LESS 4.0)
	message(FATAL_ERROR " Your OpenMP is too old. Required OpenMP 4.0. Please upgrade.")
endif()
if (OpenMP_FOUND)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
endif()
if (CYGWIN)
	set (CMAKE_CXX_FLAGS "-std=gnu++11 ${CMAKE_CXX_FLAGS}")
endif()
if (NOT MSVC)
  set (CMAKE_CXX_FLAGS "-ffast-math ${CMAKE_CXX_FLAGS}") # perform more aggressive floating-point optimizations
  set (CMAKE_C_FLAGS "-ffast-math ${CMAKE_C_FLAGS}")
endif()


# ============ .EXE ===============

add_executable(	runtest_fstat
				${TST_DIR}/runtest_fstat.cpp
				${HPP_DIR}/timing_func.hpp
				${HPP_DIR}/fstat.hpp
			  )
#add_executable( runtest_fbp
#				${TST_DIR}/runtest_bcrfbp.cpp
#				${HPP_DIR}/FocusingBeliefPropagation.hpp
#				${HPP_DIR}/os.hpp
#				${HPP_DIR}/parse_args.hpp
#				${HPP_DIR}/fmath.hpp
#				${HPP_DIR}/patterns.hpp
#				${HPP_DIR}/score_coef.hpp
#			  )
#add_executable(	runtest_rsgd
#				${CMAKE_SOURCE_DIR}/tst/runtest_bcrsgd.cpp
#				${HPP_DIR}/ReplicatedSGD.hpp
#				${HPP_DIR}/os.hpp
#				${HPP_DIR}/parse_args.hpp
#				${HPP_DIR}/fmath.hpp
#				${HPP_DIR}/patterns.hpp
#				${HPP_DIR}/score_coef.hpp
#			  )
add_executable(	runtest_cv
				${CMAKE_SOURCE_DIR}/tst/runtest_cv.cpp
				${HPP_DIR}/cross_validation.hpp
			  )
add_executable(	runtest_nn
				${CMAKE_SOURCE_DIR}/tst/runtest_nn.cpp
				${HPP_DIR}/classifier.hpp
				${HPP_DIR}/neural_network.hpp
				${HPP_DIR}/fmath.hpp
				${HPP_DIR}/patterns.hpp
			  )
add_executable( runtest_pca
				${CMAKE_SOURCE_DIR}/tst/runtest_pca.cpp
				${HPP_DIR}/dimensionality_reduction.hpp
				${HPP_DIR}/fstat.hpp
				${HPP_DIR}/fmath.hpp
        ${HPP_DIR}/patterns.hpp
			  )
add_executable(	runtest_bayes_cls
				${CMAKE_SOURCE_DIR}/tst/runtest_bayes_cls.cpp
				${HPP_DIR}/classifier.hpp
				${HPP_DIR}/diagQDA.hpp
        ${HPP_DIR}/QDA.hpp
			  )
add_executable(	runtest_kabsch
				${CMAKE_SOURCE_DIR}/tst/runtest_kabsch.cpp
				${HPP_DIR}/kabsch.hpp
				${HPP_DIR}/fmath.hpp
				${HPP_DIR}/points.hpp
			  )
