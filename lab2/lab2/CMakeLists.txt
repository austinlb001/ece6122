
# Add the executable
add_executable(${PROJECT_NAME} code/lab2.cpp code/lib.cpp)

# Link the executable to the libraries in the lib directory
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-graphics sfml-system sfml-window)
