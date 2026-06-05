#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <grpcpp/grpcpp.h>
#include "student_service.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using school::Student;
using school::StudentService;
using school::GetStudentRequest;
using school::GetStudentResponse;
using school::GetStudentsRequest;
using school::GetStudentsResponse;

class StudentClient {
public:
    StudentClient(std::shared_ptr<Channel> channel)
        : stub_(StudentService::NewStub(channel)) {}
    
    void GetStudent(int id) {
        GetStudentRequest request;
        request.set_student_id(id);
        
        GetStudentResponse response;
        ClientContext context;
        
        Status status = stub_->GetStudent(&context, request, &response);
        
        if (status.ok()) {
            const Student& student = response.student();
            std::cout << "获取学生成功:" << std::endl;
            std::cout << "  ID: " << student.id() << std::endl;
            std::cout << "  姓名: " << student.name() << std::endl;
            std::cout << "  年龄: " << student.age() << std::endl;
            std::cout << "  邮箱: " << student.email() << std::endl;
        } else {
            std::cout << "RPC 失败: " << status.error_code() << ": " << status.error_message() << std::endl;
        }
    }
    
    void GetStudents(const std::vector<int>& ids) {
        GetStudentsRequest request;
        for (int id : ids) {
            request.add_student_ids(id);
        }
        
        GetStudentsResponse response;
        ClientContext context;
        
        Status status = stub_->GetStudents(&context, request, &response);
        
        if (status.ok()) {
            std::cout << "批量获取学生成功，共 " << response.students_size() << " 人:" << std::endl;
            for (const auto& student : response.students()) {
                std::cout << "  [" << student.id() << "] " << student.name() 
                          << ", 年龄: " << student.age() 
                          << ", 邮箱: " << student.email() << std::endl;
            }
        } else {
            std::cout << "RPC 失败: " << status.error_code() << ": " << status.error_message() << std::endl;
        }
    }

private:
    std::unique_ptr<StudentService::Stub> stub_;
};

int main(int argc, char** argv) {
    StudentClient client(grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials()));
    
    std::cout << "=== 测试单个获取 ===" << std::endl;
    client.GetStudent(1001);
    
    std::cout << "\n=== 测试批量获取 ===" << std::endl;
    client.GetStudents({1001, 1002, 1003});
    
    return 0;
}