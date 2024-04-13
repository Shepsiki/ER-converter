#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include "Entity.h"
#include "Relationship.h"
#include "Errors.h"

namespace erconv {

    class ERModel {

    public:
        ERModel() : entities(), relationships() {}
        ~ERModel() = default;

    // СОЗДАНИЕ/УДАЛЕНИЕ:

        bool AddEntity(const std::string name);

        bool AddEntity(Entity& entity);

        bool AddEntityField(const std::string   entityName, 
                            const std::string   fieldName, 
                            const DataTypeEntity & fieldType, 
                            const std::vector<ConstraintsEntity> & fieldConstr);

        bool AddRelationship(Relationship& rel);

        bool AddRelationship(TypeRelationship type, const std::string name, Entity &lhsEntity, Entity &rhsEntity);

        bool RemoveEntity(const std::string& name);

        bool RemoveEntityField(const std::string& name, const std::string& fieldName);

        bool RemoveRelationship(const std::string& name);

    // ПОИСК:

        const Entity* FindEntity(const std::string& name);

        const Relationship* FindRelationship(const std::string& name);

    // ГЕТТЕРЫ:

        const Entity& GetEntity(const std::string& name);

        const Relationship& GetRelationship(const std::string& name);

        const std::vector<Entity>& GetEntities() const;

        const std::vector<Relationship>& GetRelationships() const;

        const std::vector<Relationship> GetConnectedRelationships(const Entity& entity) const;

    // ПРОВЕРКА:

        bool IsEmpty();

    private:
        std::vector<Entity>::iterator findEntityIter(const std::string& name);

        std::vector<Relationship>::iterator findRelationshipIter(const std::string& name);

        bool isValidName(const std::string& name);

    private:
        std::vector<Entity> entities;
        std::vector<Relationship> relationships;

    };

}; // namespace erconv