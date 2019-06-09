#include <iostream>

#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>

#include "proto/emulator.grpc.pb.h"

int main(int argc, char **argv)
{
    std::cout << "Hello World" << std::endl;

    grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());

    return 0;
}
