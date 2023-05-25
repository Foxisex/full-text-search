set(target_name_test sum_test)

add_executable(${target_name_test})

include(CompileOptions)
set_compile_options(${target_name_test})

target_sources(
  ${target_name_test}
  PRIVATE
    tests/solver.test.cpp
)

target_link_libraries(
  ${target_name_test}
  PRIVATE
    fts
    gtest
)

set(target_name_test parser_test)

add_executable(${target_name_test})

include(CompileOptions)
set_compile_options(${target_name_test})

target_sources(
  ${target_name_test}
  PRIVATE
    tests/parser.test.cpp
)

target_link_libraries(
  ${target_name_test}
  PRIVATE
    fts
    gtest
)

set(target_name_test indexer_test)

add_executable(${target_name_test})

include(CompileOptions)
set_compile_options(${target_name_test})

target_sources(
  ${target_name_test}
  PRIVATE
    tests/indexer.test.cpp
)

target_link_libraries(
  ${target_name_test}
  PRIVATE
    fts
    gtest
)

set(target_name_test searcher_test)

add_executable(${target_name_test})

include(CompileOptions)
set_compile_options(${target_name_test})

target_sources(
  ${target_name_test}
  PRIVATE
    tests/searcher.test.cpp
)

target_link_libraries(
  ${target_name_test}
  PRIVATE
    fts
    gtest
)
