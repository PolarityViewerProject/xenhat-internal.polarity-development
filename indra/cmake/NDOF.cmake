# -*- cmake -*-
include(Prebuilt)

set(NDOF ON CACHE BOOL "Use NDOF space navigator joystick library.")

if (NDOF)
  if (USESYSTEMLIBS)
    set(NDOF_FIND_REQUIRED ON)
    include(FindNDOF)
  else (USESYSTEMLIBS)
    if (WINDOWS OR DARWIN)
      use_prebuilt_binary(libndofdev)
    elseif (LINUX)
      use_prebuilt_binary(libndofdev-opem)
    endif (WINDOWS OR DARWIN)

    if (WINDOWS)
      set(NDOF_LIBRARY libndofdev)
    elseif (DARWIN OR LINUX)
      set(NDOF_LIBRARY ndofdev)
    endif (WINDOWS)

    set(NDOF_INCLUDE_DIR ${ARCH_PREBUILT_DIRS}/include/ndofdev)
    set(NDOF_FOUND 1)
  endif (USESYSTEMLIBS)
endif (NDOF)

if (NDOF_FOUND)
  add_definitions(-DLIB_NDOF=1)
  include_directories(${NDOF_INCLUDE_DIR})
else (NDOF_FOUND)
  message(STATUS "Building without N-DoF joystick support")
  set(NDOF_INCLUDE_DIR "")
  set(NDOF_LIBRARY "")
endif (NDOF_FOUND)

