include(FetchContent)

set(UNIT_TESTS "Unit_Tests")

# add source files
set(SOURCES 
    "../src/Matrix.cpp"
    "../src/Renderer.cpp"
    "../src/Texture.cpp"
    "../src/Timer.cpp"
    "../src/Vector2.cpp"
    "../src/Vector3.cpp"
    "../src/Vector4.cpp"
)

# add test source files
set(TESTS
    "Tests.cpp"
)

add_executable(${UNIT_TESTS} ${SOURCES} ${TESTS})

# add gtest
FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)
target_link_libraries(${UNIT_TESTS} PRIVATE gtest gtest_main)

# Simple Directmedia Layer (just in case you write tests that require this)
set(SDL_DIR "${CMAKE_SOURCE_DIR}/project/libs/SDL2-2.30.7")
add_library(SDL STATIC IMPORTED)
set_target_properties(SDL PROPERTIES
    IMPORTED_LOCATION "${SDL_DIR}/lib/x64/SDL2.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL_DIR}/include"
)
target_link_libraries(${UNIT_TESTS} PRIVATE SDL)

file(GLOB_RECURSE DLL_FILES
    "${SDL_DIR}/lib/x64/*.dll"
    "${SDL_DIR}/lib/x64/*.manifest"
)

foreach(DLL ${DLL_FILES})
    add_custom_command(TARGET ${UNIT_TESTS} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${DLL}
        $<TARGET_FILE_DIR:${UNIT_TESTS}>)
endforeach(DLL)

# Simple Directmedia Layer Image (just in case you write tests that require this)
set(SDL_IMAGE_DIR "${CMAKE_SOURCE_DIR}/project/libs/SDL2_image-2.8.2")
add_library(SDL_IMAGE STATIC IMPORTED)
set_target_properties(SDL_IMAGE PROPERTIES
    IMPORTED_LOCATION "${SDL_IMAGE_DIR}/lib/x64/SDL2_image.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL_IMAGE_DIR}/include"
)
target_link_libraries(${UNIT_TESTS} PRIVATE SDL_IMAGE)

file(GLOB_RECURSE DLL_FILES
    "${SDL_IMAGE_DIR}/lib/x64/*.dll"
    "${SDL_IMAGE_DIR}/lib/x64/*.manifest"
)

foreach(DLL ${DLL_FILES})
    add_custom_command(TARGET ${UNIT_TESTS} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${DLL}
        $<TARGET_FILE_DIR:${UNIT_TESTS}>)
endforeach(DLL)


# Visual Leak Detector
if(WIN32 AND CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(ENABLE_VLD=1)
    set(VLD_DIR "${CMAKE_SOURCE_DIR}/project/libs/vld")
    add_library(vld STATIC IMPORTED)
    set_target_properties(vld PROPERTIES
        IMPORTED_LOCATION "${VLD_DIR}/lib/x64/vld.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${VLD_DIR}/include"
    )

    target_link_libraries(${UNIT_TESTS} PRIVATE vld)

    set(DLL_SOURCE_DIR "${VLD_DIR}/lib")

    file(GLOB_RECURSE DLL_FILES
        "${DLL_SOURCE_DIR}/*.dll"
        "${DLL_SOURCE_DIR}/*.manifest"
    )

    foreach(DLL ${DLL_FILES})
        add_custom_command(TARGET ${UNIT_TESTS} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${DLL}
            $<TARGET_FILE_DIR:${UNIT_TESTS}>)
    endforeach(DLL)
endif()



# only needed if header files are not in same directory as source files
# target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# add unit tests for test runner to discover
add_test(NAME ${UNIT_TESTS} COMMAND ${UNIT_TESTS})
