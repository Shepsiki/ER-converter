#include "../include/Entity.h"

TError Entity::AddField(std::string name, DataTypeEntity type, std::vector<ConstraintsEntity> constr) 
{
    TError nameCheck = CheckIsValidName(name);
    if (nameCheck.TypeError != SUCCESS_VALUE_E) {
        return nameCheck;
    }

    TEntityField newEntity {name, type, constr};

    Fields.push_back(newEntity);

    return TError(SUCCESS_VALUE_E);
}

TError Entity::DeleteField(std::string name) 
{
    std::vector<TEntityField>::iterator iToDelete = FindName(name);
    if (iToDelete == Fields.end()) {
        return TError(NOT_FOUND_NAME_E);
    } else {
        Fields.erase(iToDelete);
    }

    return TError(SUCCESS_VALUE_E);
}

std::vector<TEntityField>::iterator Entity::FindName(std::string nameF) 
{
    for (int i = 0; i < Fields.size(); ++i) {
        if (Fields[i].Name == nameF) {
            return Fields.begin() + i;
        }
    }

    return Fields.end();
}

TError Entity::CheckIsValidName(std::string name) 
{
    // Checking to letter in name has valid diapason
    for (int i = 0; i < name.size(); ++i) {
        if (
            !(
                (name[i] >= 'a' && name[i] <= 'z') || 
                (name[i] >= 'A' && name[i] <= 'Z') || 
                (name[i] == '_') || 
                (name[i] >= '0' && name[i] <= '9')
            )
            ) {
            return TError(INVALID_NAME_E);
        }
    }

    // Checking the uniqueness of the name in all fields
    if (FindName(name) == Fields.end()) {
        return TError(EXISTING_NAME_E);
    }

return TError(SUCCESS_VALUE_E);
}