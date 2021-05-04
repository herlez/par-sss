enable_testing()

macro(ae_tiir_build_test TESTNAME)
  add_executable(${TESTNAME}
    ${TESTNAME}.cpp)
  target_include_directories(${TESTNAME} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/exercise/>
    $<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}>/exercise/
    )
  target_link_libraries(${TESTNAME}
    tlx sss_implementations -lpthread)
  add_test(
    NAME ${TESTNAME}
    COMMAND ${TESTNAME} ${ARGN})
endmacro(ae_tiir_build_test)

ae_tiir_build_test(sss_test)
