cmake_minimum_required(VERSION 3.21)
project(main)

set(CMAKE_CXX_STANDARD 17)

set(SRC_DIR ${CMAKE_SOURCE_DIR})

set(LIB_NAME edsdkw)

set(EDSDK_DIR ${SRC_DIR}/edsdk)
set(EDSDK_HEADER_DIR ${EDSDK_DIR}/include)
set(EDSDK_LIB_DIR ${EDSDK_DIR}/lib)

set(SRC_LIST
        edsdk_wrapper.hpp
        edsdk_wrapper.cpp
        logger.hpp
        )

add_library(${LIB_NAME} STATIC ${SRC_LIST})

target_include_directories(${LIB_NAME} PRIVATE ${EDSDK_HEADER_DIR})
target_link_libraries(${LIB_NAME} PUBLIC ${EDSDK_LIB_DIR}/EDSDK.lib)

set(EDSDK_DLL_LIST
        ${EDSDK_LIB_DIR}/EDSDK.dll
        ${EDSDK_LIB_DIR}/EdsImage.dll
        )
add_custom_command(TARGET ${LIB_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${EDSDK_DLL_LIST} ${CMAKE_BINARY_DIR})
