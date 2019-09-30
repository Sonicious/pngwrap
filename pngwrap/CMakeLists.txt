add_library(pngwrap pngwrap.cpp)
set_target_properties(pngwrap PROPERTIES PUBLIC_HEADER "pngwrap.h")
target_link_libraries(pngwrap PNG::PNG ZLIB::ZLIB)