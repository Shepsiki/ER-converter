#pragma once

#include <iostream>
#include <vector>

#include "./Errors.h"

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

    TError AddField(
        std::string name, 
        DataTypeEntity type, 
        std::vector<ConstraintsEntity> constr
    );

    TError DeleteField(std::string name);

    ~Entity() = default;

private:
    std::vector<TEntityField>::iterator FindName(std::string nameF);
    TError CheckIsValidName(std::string name);

private:
    std::string EntityName = "NO_NAME";
    std::vector<TEntityField> Fields;
};
