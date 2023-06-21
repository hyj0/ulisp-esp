
set(CMAKE_CXX_FLAGS " -std=c++11 -fpermissive  -fsanitize=address ")
add_executable(ulisp-esp-comments ulisp-esp-comments.cpp)
target_link_libraries(ulisp-esp-comments asan rt)