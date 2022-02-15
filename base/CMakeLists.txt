set(base_SRCS
  thread_pool/ctrip_thread_pool.cc
  )

add_library(base_lib ${base_SRCS})
target_link_libraries(base_lib pthread)

install(TARGETS base_lib DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/)

if(DATA_STRUCT_BUILD_EXAMPLES)
  add_subdirectory(tests)
endif()