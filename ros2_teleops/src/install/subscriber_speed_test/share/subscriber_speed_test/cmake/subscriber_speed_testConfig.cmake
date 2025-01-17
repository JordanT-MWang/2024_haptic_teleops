# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_subscriber_speed_test_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED subscriber_speed_test_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(subscriber_speed_test_FOUND FALSE)
  elseif(NOT subscriber_speed_test_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(subscriber_speed_test_FOUND FALSE)
  endif()
  return()
endif()
set(_subscriber_speed_test_CONFIG_INCLUDED TRUE)

# output package information
if(NOT subscriber_speed_test_FIND_QUIETLY)
  message(STATUS "Found subscriber_speed_test: 0.0.0 (${subscriber_speed_test_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'subscriber_speed_test' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${subscriber_speed_test_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(subscriber_speed_test_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${subscriber_speed_test_DIR}/${_extra}")
endforeach()
