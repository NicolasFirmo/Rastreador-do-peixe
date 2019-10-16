add_library(
    vendor
    include/vendor.h
    lib/vendor.cpp
)

target_include_directories(vendor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})