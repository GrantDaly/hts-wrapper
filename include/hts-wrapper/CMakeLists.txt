include(GenerateExportHeader)
generate_export_header(hts-wrapper)

target_sources(hts-wrapper
PUBLIC
FILE_SET HEADERS
FILES
bam-header.h
bam-record.h
cigar.h
faidx-cpp.h
sam-file.h
sam-iterator.h
)
