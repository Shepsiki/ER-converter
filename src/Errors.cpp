#include "Errors.h"

erconv::TError::TError(const ErrorsType & err) 
{
    switch (err) {
    case ErrorsType::INVALID_NAME_E:
        TypeError = ErrorsType::INVALID_NAME_E;
        msg = "ERROR: The field name has invalid characters\n";
        break;
    case ErrorsType::EXISTING_NAME_E:
        TypeError = ErrorsType::EXISTING_NAME_E;
        msg = "ERROR: This name is already used in the fields of this entity\n";
        break;
    case ErrorsType::NOT_FOUND_NAME_E:
        TypeError = ErrorsType::NOT_FOUND_NAME_E;
        msg = "ERROR: There is no field with this name\n";
        break;
    case ErrorsType::DATA_TYPE_NOT_DECLARED_E:
        TypeError = ErrorsType::DATA_TYPE_NOT_DECLARED_E;
        msg = "ERROR: Trying to create an entity field without a data type\n";
        break;
    case ErrorsType::COLLISION_WITH_CONSTRAINTS_E:
        TypeError = ErrorsType::COLLISION_WITH_CONSTRAINTS_E;
        msg = "ERROR: Trying to use constraints with disjointed constraints\n";
        break;
    case ErrorsType::REDEFINITION_PRIMARY_KEY_E:
        TypeError = ErrorsType::REDEFINITION_PRIMARY_KEY_E;
        msg = "ERROR: Attempt to redefine the constraint PRIMARY KEY\n";
        break;    
    default:
        break;
    }
}

const std::string & erconv::TError::GetMessage() const
{
    return msg;
}