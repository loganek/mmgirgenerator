add_executable(mmgirgenerator
  main.cc
  defsprinter.cc
  repositoryloader.cc
  datastructures.h)

target_link_libraries(mmgirgenerator ${LIBXMLPP3_LIBRARIES} dl)
