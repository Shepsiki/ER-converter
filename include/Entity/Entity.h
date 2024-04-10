#pragma once

#include <iostream>
#include <vector>

#include "../Errors/Errors.h"

/*
    This enum contains all valid data
    in the fields of the entity.
*/
enum DataTypeEntity {
    SMALL_INT_T = 0,
    INT_T,
    BIG_INT_T,

    NUMERIC_DEC_T,
    REAL_T,
    DOUBLE_PRECIS_T, 
    FLOAT_T,

    CHAR_T,
    VARCHAR_T,
    TEXT_T, 

    DATE_T,
    TIME_T,
    TIMESTAMP_WITH_TZ_T,
    TIMESTAMP_WITHOUT_TZ_T,
};

/*
    This enum contains all valid
    constraints in the some fileds
    of the entity
*/
enum ConstraintsEntity {
    NOT_NULL_C = 0,
    UNIQUE_C,
    PRIMARY_KEY_C,
    FOREIGN_KEY_C,
    CHECK_C,
    DEFAULT_C,
    CREATE_INDEX_C,
};

typedef struct _T_entity_field
{
    std::string Name;
    DataTypeEntity DataType;
    std::vector<ConstraintsEntity> Constraints;
} TEntityField;


class Entity
{
public:
    Entity() = default;
    Entity(std::string nameEn) : EntityName(nameEn) {}

    TError AddField(std::string name, DataTypeEntity type, std::vector<ConstraintsEntity> constr) 
    {
        TError nameCheck = CheckIsValidName(name);
        if (nameCheck.TypeError != SUCCESS_VALUE_E) {
            return nameCheck;
        }

        TEntityField newEntity {name, type, constr};

        Fields.push_back(newEntity);

        return TError(SUCCESS_VALUE_E);
    }

    TError DeleteField(std::string name) 
    {
        std::vector<TEntityField>::iterator iToDelete = FindName(name);
        if (iToDelete == Fields.end()) {
            return TError(NOT_FOUND_NAME_E);
        } else {
            Fields.erase(iToDelete);
        }

        return TError(SUCCESS_VALUE_E);
    }

    ~Entity() = default;

private:
    std::vector<TEntityField>::iterator FindName(std::string nameF) 
    {
        for (int i = 0; i < Fields.size(); ++i) {
            if (Fields[i].Name == nameF) {
                return Fields.begin() + i;
            }
        }

        return Fields.end();
    }

    TError CheckIsValidName(std::string name) 
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

private:
    std::string EntityName = "NO_NAME";
    std::vector<TEntityField> Fields;
};
