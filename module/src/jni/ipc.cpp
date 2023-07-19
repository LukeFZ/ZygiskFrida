//
// Created by lukef on 19.07.2023.
//

#include <unistd.h>

#include <fstream>
#include <istream>
#include <fcntl.h>

#include "ipc.h"
#include "log.h"
#include "inject.h"
#include "config.h"

#define SEND_COMMAND(x) \
    command = x; \
    SEND(&command, sizeof(command))

#define SEND(x, y) \
    if (write(fd, (void*)(x), y) != (y)) { \
        return false;                   \
    }

#define READ_COMMAND(x) \
    READ(&command, sizeof(command)) \
    if (command != (x)) { \
        return false;   \
    }

#define READ(x, y) \
    if (read(fd, (void*)(x), y) != (y)) { \
        return false;                   \
    }

bool should_inject(int fd, const std::string& app_name, bool* result) {
    IpcCommand command;

    SEND_COMMAND(IpcCommand::ShouldInject)

    const auto nameLength = app_name.length();
    SEND(&nameLength, sizeof(nameLength))
    SEND(app_name.c_str(), nameLength)

    READ_COMMAND(IpcCommand::ShouldInjectResponse)

    READ(result, sizeof(bool))

    return true;
}

bool companion_should_inject(int fd) {
    size_t app_name_length = 0;
    READ(&app_name_length, sizeof(app_name_length))

    std::vector<char> app_name_buffer(app_name_length, 0);
    READ(&app_name_buffer[0], app_name_length)

    std::string app_name(&app_name_buffer[0], app_name_buffer.size());

    const auto module_path = std::string("/data/local/tmp/") + ModulePackageName;
    bool result = is_app_name_target(module_path, app_name);

    IpcCommand command;

    SEND_COMMAND(IpcCommand::ShouldInjectResponse)
    SEND(&result, sizeof(result))

    return true;
}