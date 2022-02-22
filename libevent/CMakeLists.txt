set(base_SRCS
  a.cc
  )

add_library(lib_event ${base_SRCS})
target_link_libraries(lib_event pthread event)

install(TARGETS lib_event DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/)

if(DATA_STRUCT_BUILD_EXAMPLES)
  add_subdirectory(tests)
endif()