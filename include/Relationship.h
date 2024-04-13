#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include "Entity.h"

namespace erconv {
    enum TypeRelationship {
        UNDEFINED_RELATIONSHIP_T = 0,

        ONE_TO_ONE_T, // IDENTIFYING_T,
        ONE_TO_MANY_T, // NON_IDENTIFYING_T,
        MANY_TO_MANY_T,
    };

    class Relationship {
    // МЕТОДЫ
    public:
        Relationship(TypeRelationship   _type, 
                     const std::string &_relationName, 
                     Entity*            _lhsEntity, 
                     Entity*            _rhsEntity) :
            type(_type), 
            relationName(_relationName), 
            lhsEntity(_lhsEntity), 
            rhsEntity(_rhsEntity) {}

        Relationship() : 
        type(UNDEFINED_RELATIONSHIP_T), 
        relationName("NO_NAME"),
        lhsEntity(nullptr),
        rhsEntity(nullptr) {}

        std::string      GetName() const;

        TypeRelationship GetType() const;

        const Entity*    GetLhsEntity() const;

        const Entity*    GetRhsEntity() const;

        ~Relationship() = default;

        friend std::ostream& operator<<(std::ostream& os, const Relationship& rel);

    private:

    // ПОЛЯ
    private:
        TypeRelationship type;

        std::string relationName;

        Entity* lhsEntity;
        Entity* rhsEntity;

    };

    // ПЕРЕВОД ТИПА ОТНОШЕНИЯ в СТРОКУ
    //std::string TypeRelationship2Str(TypeRelationship type); <-- в .cpp

}; // namespace erconv

