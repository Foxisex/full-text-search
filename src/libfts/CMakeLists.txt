set(target_name fts)

add_library(${target_name} STATIC
	libfts/solver.cpp
	libfts/parser.cpp
	libfts/indexer.cpp
	libfts/searcher.cpp
	libfts/options.cpp)
	
include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/external)


target_link_libraries(
	${target_name}
	PRIVATE
	m
	csv2
)
