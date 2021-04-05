enable_testing()

function(add_test NAME)
  add_executable(${NAME} ${NAME}.cpp)
  target_link_libraries(${NAME} PRIVATE GTest::gtest_main)
  target_include_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
  target_compile_features(${NAME} PRIVATE cxx_std_17)
  gtest_discover_tests(${NAME})
endfunction()

add_test(ShapesTest)

add_test(SDLBackendTest)
target_link_libraries(SDLBackendTest PRIVATE SDLBackend)

add_test(SurfaceTest)
target_link_libraries(SurfaceTest PRIVATE Rendering)

add_test(TextureAtlasTest)
