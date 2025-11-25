cmake_minimum_required(VERSION 3.23)
project(WinVision LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 프로젝트별 루트 경로를 한 곳에서 관리하면 VS/CI 모두에서 경로 헷갈리지 않음
set(IMAGECORE_SRC_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/ImageCore/src)
set(IMAGECORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ImageCore/include)
set(WINVISION_SRC_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/WinVision/src)
set(WINVISION_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/WinVision/include)

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

add_library(ImageCore STATIC
    ${IMAGECORE_SRC_DIR}/Image.cpp
    ${IMAGECORE_SRC_DIR}/ImageFilter.cpp
    ${IMAGECORE_SRC_DIR}/ImageIO.cpp
    ${IMAGECORE_SRC_DIR}/ImageStats.cpp
)
target_include_directories(ImageCore PUBLIC ${IMAGECORE_INCLUDE_DIR})
target_compile_features(ImageCore PUBLIC cxx_std_17)

add_executable(WinVision
    ${WINVISION_SRC_DIR}/WinVisionApp.cpp
)
target_include_directories(WinVision PRIVATE ${WINVISION_INCLUDE_DIR})
target_link_libraries(WinVision PRIVATE ImageCore)

if (MSVC)
    target_compile_options(ImageCore PRIVATE /W4 /permissive- /WX-)
    target_compile_options(WinVision PRIVATE /W4 /permissive- /WX-)
else()
    target_compile_options(ImageCore PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(WinVision PRIVATE -Wall -Wextra -Wpedantic)
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
        ImageCoreTests/ImageCoreTests.cpp
    )
    target_include_directories(ImageCoreTests PRIVATE ${IMAGECORE_INCLUDE_DIR})
    target_link_libraries(ImageCoreTests
        PRIVATE
            ImageCore
            GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(ImageCoreTests)
endif()

install(TARGETS WinVision RUNTIME DESTINATION bin)