#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <set>
#include <unordered_map>

#include "base/Entity.h"
#include "base/Relationship.h"
#include "base/Errors.h"
#include "base/Graph.h"
#include "base/IDManager.h"

namespace erconv {

    class ERModel {
    public:
        using EntityID = size_t;
        using RelationshipID = size_t;

        ERModel() = default;
        ~ERModel() = default;

        EntityID AddEntity(const std::string name);
        EntityID AddEntity(Entity& entity);
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

        EntityID GetEntityIDByName(const std::string& name);
        const Entity& GetEntity(const std::string& name);
        const Relationship& GetRelationship(const std::string &lhsEntityName, 
                                            const std::string &rhsEntityName,
                                            const std::string &foreignKey);
        const std::vector<Entity>& GetEntities() const;
        const std::vector<Relationship>& GetRelationships() const;
        const std::vector<Relationship> GetConnectedRelationships(const Entity& entity) const;

        bool IsEmpty();
        bool HasEntityWithName(const std::string& name);

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

        std::unordered_map<std::string, EntityID> namesToIDsMapping;
        erconv::IDManager idmEntities, idmRelationships;
        std::unordered_map<EntityID, Entity> entitiesMapping;
        std::unordered_map<RelationshipID, Relationship> relationshipsMapping;
        erconv::DirectedGraph<EntityID, RelationshipID> graph;
    };

} // namespace erconv
