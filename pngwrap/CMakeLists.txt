add_library(pngwrap pngwrap.cpp)
target_link_libraries(pngwrap PNG::PNG ZLIB::ZLIB)