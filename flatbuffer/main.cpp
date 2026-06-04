#include <iostream>
#include <fstream>
#include <vector>
#include <flatbuffers/flatbuffers.h>
#include "student_generated.h"

using namespace school;

int main() {
    flatbuffers::FlatBufferBuilder builder(1024);
    
    // 1. 创建嵌套的 Address 对象
    auto city = builder.CreateString("北京");
    auto street = builder.CreateString("朝阳区");
    auto address = CreateAddress(builder, city, street);
    
    // 2. 创建 Vector（课程列表）
    std::vector<flatbuffers::Offset<flatbuffers::String>> course_offsets;
    course_offsets.push_back(builder.CreateString("数学"));
    course_offsets.push_back(builder.CreateString("英语"));
    course_offsets.push_back(builder.CreateString("计算机"));
    auto courses = builder.CreateVector(course_offsets);
    
    // 3. 创建 Student 对象
    auto name = builder.CreateString("张三");
    auto student = CreateStudent(builder, 1001, name, 20, address, courses);
    
    builder.Finish(student);
    
    // 4. 保存到文件
    uint8_t* buf = builder.GetBufferPointer();
    size_t size = builder.GetSize();
    
    std::cout << "序列化成功！大小: " << size << " 字节" << std::endl;
    
    std::ofstream outfile("student.bin", std::ios::binary);
    outfile.write(reinterpret_cast<const char*>(buf), size);
    outfile.close();
    
    // 5. 读取并解析
    std::ifstream infile("student.bin", std::ios::binary);
    std::vector<uint8_t> loaded_data((std::istreambuf_iterator<char>(infile)),
                                     std::istreambuf_iterator<char>());
    infile.close();
    
    const Student* loaded_student = GetStudent(loaded_data.data());
    
    // 6. 访问字段（零拷贝！）
    std::cout << "\n学生信息:" << std::endl;
    std::cout << "ID: " << loaded_student->id() << std::endl;
    std::cout << "姓名: " << loaded_student->name()->c_str() << std::endl;
    std::cout << "年龄: " << loaded_student->age() << std::endl;
    
    // 访问嵌套 Table
    const Address* addr = loaded_student->address();
    std::cout << "地址: " << addr->city()->c_str() << " " << addr->street()->c_str() << std::endl;
    
    // 访问 Vector
    const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>* course_list = loaded_student->courses();
    std::cout << "课程 (" << course_list->size() << "门):" << std::endl;
    for (size_t i = 0; i < course_list->size(); i++) {
        std::cout << "  - " << course_list->Get(i)->c_str() << std::endl;
    }
    
    return 0;
}
