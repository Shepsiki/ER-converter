#pragma once

#include <string>

namespace erconv {
    enum class ErrorsType {
        DEFAULT_E,

        INVALID_ENTITY_NAME_E,
        EXISTING_ENTITY_NAME_E,
        NOT_FOUND_ENTITY_NAME_E,

        INVALID_ENTITY_FIELD_NAME_E,
        EXISTING_ENTITY_FIELD_NAME_E,
        NOT_FOUND_ENTITY_FIELD_NAME_E,

        DATA_TYPE_NOT_DECLARED_E,
        COLLISION_WITH_CONSTRAINTS_E,
        REDEFINITION_PRIMARY_KEY_E,
        HAVE_NO_PRIMARY_KEY_E,

        INVALID_RELATIONSHIP_NAME_E,
        EXISTING_RELATIONSHIP_E,
        NOT_FOUND_RELATIONSHIP_E,
    };

    struct TError {
        ErrorsType TypeError;
        std::string msg;

        TError(const ErrorsType & err);

        const std::string & GetMessage() const;
    };
}; // namespace erconv