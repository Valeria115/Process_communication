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


# strace -f -o output.txt ./parent