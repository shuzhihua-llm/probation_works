#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "student_service.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using school::Student;
using school::StudentService;
using school::GetStudentRequest;
using school::GetStudentResponse;
using school::GetStudentsRequest;
using school::GetStudentsResponse;

class StudentServiceImpl final : public StudentService::Service {
    Status GetStudent(ServerContext* context, const GetStudentRequest* request,
                      GetStudentResponse* response) override {
        std::cout << "收到请求: 获取学生 ID=" << request->student_id() << std::endl;
        
        Student* student = response->mutable_student();
        student->set_id(request->student_id());
        student->set_name("张三");
        student->set_age(20);
        student->set_email("zhangsan@example.com");
        
        return Status::OK;
    }
    
    Status GetStudents(ServerContext* context, const GetStudentsRequest* request,
                       GetStudentsResponse* response) override {
        std::cout << "收到批量请求: " << request->student_ids_size() << " 个学生" << std::endl;
        
        for (int id : request->student_ids()) {
            Student* student = response->add_students();
            student->set_id(id);
            student->set_name("学生" + std::to_string(id));
            student->set_age(20 + id % 5);
            student->set_email("student" + std::to_string(id) + "@example.com");
        }
        
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    StudentServiceImpl service;
    
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "gRPC 服务已启动，监听地址: " << server_address << std::endl;
    
    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();
    return 0;
}