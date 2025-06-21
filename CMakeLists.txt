cmake_minimum_required(VERSION 3.16)

project(plagiarism-detector VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(Qt6 REQUIRED COMPONENTS Quick Concurrent)

qt_standard_project_setup(REQUIRES 6.8)

# And make sure all headers are listed


# List all source files
set(SOURCE_FILES
    main.cpp
    backend.cpp
    Preprocessor.cpp
    Rabin_karp.cpp
    filereader.cpp
    backend.h
    Preprocessor.h
    Rabin_karp.h
    filereader.h
)

qt_add_executable(plagiarism-detector
    ${SOURCE_FILES}
)

qt_add_qml_module(plagiarism-detector
    URI plagiarism.detector  # Must match the module name in loadFromModule()
    VERSION 1.0
    QML_FILES
        Main.qml
        FileCard.qml
    RESOURCES
        icons/add_file.png
        icons/compare.svg
        icons/history.png
        icons/menu.svg
        icons/settings.svg
        icons/theme.svg
)

target_link_libraries(plagiarism-detector
    PRIVATE
        Qt6::Quick
        Qt6::Concurrent
)

set_target_properties(plagiarism-detector PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
