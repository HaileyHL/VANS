cmake_minimum_required(VERSION 3.5)
project(vans VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

# Core library target
add_library(vans_core
               src/general/controller.h
               src/general/component.h
               src/general/utils.h
               src/general/tick.h
               src/general/config.h
               src/general/request_queue.h
               src/general/buffer.h
               src/general/rmw.cpp
               src/general/rmw.h
               src/general/static_memory.h
               src/general/imc.cpp
               src/general/imc.h
               src/general/ait.cpp
               src/general/ait.h
               src/general/mapping.h
               src/general/dram.h
               src/general/ddr4.cpp
               src/general/ddr4.h
               src/general/dram_memory.h
               src/general/nv_media.h
               src/general/factory.h
               src/general/rmc.h
               src/general/trace.cpp
               src/general/trace.h
               src/general/nvram_system.h
               src/general/ddr4_system.h
               src/general/factory.cpp
               src/general/common.h
               src/cxl/cxl_vans_interfaces.h
               src/cxl/CxlCommandProcessor.cpp
               src/cxl/CxlCommandProcessor.h
               src/cxl/mocks/MockCxlMemory.h
               src/cxl/mocks/MockCxlProtocolHandler.h
               src/cxl/CxlTypes.h 
               src/cxl/mocks/MockCxlDeviceInterface.h 
               src/cxl/mocks/MockCxlHostInterface.h 
               src/cxl/mocks/MockCxlDeviceInterface.cpp
               src/cxl/mocks/MockCxlHostInterface.cpp
               src/cxl/CxlProtocolHandlerImpl.h
               src/cxl/CxlDeviceInterfaceImpl.h
               src/cxl/CxlHostInterfaceImpl.h
               )

target_include_directories(vans_core
                           PUBLIC
                           src/general
                           src/cxl
                           PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}/src
                           )

target_compile_options(vans_core PRIVATE -Wno-subobject-linkage)

add_executable(vans src/vans.cpp)
target_link_libraries(vans PRIVATE vans_core)

include(CTest)
enable_testing()

add_subdirectory(tests/cxl)

add_test(
        NAME TestPrecision
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND python
        tests/precision/precision_test.py
        tests/precision
)
set_tests_properties(TestPrecision PROPERTIES TIMEOUT 3600)
