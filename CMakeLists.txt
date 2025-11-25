cmake_minimum_required(VERSION 3.23)
project(WinVision LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(IMAGECORE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/ImageCore)
set(WINVISION_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/WinVision)
set(IMAGECORE_TESTS_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/ImageCoreTests)

# 빌드 결과물을 한 디렉터리(bin/lib)에 모아두면 GitHub Actions, Inno Setup에서 다루기 쉬움
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    foreach(cfg IN LISTS CMAKE_CONFIGURATION_TYPES)
        string(TOUPPER "${cfg}" cfg_upper)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfg_upper} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${cfg_upper} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${cfg_upper} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
    endforeach()
endif()

option(WINVISION_BUILD_TESTS "Build GoogleTest-based ImageCore tests" ON)

set(IMAGECORE_SOURCES
    ${IMAGECORE_DIR}/Image.cpp
    ${IMAGECORE_DIR}/ImageFilter.cpp
    ${IMAGECORE_DIR}/ImageIO.cpp
    ${IMAGECORE_DIR}/ImageStats.cpp
    ${IMAGECORE_DIR}/pch.cpp
)
if (WIN32)
    list(APPEND IMAGECORE_SOURCES ${IMAGECORE_DIR}/dllmain.cpp)
endif()

add_library(ImageCore STATIC ${IMAGECORE_SOURCES})
target_include_directories(ImageCore PUBLIC ${IMAGECORE_DIR})

add_executable(WinVisionApp
    ${WINVISION_DIR}/WinVisionApp.cpp
    ${WINVISION_DIR}/App.h
)
target_include_directories(WinVisionApp PRIVATE ${WINVISION_DIR} ${IMAGECORE_DIR})
target_link_libraries(WinVisionApp PRIVATE ImageCore)

if (MSVC)
    target_compile_options(ImageCore PRIVATE /W4 /permissive- /WX-)
    target_compile_options(WinVisionApp PRIVATE /W4 /permissive- /WX-)
else()
    target_compile_options(ImageCore PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(WinVisionApp PRIVATE -Wall -Wextra -Wpedantic)
endif()

if (WINVISION_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(ImageCoreTests
        ${IMAGECORE_TESTS_DIR}/ImageCoreTests.cpp
    )
    target_include_directories(ImageCoreTests PRIVATE ${IMAGECORE_DIR})
    target_link_libraries(ImageCoreTests
        PRIVATE
            ImageCore
            GTest::gtest_main
    )

    if (MSVC)
        target_compile_options(ImageCoreTests PRIVATE /W4 /permissive- /WX-)
    else()
        target_compile_options(ImageCoreTests PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    include(GoogleTest)
    gtest_discover_tests(ImageCoreTests)
endif()

install(TARGETS WinVisionApp RUNTIME DESTINATION bin)