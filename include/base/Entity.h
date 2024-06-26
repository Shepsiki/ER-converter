#pragma once

#include <iostream>
#include <vector>

#include "Errors.h"

namespace erconv {
    /*
        This enum contains all valid data
        in the fields of the entity.
    */
    enum class DataTypeEntity {
        UNDEFINED = 0,

        SMALL_INT_T,
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

   //Удалил из класса ниже ограничение NULL_C - такого просто нет
    enum class ConstraintsEntity {
        UNDEFINED = 0,
        NOT_NULL_C,
        UNIQUE_C,
        PRIMARY_KEY_C,
        FOREIGN_KEY_C,
        DEFAULT_C,
    };

    /*
        This type implements the representation 
        of each entity field. The NAME field is unique
    */
    struct TEntityField {
        std::string Name = "no_name";
        DataTypeEntity DataType = DataTypeEntity::UNDEFINED;
        std::vector<ConstraintsEntity> Constraints {ConstraintsEntity::UNDEFINED};

        bool HasConstraint(const ConstraintsEntity& con) const;
    };

    /*
        This class is a complete representation of a single entity 
        in databases. Its ability is to add or remove a field with
        all possible types and restrictions.
    */
    class Entity
    {
    public:
        Entity(const std::string & nameEn) : EntityName(nameEn) {}

        bool AddField(
            const std::string & name, 
            const DataTypeEntity & type, 
            const std::vector<ConstraintsEntity> & constr
        );
        bool DeleteField(const std::string & name);
        void DeleteAllFields(); // added by Lenina-Paket

        const std::vector<TEntityField> & GetAllFields() const; // <-- Добавил const
        const TEntityField & GetFieldByName(const std::string & name) const; // <-- Добавил const

        bool SetForeignKeyForField(const std::string& fieldName);
        bool UnsetForeignKeyForField(const std::string& fieldName);

        Entity() : EntityName("NO_NAME") {}
        std::string GetName() const;
        bool operator==(const Entity& other) const;
        bool HasPrimaryKey() const;
        const std::string GetPrimaryKeyName() const;

        ~Entity() = default;

    private:
        const std::vector<TEntityField>::iterator findName(const std::string & nameF);
        bool checkIsValidNameAndUnique(const std::string & name);
        bool checkIsValidDataTypeAndConstraints(
            const DataTypeEntity & dt, 
            const std::vector<ConstraintsEntity> & constr
        );

    private:
        std::string EntityName = "NO_NAME";
        std::vector<TEntityField> Fields;
        /*
            This field (global variable) that is needed for validation
            is one of the uses of a primary key.
        */
        bool hasPrimaryKey = false;
    };    
} // namespace erconv
