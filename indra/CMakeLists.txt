# -*- cmake -*-

# cmake_minimum_required should appear before any
# other commands to guarantee full compatibility
# with the version specified
  cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(ROOT_PROJECT_NAME "Polarity" CACHE STRING
    "The root project/makefile/solution name. Defaults to Polarity.")
project(${ROOT_PROJECT_NAME})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(Variables)
include(BuildVersion)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Build type.  One of: Debug Release RelWithDebInfo" FORCE)
endif (NOT CMAKE_BUILD_TYPE)

add_subdirectory(cmake)

add_subdirectory(${LIBS_OPEN_PREFIX}llaudio)
add_subdirectory(${LIBS_OPEN_PREFIX}llappearance)
add_subdirectory(${LIBS_OPEN_PREFIX}llcharacter)
add_subdirectory(${LIBS_OPEN_PREFIX}llcommon)
add_subdirectory(${LIBS_OPEN_PREFIX}llcorehttp)
add_subdirectory(${LIBS_OPEN_PREFIX}llimage)
add_subdirectory(${LIBS_OPEN_PREFIX}llkdu)
add_subdirectory(${LIBS_OPEN_PREFIX}llimagej2coj)
add_subdirectory(${LIBS_OPEN_PREFIX}llinventory)
add_subdirectory(${LIBS_OPEN_PREFIX}llmath)
add_subdirectory(${LIBS_OPEN_PREFIX}llmessage)
add_subdirectory(${LIBS_OPEN_PREFIX}llprimitive)
add_subdirectory(${LIBS_OPEN_PREFIX}llrender)
add_subdirectory(${LIBS_OPEN_PREFIX}llvfs)
add_subdirectory(${LIBS_OPEN_PREFIX}llwindow)
add_subdirectory(${LIBS_OPEN_PREFIX}llxml)

if (WINDOWS AND EXISTS ${LIBS_CLOSED_DIR}copy_win_scripts)
  add_subdirectory(${LIBS_CLOSED_PREFIX}copy_win_scripts)
endif (WINDOWS AND EXISTS ${LIBS_CLOSED_DIR}copy_win_scripts)

add_custom_target(viewer)
add_subdirectory(${LIBS_OPEN_PREFIX}llcrashlogger)
add_subdirectory(${LIBS_OPEN_PREFIX}llplugin)
add_subdirectory(${LIBS_OPEN_PREFIX}llui)
add_subdirectory(${LIBS_OPEN_PREFIX}viewer_components)

# Legacy C++ tests. Build and run if LL_TESTS is true.
if (LL_TESTS)
  add_subdirectory(${VIEWER_PREFIX}test)
endif (LL_TESTS)

# if (ENABLE_MEDIA_PLUGINS)
# viewer media plugins
add_subdirectory(${LIBS_OPEN_PREFIX}media_plugins)
# endif (ENABLE_MEDIA_PLUGINS)

if (LINUX)
  add_subdirectory(${VIEWER_PREFIX}linux_crash_logger)
  if (INSTALL_PROPRIETARY)
      include(LLAppearanceUtility)
      add_subdirectory(${LLAPPEARANCEUTILITY_SRC_DIR} ${LLAPPEARANCEUTILITY_BIN_DIR})
  endif (INSTALL_PROPRIETARY)
  add_dependencies(viewer linux-crash-logger-strip-target linux-updater)
elseif (DARWIN)
  add_subdirectory(${VIEWER_PREFIX}mac_crash_logger)
  add_dependencies(viewer mac-crash-logger)
elseif (WINDOWS)
  add_subdirectory(${VIEWER_PREFIX}win_crash_logger)
  # cmake EXISTS requires an absolute path, see indra/cmake/Variables.cmake
  if (EXISTS ${VIEWER_DIR}win_setup)
    add_subdirectory(${VIEWER_DIR}win_setup)
  endif (EXISTS ${VIEWER_DIR}win_setup)
  # add_dependencies(viewer windows-setup windows-crash-logger)
  add_dependencies(viewer windows-crash-logger)
elseif (SOLARIS)
  add_subdirectory(solaris_crash_logger)
  add_dependencies(viewer solaris-crash-logger)
endif (LINUX)

add_subdirectory(${VIEWER_PREFIX}newview)
add_dependencies(viewer polarity-bin)

add_subdirectory(${VIEWER_PREFIX}doxygen EXCLUDE_FROM_ALL)

if (WINDOWS)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT polarity-bin)
endif (WINDOWS)

if (LL_TESTS)
  # Define after the custom targets are created so
  # individual apps can add themselves as dependencies
  add_subdirectory(${INTEGRATION_TESTS_PREFIX}integration_tests)
endif (LL_TESTS)

