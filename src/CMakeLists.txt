add_executable(mmgirgenerator
  main.cc
  defsprinter.cc
  repositoryloader.cc
  datastructures.h
  dependencyresolver.cc)

target_link_libraries(mmgirgenerator ${LIBXMLPP3_LIBRARIES} dl)
