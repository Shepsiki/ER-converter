#pragma once

#include <string>

enum ErrorsType {
    SUCCESS_VALUE_E = 0,

    INVALID_NAME_E,
    EXISTING_NAME_E,

    NOT_FOUND_NAME_E,
};

struct TError {
    ErrorsType TypeError;
    std::string msg;

    TError(ErrorsType err);
};