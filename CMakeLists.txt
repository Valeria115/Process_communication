# cmake_minimum_required(VERSION 3.10)
# project(oop_dz1)

# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized")

# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   GIT_REPOSITORY https://github.com/google/googletest.git
#   GIT_TAG v1.15.0
#   TLS_VERIFY false
# )
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

# add_library(${CMAKE_PROJECT_NAME}_lib src/scary_numbers.cpp)
# add_executable(${CMAKE_PROJECT_NAME}_exe main.cpp)

# target_link_libraries(${CMAKE_PROJECT_NAME}_exe PRIVATE ${CMAKE_PROJECT_NAME}_lib)


# # Добавление тестов
# enable_testing()

# add_executable(tests test/tests01.cpp)
# target_link_libraries(tests ${CMAKE_PROJECT_NAME}_lib gtest_main)

# # Добавление тестов в тестовый набор
# add_test(NAME MyProjectTests COMMAND tests)

cmake_minimum_required(VERSION 3.10)

# Название проекта
project(ProcessPipes C)

# Включаем строгие предупреждения (чтобы код был чище)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")

# Добавляем исполняемые файлы
add_executable(parent parent.c)
add_executable(child child.c)

# Указываем, что после сборки parent и child должны лежать рядом
set_target_properties(parent PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_target_properties(child PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
