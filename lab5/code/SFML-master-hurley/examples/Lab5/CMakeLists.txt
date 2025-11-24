add_executable(server server.cpp)
target_link_libraries(server sfml-network)
add_executable(client client.cpp)
target_link_libraries(client sfml-network)