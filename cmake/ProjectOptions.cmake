include(cmake/Warnings.cmake)

add_library(ProjectOptions INTERFACE)
target_compile_features(ProjectOptions INTERFACE cxx_std_17)
set_project_warnings(ProjectOptions)
