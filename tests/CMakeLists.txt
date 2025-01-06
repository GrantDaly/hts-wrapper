add_executable(FastaTest fasta-test.cpp)
target_link_libraries(FastaTest hts-wrapper)

add_executable(MakeNumt make-numt.cpp)
target_link_libraries(MakeNumt hts-wrapper)

add_executable(PairedTest read-paired-end.cpp)
target_link_libraries(PairedTest hts-wrapper)


add_executable(ReadCompression test-compression-reading.cpp)
target_link_libraries(ReadCompression hts-wrapper)

add_executable(TestAlignment test-alignment.cpp)
target_link_libraries(TestAlignment hts-wrapper)


configure_file(data/test1.fasta ${CMAKE_CURRENT_BINARY_DIR}/data/test1.fasta COPYONLY)
configure_file(data/test.numt.fasta ${CMAKE_CURRENT_BINARY_DIR}/data/test.numt.fasta COPYONLY)

configure_file(data/random.txt ${CMAKE_CURRENT_BINARY_DIR}/data/random.txt COPYONLY)
#configure_file(data/test1.fasta.fai ${CMAKE_CURRENT_BINARY_DIR}/data/test1.fasta.fai COPYONLY)
#configure_file(data/test1.fasta ${CMAKE_CURRENT_BINARY_DIR}/data/test1.fasta COPYONLY)
#add_test(NAME TestInstantiator
#         COMMAND TestInstantiator)
