add_library(Logger Logger.cpp)
target_compile_features(Logger PRIVATE cxx_std_11)
target_link_libraries(Logger PRIVATE Config)
target_include_directories(Logger PUBLIC ${CMAKE_CURRENT_LIST_DIR})
