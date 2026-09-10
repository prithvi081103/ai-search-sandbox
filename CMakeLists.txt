cmake_minimum_required(VERSION 3.14)
project(Graph_Project)

# Enforce C++17 Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch and integrate Raylib automatically
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
)
FetchContent_MakeAvailable(raylib)

# Define the executable and its source files
# (Mapped to the src/ directory where we built them)
add_executable(SearchLab 
    main.cpp
    src/GridProblem.cpp
    src/BFS.cpp
    src/DFS.cpp
    src/IDDFS.cpp
    src/MDFBB.cpp
    src/AStar.cpp
    src/EightPuzzleProblem.cpp
    src/Visualizer.cpp
)

# Link Raylib to the executable
target_link_libraries(SearchLab PRIVATE raylib)
