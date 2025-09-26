
# Add the executable
add_executable(${PROJECT_NAME} lab1.cpp ECE_Buzzy.cpp ECE_Enemy.cpp 
    ECE_LaserBlast.cpp)


# Link the executable to the libraries in the lib directory
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-graphics sfml-system sfml-window)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)




