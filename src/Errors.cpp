#include "../include/Errors.h"

TError::TError(ErrorsType err) 
{
    switch (err)
    {
    case SUCCESS_VALUE_E:
        TypeError = SUCCESS_VALUE_E;
        msg = "OK: Success code received, function completed successfully\n";
        break;
    case INVALID_NAME_E:
        TypeError = INVALID_NAME_E;
        msg = "ERROR: The field name has invalid characters\n";
        break;
    case EXISTING_NAME_E:
        TypeError = EXISTING_NAME_E;
        msg = "ERROR: This name is already used in the fields of this entity\n";
        break;
    case NOT_FOUND_NAME_E:
        TypeError = NOT_FOUND_NAME_E;
        msg = "ERROR: There is no field with this name\n";
        break;
    default:
        break;
    }
}