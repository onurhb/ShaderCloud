#-----------------------------------------------------------------------------------
#░░▄░░░▄░▄▄▄▄░░░░░░░░░░░░░░░
#░░█▀▄▀█░█▄▄░░░░░░░░░░░░░░░░
#░░█░░░█░█▄▄▄░░░░░░░░░░░░░░░
#░░░░░░░░░░░░░░░░░░░░░░░░░░░
#░▄▄▄░▄░░░▄░░▄▄▄░▄▄▄▄▄░░▄▄▄░
#█░░░░█░░░█░█░░░░░░█░░░█░░░█
#█░▀█░█░░░█░░▀▀▄░░░█░░░█▀▀▀█
#░▀▀▀░░▀▀▀░░▄▄▄▀░░░▀░░░▀░░░▀
#-------------------------------------------------------------------------------------
project(Project)
cmake_minimum_required(VERSION 3.6)

#-------------------------------------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building with DEBUG mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -w")
else()
    message("Building with RELEASE mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14") # -mwindows
endif()

#Path to dependencies
set(DEPENDENCIES_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies)
#Path to CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)
#Path to GLFW
set(GLFW_LOCATION ${DEPENDENCIES_LOCATION}/glfw)
#Path to RTAUDIO
set(RTAUDIO_LOCATION ${DEPENDENCIES_LOCATION}/rtaudio)
#Path to SNDFILE
set(SNDFILE_LOCATION ${DEPENDENCIES_LOCATION}/sndfile)
#Path to LAME
set(LAME_LOCATION ${DEPENDENCIES_LOCATION}/lame)
#Path to CURL
set(CURL_LOCATION ${DEPENDENCIES_LOCATION}/curl)
#-------------------------------------------------------------------------------------

include_directories(
        ${DEPENDENCIES_LOCATION}/nativefiledialogs
        ${DEPENDENCIES_LOCATION}/glfw/include
        ${DEPENDENCIES_LOCATION}/glad/include
        ${DEPENDENCIES_LOCATION}/kissfft
        ${DEPENDENCIES_LOCATION}/nanovg
        ${DEPENDENCIES_LOCATION}/glm/glm
        ${DEPENDENCIES_LOCATION}/imgui
        ${DEPENDENCIES_LOCATION}/stb
        )

#-------------------------------------------------------------------------------------
set(GLAD_SOURCE_FILES ${DEPENDENCIES_LOCATION}/glad/src/glad.c)
set(NANOVG_SOURCE_FILES ${DEPENDENCIES_LOCATION}/nanovg/nanovg.c)
set(KISSFFT_SOURCE_FILES ${DEPENDENCIES_LOCATION}/kissfft/kiss_fft.c)
set(IMGUI_SOURCE_FILES ${DEPENDENCIES_LOCATION}/imgui/imgui.cpp ${DEPENDENCIES_LOCATION}/imgui/imgui_draw.cpp ${DEPENDENCIES_LOCATION}/imgui/imgui_binding.cpp)
set(NATIVEFILEDIALOGS_SOURCE_FILES ${DEPENDENCIES_LOCATION}/nativefiledialogs/nfd_win.cpp ${DEPENDENCIES_LOCATION}/nativefiledialogs/nfd_common.c)

set(SOURCE_FILES
        Source/main.cpp
        Source/Utils/Math.h
        Source/Algorithm/FFT.h
        Source/Utils/FileDialog.h
        Source/Graphics/Texture.h
        Source/Utils/Thread.h Source/config.h
        Source/Utils/HTTP.cpp Source/Utils/HTTP.h
        Source/Application.cpp Source/Application.h
        Source/Audio/Decoder.cpp Source/Audio/Decoder.h
        Source/Audio/Playback.cpp Source/Audio/Playback.h
        Source/Graphics/Window.cpp Source/Graphics/Window.h
        Source/Graphics/Shader.cpp Source/Graphics/Shader.h
        Source/Audio/AudioFile.cpp Source/Audio/AudioFile.h
        Source/Graphics/UserInterface.cpp Source/Graphics/UserInterface.h
        Source/Graphics/ShaderVisualizer.cpp Source/Graphics/ShaderVisualizer.h
        Source/Utils/NotificationHandler.cpp Source/Utils/NotificationHandler.h)

#-------------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${IMGUI_SOURCE_FILES} ${NANOVG_SOURCE_FILES} ${GLAD_SOURCE_FILES}
               ${KISSFFT_SOURCE_FILES} ${NATIVEFILEDIALOGS_SOURCE_FILES} ${NANOVG_SOURCE_FILES})
#-------------------------------------------------------------------------------------
find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED glfw3)
find_package(RTAUDIO REQUIRED)
find_package(SNDFILE REQUIRED)
find_package(LAME REQUIRED)
find_package(CURL REQUIRED)
#-------------------------------------------------------------------------------------

if (OPENGL_FOUND)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
    message("-- OPENGL WAS FOUND...")
endif ()

if (GLFW_FOUND)
    include_directories(${GLFW_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})
    message("-- GLFW WAS FOUND...")
endif ()

if(RTAUDIO_FOUND)
    include_directories(${RTAUDIO_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${RTAUDIO_LIBRARIES})
    message("-- RTAUDIO WAS FOUND...")
endif()

if(SNDFILE_FOUND)
    include_directories(${SNDFILE_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SNDFILE_LIBRARIES})
    message("-- SNDFILE WAS FOUND...")
endif()

if(LAME_FOUND)
    include_directories(${LAME_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${LAME_LIBRARIES})
    message("-- LAME WAS FOUND...")
endif()

if(CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})
    message("-- CURL WAS FOUND...")
endif()

#-------------------------------------------------------------------------------------

if (WIN32)
    #Copy GLFW dll's to .exe dir
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DEPENDENCIES_LOCATION}/GLFW/lib/glfw3.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying glfw3 .dll files to .exe folder")
    #Copy RTAUDIO dll's to .exe dir
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DEPENDENCIES_LOCATION}/RTAUDIO/lib/librtaudio.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying rtaudio .dll files to .exe folder")
    #Copy SNDFILE dll's to .exe dir
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DEPENDENCIES_LOCATION}/SNDFILE/lib/libsndfile-1.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying sndfile .dll files to .exe folder")
    #Copy LAME dll's to .exe dir
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DEPENDENCIES_LOCATION}/LAME/lib/libmp3lame.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying lame .dll files to .exe folder")
    #Copy CURL dll's to .exe dir
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DEPENDENCIES_LOCATION}/curl/lib/libcurl.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying curl .dll files to .exe folder")
    #Copy CURL dll's to .exe dir
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DEPENDENCIES_LOCATION}/curl/lib/zlib1.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying curl .dll files to .exe folder")
endif ()




