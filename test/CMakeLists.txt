enable_testing()

function(add_test NAME)
  add_executable(${NAME})
  target_link_libraries(${NAME} PRIVATE GTest::gtest_main)
  target_include_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
  target_compile_features(${NAME} PRIVATE cxx_std_17)

  foreach(f ${ARGN})
    target_sources(${NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/${f})
  endforeach()

  gtest_discover_tests(${NAME})
endfunction()


add_test(CommonTest 
  ShapesTest.cpp
)

target_link_libraries(CommonTest PRIVATE Common)
