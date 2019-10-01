#### Tests:

add_executable(pngwrapReadWrite pngwrapReadWrite.cpp)
target_link_libraries(pngwrapReadWrite pngwrap)
add_test(pngwrapReadWrite pngwrapReadWrite)