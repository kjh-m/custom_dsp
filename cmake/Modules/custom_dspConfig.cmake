INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_CUSTOM_DSP custom_dsp)

FIND_PATH(
    CUSTOM_DSP_INCLUDE_DIRS
    NAMES custom_dsp/api.h
    HINTS $ENV{CUSTOM_DSP_DIR}/include
        ${PC_CUSTOM_DSP_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    CUSTOM_DSP_LIBRARIES
    NAMES gnuradio-custom_dsp
    HINTS $ENV{CUSTOM_DSP_DIR}/lib
        ${PC_CUSTOM_DSP_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/custom_dspTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CUSTOM_DSP DEFAULT_MSG CUSTOM_DSP_LIBRARIES CUSTOM_DSP_INCLUDE_DIRS)
MARK_AS_ADVANCED(CUSTOM_DSP_LIBRARIES CUSTOM_DSP_INCLUDE_DIRS)
