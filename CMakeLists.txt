cmake_minimum_required(VERSION 3.10)  
project(MySTL)  
  
# 设置C++标准    
set(CMAKE_CXX_STANDARD 14)  
  
# 添加项目源文件    
#add_executable(MySTL src/main.cpp) # 假设你的主源文件是src/main.cpp  
#set_target_properties(MySTL PROPERTIES LINKER_LANGUAGE CXX)  
  

# 包含头文件目录  
include_directories(${PROJECT_SOURCE_DIR}/include)  

# 链接库文件目录  
link_directories(
    ${PROJECT_SOURCE_DIR}/lib
    ${PROJECT_SOURCE_DIR}/lib/googletest)  

# 添加测试源文件  
enable_testing()  
add_executable(MySTLTests
    src/test/test_vector.cpp
    src/test/test_main.cpp) # 假设你的测试源文件是test/my_test.cpp  
target_link_libraries(MySTLTests gtest) # 链接MySTL和gtest到测试可执行文件  
add_test(NAME MySTLTests COMMAND MySTLTests) # 运行测试可执行文件进行测试  