# set(base_SRCS
#   )

# add_library(netbase ${base_SRCS})
# target_link_libraries(netbase pthread)

# install(TARGETS netbase DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/)

if(DATA_STRUCT_BUILD_EXAMPLES)
  add_subdirectory(tests)
endif()