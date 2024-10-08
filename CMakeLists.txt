cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(print_ip VERSION ${PROJECT_VESRION})

option(WITH_GTEST_TEST "Whether to build Google test" ON)

configure_file(version.h.in ${CMAKE_SOURCE_DIR}/version.h)

add_executable(print_ip print_ip.cpp)
add_library(version_lib lib.cpp)

set_target_properties(print_ip print_ip PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(print_ip
    PRIVATE "${CMAKE_BINARY_DIR}"
)

if(WITH_GTEST_TEST)
    find_package(GTest REQUIRED)
	add_executable(test_version_gtest test_version_gtest.cpp)
    	
	target_include_directories(test_version_gtest
		PRIVATE ${GTEST_INCLUDE_DIRS}        
	)

	target_link_libraries(
		test_version_gtest
		${GTEST_BOTH_LIBRARIES}		
        version_lib
	)
endif()

if (MSVC)
    target_compile_options(print_ip PRIVATE
        /W4
    )    
    if(WITH_GTEST_TEST)
        target_compile_options(test_version_gtest PRIVATE
            /W4
        )
    endif()
else ()    
    target_compile_options(print_ip PRIVATE
        -Wall -Wextra -pedantic -Werror
    )    
    if(WITH_GTEST_TEST)
        target_compile_options(test_version_gtest PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()

install(TARGETS print_ip RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

enable_testing()

if(WITH_GTEST_TEST)    
    add_test(test_version_gtest test_version_gtest)
endif()
