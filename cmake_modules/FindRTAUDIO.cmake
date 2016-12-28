#  RTAUDIO_FOUND
#  RTAUDIO_INCLUDE_DIR
#  RTAUDIO_LIBRARY

find_path(RTAUDIO_INCLUDE_DIR
        NAMES RtAudio.h
        HINTS ${RTAUDIO_LOCATION}/include
        )

find_library(RTAUDIO_LIBRARIES
        NAMES rtaudio librtaudio
        HINTS ${RTAUDIO_LOCATION}/lib
        )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RTAUDIO DEFAULT_MSG RTAUDIO_LIBRARIES RTAUDIO_INCLUDE_DIR)