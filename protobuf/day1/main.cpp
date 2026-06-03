#include <iostream>
#include <fstream>
#include <string>
#include "person.pb.h"

void serializeExample() {
    tutorial::Person person;
    person.set_name("张三");
    person.set_id(1001);
    person.set_email("zhangsan@example.com");

    tutorial::Person::PhoneNumber* phone1 = person.add_phones();
    phone1->set_number("138-1234-5678");
    phone1->set_type(tutorial::Person::MOBILE);

    tutorial::Person::PhoneNumber* phone2 = person.add_phones();
    phone2->set_number("010-8765-4321");
    phone2->set_type(tutorial::Person::WORK);

    std::string serialized;
    if (!person.SerializeToString(&serialized)) {
        std::cerr << "序列化失败!" << std::endl;
        return;
    }

    std::cout << "=== 序列化结果 ===" << std::endl;
    std::cout << "原始数据大小: " << serialized.size() << " 字节" << std::endl;
    std::cout << "Person 信息:" << std::endl;
    std::cout << "  姓名: " << person.name() << std::endl;
    std::cout << "  ID: " << person.id() << std::endl;
    std::cout << "  邮箱: " << person.email() << std::endl;
    for (int i = 0; i < person.phones_size(); ++i) {
        const auto& phone = person.phones(i);
        std::cout << "  电话" << i+1 << ": " << phone.number() 
                  << " (类型: " << phone.type() << ")" << std::endl;
    }

    {
        std::ofstream outfile("person.bin", std::ios::binary);
        outfile.write(serialized.data(), serialized.size());
        outfile.close();
        std::cout << "\n已保存到 person.bin 文件" << std::endl;
    }
}

void deserializeExample() {
    std::ifstream infile("person.bin", std::ios::binary);
    std::string serialized((std::istreambuf_iterator<char>(infile)),
                           std::istreambuf_iterator<char>());
    infile.close();

    tutorial::Person person;
    if (!person.ParseFromString(serialized)) {
        std::cerr << "反序列化失败!" << std::endl;
        return;
    }

    std::cout << "\n=== 反序列化结果 ===" << std::endl;
    std::cout << "从 person.bin 读取数据:" << std::endl;
    std::cout << "  姓名: " << person.name() << std::endl;
    std::cout << "  ID: " << person.id() << std::endl;
    std::cout << "  邮箱: " << person.email() << std::endl;
    for (int i = 0; i < person.phones_size(); ++i) {
        const auto& phone = person.phones(i);
        std::string type_str;
        switch (phone.type()) {
            case tutorial::Person::MOBILE: type_str = "手机"; break;
            case tutorial::Person::HOME: type_str = "家庭"; break;
            case tutorial::Person::WORK: type_str = "工作"; break;
            default: type_str = "未知"; break;
        }
        std::cout << "  电话" << i+1 << ": " << phone.number() 
                  << " (类型: " << type_str << ")" << std::endl;
    }
}

void addressBookExample() {
    tutorial::AddressBook address_book;

    tutorial::Person* person1 = address_book.add_people();
    person1->set_name("李四");
    person1->set_id(1002);
    person1->set_email("lisi@example.com");

    tutorial::Person* person2 = address_book.add_people();
    person2->set_name("王五");
    person2->set_id(1003);
    person2->set_email("wangwu@example.com");

    std::string serialized;
    address_book.SerializeToString(&serialized);

    std::cout << "\n=== 通讯录示例 ===" << std::endl;
    std::cout << "通讯录包含 " << address_book.people_size() << " 人" << std::endl;
    std::cout << "序列化后大小: " << serialized.size() << " 字节" << std::endl;

    tutorial::AddressBook decoded_book;
    decoded_book.ParseFromString(serialized);

    std::cout << "反序列化后:" << std::endl;
    for (int i = 0; i < decoded_book.people_size(); ++i) {
        const auto& p = decoded_book.people(i);
        std::cout << "  [" << i+1 << "] " << p.name() 
                  << " (ID: " << p.id() << ", Email: " << p.email() << ")" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    std::cout << "╔════════════════════════════════════════════╗" << std::endl;
    std::cout << "║   Protobuf Day1 - 序列化/反序列化示例      ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════╝" << std::endl;

    serializeExample();
    deserializeExample();
    addressBookExample();

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
