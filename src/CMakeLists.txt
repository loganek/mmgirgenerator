add_executable(mmgirgenerator
  application.cc
  main.cc
  defsprinter.cc
  repositoryloader.cc
  datastructures.cc
  dependencyresolver.cc
  mainoptiongroup.cc
  namespacecollection.cc)

target_link_libraries(mmgirgenerator ${LIBXMLPP3_LIBRARIES} dl)
