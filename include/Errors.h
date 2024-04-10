#pragma once

#include <string>

namespace erconv {
    enum class ErrorsType {
        DEFAULT_E,

        INVALID_NAME_E,
        EXISTING_NAME_E,
        NOT_FOUND_NAME_E,

        DATA_TYPE_NOT_DECLARED_E,
        COLLISION_WITH_CONSTRAINTS_E,
        REDEFINITION_PRIMARY_KEY_E,
    };

    struct TError {
        ErrorsType TypeError;
        std::string msg;

        TError(const ErrorsType & err);

        const std::string & GetMessage() const;
    };
}; // namespace erconv