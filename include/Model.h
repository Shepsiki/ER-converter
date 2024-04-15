#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <set>
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
                            const DataTypeEntity &fieldType, 
                            const std::vector<ConstraintsEntity> &fieldConstr);

        bool AddRelationship(TypeRelationship type,
                             const std::string &lhsEntityName, 
                             const std::string &rhsEntityName,
                             const std::string &foreignKey,
                             const std::string _name="unnamed_relation");

        bool RemoveEntity(const std::string& name);

        bool RemoveEntityField(const std::string& name, const std::string& fieldName);

        bool RemoveRelationship(const std::string &lhsEntityName, 
                                const std::string &rhsEntityName,
                                const std::string &foreignKey);

    // ГЕТТЕРЫ:

        const Entity& GetEntity(const std::string& name);

        const Relationship& GetRelationship(const std::string &lhsEntityName, 
                                            const std::string &rhsEntityName,
                                            const std::string &foreignKey);

        const std::vector<Entity>& GetEntities() const;

        const std::vector<Relationship>& GetRelationships() const;

        const std::vector<Relationship> GetConnectedRelationships(const Entity& entity) const;

    // ПРОВЕРКА:

        bool IsEmpty();

    private:
        const Entity* findEntity(const std::string& name);

        const Relationship* findRelationship(const std::string &lhsEntityName, 
                                             const std::string &rhsEntityName,
                                             const std::string &foreignKey);

        std::vector<Entity>::iterator findEntityIter(const std::string& name);

        std::vector<Relationship>::iterator findRelationshipIter(const std::string &lhsEntityName, 
                                                                 const std::string &rhsEntityName,
                                                                 const std::string &foreignKey);

        bool removeRelationshipByRef(Relationship& rel);

        bool isValidName(const std::string& name);

    private:
        std::vector<Entity> entities;
        std::vector<Relationship> relationships;

        // ... еще чьи-то IDs

    };

}; // namespace erconv