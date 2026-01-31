cmake_minimum_required(VERSION 3.21)

project(NotepadCLI LANGUAGES CXX)

add_executable(NotepadCLI
    src/main.cpp
    src/app.cpp
)

target_include_directories(NotepadCLI PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(NotepadCLI PRIVATE cxx_std_17)
