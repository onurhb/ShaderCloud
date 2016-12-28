find_path(LAME_INCLUDE_DIR
        NAMES
        lame.h
        HINTS
        "${LAME_LOCATION}/include"
        /opt/local/include
        NO_DEFAULT_PATH)

find_library(LAME_LIBRARIES
        NAMES
        mp3lame
        libmp3lame
        HINTS
        "${LAME_LOCATION}/lib"
        /opt/local/lib
        /usr/lib)


if (LAME_INCLUDE_DIR AND LAME_LIBRARIES)
    set(LAME_FOUND TRUE)
endif (LAME_INCLUDE_DIR AND LAME_LIBRARIES)

if (LAME_FOUND)
    if (NOT LAME_FIND_QUIETLY)
    endif (NOT LAME_FIND_QUIETLY)
else (LAME_FOUND)
    if (LAME_FIND_REQUIRED)
        message (FATAL_ERROR "Could not find LAME")
    endif (LAME_FIND_REQUIRED)
endif (LAME_FOUND)