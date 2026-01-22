cmake_minimum_required(VERSION 4.0)

project (unamed_language)

set(SOURCES
  main.c
  src/lexer/lexer.c
  src/token_stream/token_stream.c
  src/parser/parser.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
  include
)
