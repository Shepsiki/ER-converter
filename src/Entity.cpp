#include "Entity.h"

/*  
    Simple method to added new field in Entity
    in first he check to currect new Name and at
    the end push new field in Entity
*/
bool erconv::Entity::AddField(
    const std::string & name, 
    const DataTypeEntity & type, 
    const std::vector<ConstraintsEntity> & constr
) 
{
    checkIsValidNameAndUnique(name);
    checkIsValidDataTypeAndConstraints(type, constr);
    
    TEntityField newEntity {name, type, constr};
    Fields.push_back(newEntity);

    return true;
}

bool erconv::Entity::DeleteField(const std::string & name) 
{
    std::vector<TEntityField>::iterator iToDelete = findName(name);

    if (iToDelete == Fields.end()) {
        return false;
    }
    
    Fields.erase(iToDelete);
    return true;
}

const std::vector<erconv::TEntityField> & erconv::Entity::GetAllFields() 
{
    return Fields;
}

/*
    If Field by name not found, function throw an exception
*/
const erconv::TEntityField & erconv::Entity::GetFieldByName(const std::string & name)
{
    for (size_t i = 0; i < Fields.size(); ++i) {
        if (Fields[i].Name == name) {
            return Fields[i];
        }
    }

    throw TError(ErrorsType::NOT_FOUND_ENTITY_NAME_E);
}

const std::vector<erconv::TEntityField>::iterator erconv::Entity::findName(const std::string & nameF) 
{
    for (int i = 0; i < Fields.size(); ++i) {
        if (Fields[i].Name == nameF) {
            return Fields.begin() + i;
        }
    }

    return Fields.end();
}

bool erconv::Entity::checkIsValidNameAndUnique(const std::string & name) 
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
            throw TError(ErrorsType::INVALID_ENTITY_NAME_E);
        }
    }
    // Checking the uniqueness of the name in all fields
    if (findName(name) != Fields.end()) {
        throw TError(ErrorsType::EXISTING_ENTITY_NAME_E);
    }

    return true;
}

/*
    This method checks whether there may be 
    some сonstraints on some data types
*/
bool erconv::Entity::checkIsValidDataTypeAndConstraints(
            const DataTypeEntity & dt, 
            const std::vector<ConstraintsEntity> & constr
)
{
    /*
        We check that there is no invalid
        data type for a certain constraint AND
        checking some colission with constraint.
    */
    for (size_t i = 0; i < constr.size(); ++i) {
        switch (constr[i]) {
        case ConstraintsEntity::UNDEFINED:
            if (dt != DataTypeEntity::UNDEFINED) {
                throw TError(ErrorsType::DATA_TYPE_NOT_DECLARED_E);
                return false;
            }
            break;

        case ConstraintsEntity::NOT_NULL_C:
            for (size_t j = 0; j < constr.size(); ++j) {
                if (
                    constr[j] == ConstraintsEntity::NULL_C ||
                    constr[j] == ConstraintsEntity::PRIMARY_KEY_C ||
                    constr[j] == ConstraintsEntity::FOREIGN_KEY_C
                ) {
                    throw TError(ErrorsType::COLLISION_WITH_CONSTRAINTS_E);
                    return false;
                }
            }
            break;

        case ConstraintsEntity::NULL_C:
            for (size_t j = 0; j < constr.size(); ++j) {
                if (
                    constr[j] == ConstraintsEntity::NOT_NULL_C ||
                    constr[j] == ConstraintsEntity::PRIMARY_KEY_C ||
                    constr[j] == ConstraintsEntity::FOREIGN_KEY_C
                ) {
                    throw TError(ErrorsType::COLLISION_WITH_CONSTRAINTS_E);
                    return false;
                }
            }
            break;

        case ConstraintsEntity::PRIMARY_KEY_C:
            if (hasPrimaryKey == true) {
                throw TError(ErrorsType::REDEFINITION_PRIMARY_KEY_E);
            }

            for (size_t j = 0; j < constr.size(); ++j) {
                if (constr[j] == ConstraintsEntity::NULL_C ||
                    constr[j] == ConstraintsEntity::NOT_NULL_C ||
                    constr[j] == ConstraintsEntity::FOREIGN_KEY_C
                ) {
                    throw TError(ErrorsType::COLLISION_WITH_CONSTRAINTS_E);
                    return false;
                }
            }
            
            hasPrimaryKey = true;
            break;

        case ConstraintsEntity::FOREIGN_KEY_C:
            for (size_t j = 0; j < constr.size(); ++j) {
                if (constr[j] == ConstraintsEntity::NULL_C ||
                    constr[j] == ConstraintsEntity::NOT_NULL_C ||
                    constr[j] == ConstraintsEntity::PRIMARY_KEY_C
                ) {
                    throw TError(ErrorsType::COLLISION_WITH_CONSTRAINTS_E);
                    return false;
                }
            }
            break;
        default: 
            break;
        }
    }

    return true;
}