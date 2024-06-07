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

    class ERModel;

    class Relationship {
    // МЕТОДЫ
    public:
        Relationship() = default;

        Relationship(ERModel* _model,
                     TypeRelationship _type, 
                     const std::string  &_lhsEntityName, 
                     const std::string  &_rhsEntityName, 
                     std::string _name) :
            parentModel(_model), 
            type(_type), 
            lhsEntityName(_lhsEntityName), 
            rhsEntityName(_rhsEntityName), 
            name(_name),
            primaryKeyName(""),
            foreignKeyName("") {}

        // СЕТТЕРЫ

        void SetPrimaryKey(const std::string &keyName);

        void SetForeignKey(const std::string &keyName);

        // ГЕТТЕРЫ

        const TypeRelationship GetType() const;

        const std::string& GetLhsEntityName() const;

        const std::string& GetRhsEntityName() const;

        const std::string& GetPrimaryKey() const;

        const std::string& GetForeignKey() const;

        const std::string& GetName() const;

        const ERModel* GetModelPtr() const;

        ~Relationship() = default;

    private:

    // ПОЛЯ
    public:
        

    private:
        const ERModel* parentModel;

        TypeRelationship type;
        
        std::string lhsEntityName;
        std::string rhsEntityName;

        std::string primaryKeyName;
        std::string foreignKeyName;

        std::string name;
    };

};
