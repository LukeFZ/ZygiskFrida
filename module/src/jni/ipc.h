//
// Created by lukef on 19.07.2023.
//

#ifndef ZYGISKFRIDA_IPC_H
#define ZYGISKFRIDA_IPC_H

#include <string>

enum IpcCommand : uint8_t {
    ShouldInject = 0x10,
    ShouldInjectResponse
};

bool should_inject(int fd, const std::string& app_name, bool* result);

bool companion_should_inject(int fd);


#endif //ZYGISKFRIDA_IPC_H
