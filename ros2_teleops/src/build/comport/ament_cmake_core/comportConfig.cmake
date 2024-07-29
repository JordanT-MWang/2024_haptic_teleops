# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_comport_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED comport_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(comport_FOUND FALSE)
  elseif(NOT comport_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(comport_FOUND FALSE)
  endif()
  return()
endif()
set(_comport_CONFIG_INCLUDED TRUE)

# output package information
if(NOT comport_FIND_QUIETLY)
  message(STATUS "Found comport: 0.0.0 (${comport_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'comport' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${comport_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(comport_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${comport_DIR}/${_extra}")
endforeach()
