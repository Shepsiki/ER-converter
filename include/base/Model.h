#pragma once

#include <array>
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

        ERModel& operator=(const ERModel& model);

        EntityID AddEntity(const std::string name);
        EntityID AddEntity(Entity& entity);
        bool AddEntityField(const std::string   entityName, 
                            const std::string   fieldName, 
                            const DataTypeEntity &fieldType, 
                            const std::vector<ConstraintsEntity> &fieldConstr);
        RelationshipID AddRelationship(TypeRelationship type,
                                       const std::string &lhsEntityName, 
                                       const std::string &rhsEntityName,
                                       const std::string &foreignKey,
                                       const std::string _name="unnamed_relation");

        void RemoveEntity(const EntityID id);
        bool RemoveEntity(const std::string& name);
        bool RemoveEntityField(const std::string& name, const std::string& fieldName);
        void RemoveRelationship(const RelationshipID id);
        bool RemoveRelationship(const std::string &lhsEntityName, 
                                const std::string &rhsEntityName,
                                const std::string &foreignKey);
        void RemoveAllRelationships();

        EntityID GetEntityIDByName(const std::string& name);
        const Entity& GetEntity(const std::string& name);
        const Entity& GetEntity(const EntityID id);
        const Relationship& GetRelationship(const RelationshipID id);
        const Relationship& GetRelationship(const std::string &lhsEntityName, 
                                            const std::string &rhsEntityName,
                                            const std::string &foreignKey);
        std::vector<EntityID> GetEntities() const;
        std::vector<RelationshipID> GetRelationships() const;
        const std::vector<RelationshipID> GetConnectedRelationships(const EntityID id) const;
        const std::vector<RelationshipID> GetConnectedRelationshipsInWhichEntityIsAChild(const EntityID id) const; // <- Added new getter

        bool IsEmpty();
        bool HasEntityWithName(const std::string& name);

        // Special functions:
        std::vector<EntityID> GetEntitesInTopologicalOrder() const;
        std::vector<EntityID> GetEntitiesOnWhomCurrentDepends(const EntityID& id) const;

    private:
        bool isValidName(const std::string& name);

    private:
        // Stores info about relationship
        using TripleString = std::array<std::string, 3>;

        // Mapping names -> id's
        std::unordered_map<std::string, EntityID> entitiesNamesToIDsMapping;
        std::map<TripleString, RelationshipID> relationshipsToIDsMapping;

        // Id generators
        erconv::IDManager idmEntities, idmRelationships;

        // Containers
        std::unordered_map<EntityID, Entity> entities;
        std::unordered_map<RelationshipID, Relationship> relationships;
        
        // Graph of the model
        erconv::DirectedGraph<EntityID, RelationshipID> graph;
    };

} // namespace erconv
