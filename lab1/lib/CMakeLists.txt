# Set the environment variables for OpenAL
set(OPENAL_LIBRARY openal-soft)
set(OPENAL_INCLUDE_DIR openal-soft/include)

# Add subdirectories for differnt libraries
add_subdirectory(openal-soft)
add_subdirectory(SFML)


