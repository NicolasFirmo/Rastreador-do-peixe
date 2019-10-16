add_library(
    fishtracker
    include/Botao.h
    include/GUI.h
    include/nicfunc.h
    include/Slider.h
    include/Switch.h
    lib/Botao.cpp
    lib/GUI.cpp
    lib/nicfunc.cpp
    lib/Slider.cpp
    lib/Switch.cpp
)

target_link_libraries ( fishtracker PUBLIC ${OpenCV_LIBS} )
target_link_libraries (fishtracker PRIVATE vendor)

target_include_directories(fishtracker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})