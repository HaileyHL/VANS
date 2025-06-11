message(STATUS "Adding test: ${TEST_NAME}")
file(GLOB CXL_TEST_SOURCES *.cpp)

foreach(TEST_SRC ${CXL_TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SRC})
    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
    target_link_libraries(${TEST_NAME} PRIVATE vans_core) 
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()