#include <Poco/MD5Engine.h>
#include <Poco/DigestStream.h>

#include <iostream>

#include <sys/wait.h>
#include <unistd.h>

#include <grpcpp/grpcpp.h>
#include "hello.grpc.pb.h"

void server();
void client();

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

// Logic and data behind the server's behavior.
class GreeterServiceImpl final : public Greeter::Service
{
    Status SayHello(ServerContext *context, const HelloRequest *request,
                    HelloReply *reply) override
    {
        std::string prefix("Hello ");
        reply->set_message(prefix + request->name());
        return Status::OK;
    }
};

int main(int argc, char **argv)
{

    Poco::MD5Engine md5;
    Poco::DigestOutputStream ds(md5);
    ds << "abcdefghijklmnopqrstuvwxyz";
    ds.close();

    std::cout << Poco::DigestEngine::digestToHex(md5.digest()) << std::endl;

    pid_t c_pid = fork();
    if (c_pid == -1)
    {
        return -1;
    }
    else if (c_pid > 0)
    {
        // parent
        server();
    }
    else
    {
        // child process
        client();
    }

    return 0;
}

void server()
{
    std::cout << "server " << getpid() << std::endl;
    const std::string server_address("0.0.0.0:50051");

    grpc::EnableDefaultHealthCheckService(true);

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    GreeterServiceImpl service;
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait(); // how to make service shutdown ?
}

void client()
{
    std::cout << "client " << getpid() << std::endl;
    sleep(5);

    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    const auto stub = helloworld::Greeter::NewStub(channel);
    grpc::ClientContext context;

    helloworld::HelloRequest request;
    request.set_name("Konrad client");

    helloworld::HelloReply reply;

    grpc::Status status = stub->SayHello(&context, request, &reply);

    if (status.ok())
    {
        std::cout << "Status was ok: " << reply.message() << '\n';
    }
    else
    {
        std::cout << "Status was not ok: " << status.error_message() << '\n';
    }
}
