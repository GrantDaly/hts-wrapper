add_executable(FastaTest fasta-test.cpp)
target_link_libraries(FastaTest hts-wrapper)

configure_file(data/test1.fasta ${CMAKE_CURRENT_BINARY_DIR}/data/test1.fasta COPYONLY)
#configure_file(data/test1.fasta.fai ${CMAKE_CURRENT_BINARY_DIR}/data/test1.fasta.fai COPYONLY)
#configure_file(data/test1.fasta ${CMAKE_CURRENT_BINARY_DIR}/data/test1.fasta COPYONLY)
#add_test(NAME TestInstantiator
#         COMMAND TestInstantiator)
