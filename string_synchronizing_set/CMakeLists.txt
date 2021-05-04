add_library(sss_implementations INTERFACE)
target_include_directories(sss_implementations INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/string_synchronizing_set/>
  $<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/string_synchronizing_set/>
)
