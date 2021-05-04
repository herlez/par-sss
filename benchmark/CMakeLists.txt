add_executable(benchmark benchmark.cpp)
target_include_directories(benchmark PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/benchmark/>
  $<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/benchmark/>
)

target_link_libraries(benchmark tlx malloc_count dl sss_implementations -lpthread)

add_executable(sss_comp sss_comp.cpp)
target_include_directories(sss_comp PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/sss_comp/>
  $<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/sss_comp/>
)

target_link_libraries(sss_comp tlx sss_implementations -lpthread)
