cmake_minimum_required(VERSION 3.16)
project(SimpleBlockchain)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/SimpleBlockchain.cpp
    src/Block.cpp
    src/Hash.cpp
    src/Mine.cpp
    src/Transaction.cpp
    src/TransactionPool.cpp
)

# Create executable
add_executable(simpleblockchain ${SOURCES})

# Link libraries
target_link_libraries(simpleblockchain Threads::Threads OpenSSL::SSL OpenSSL::Crypto)

# Compiler-specific flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(simpleblockchain PRIVATE -Wall -Wextra -O2)
endif()

# Platform-specific settings
if(WIN32)
    target_link_libraries(simpleblockchain ws2_32 wsock32)
endif()

# Install target (optional)
install(TARGETS simpleblockchain
        RUNTIME DESTINATION bin)

# Custom target for clean build
add_custom_target(rebuild
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
    COMMENT "Rebuilding the project"
)