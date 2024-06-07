#include "base/Model.h"

using namespace erconv;

//COPY CONSTRUCTOR

ERModel& ERModel::operator=(const ERModel& model) {
    this->graph = model.graph;
    this->entities = model.entities;
    this->entitiesNamesToIDsMapping = model.entitiesNamesToIDsMapping;
    this->idmEntities = model.idmEntities;
    this->idmRelationships = model.idmRelationships;
    this->relationships = model.relationships;
    this->relationshipsToIDsMapping = model.relationshipsToIDsMapping;
    
    return *this;
}

// СОЗДАНИЕ/УДАЛЕНИЕ:

ERModel::EntityID ERModel::AddEntity(const std::string name) {
    if (!isValidName(name)) {
        throw TError(ErrorsType::INVALID_ENTITY_NAME_E);
    }
    Entity newEntity(name);
    AddEntity(newEntity);
    return true;
}

ERModel::EntityID ERModel::AddEntity(Entity& entity) {
    // Check if an entity with the same name already exists
    if (HasEntityWithName(entity.GetName())) {
        throw TError(ErrorsType::EXISTING_ENTITY_NAME_E);
    }
    EntityID newId = idmEntities.NewID();
    entities[newId] = entity;
    entitiesNamesToIDsMapping[entity.GetName()] = newId;
    graph.AddVertex(newId);
    return newId;
}

bool ERModel::AddEntityField(const std::string entityName, 
                             const std::string fieldName, 
                             const DataTypeEntity & fieldType, 
                             const std::vector<ConstraintsEntity> & fieldConstr) 
{
    if (!HasEntityWithName(entityName)) {
        throw TError(ErrorsType::NOT_FOUND_ENTITY_FIELD_NAME_E);
    }
    Entity& ent = entities[entitiesNamesToIDsMapping[entityName]];
    
    return ent.AddField(fieldName, fieldType, fieldConstr);
}

ERModel::RelationshipID ERModel::AddRelationship(TypeRelationship type,
                                                 const std::string &lhsEntityName, 
                                                 const std::string &rhsEntityName,
                                                 const std::string &foreignKey,
                                                 const std::string _name) {
    TripleString info = { lhsEntityName, rhsEntityName, foreignKey };
    if (relationshipsToIDsMapping.count(info) != 0) {
        throw TError(ErrorsType::EXISTING_RELATIONSHIP_E);
    }

    EntityID lhs = GetEntityIDByName(lhsEntityName);
    EntityID rhs = GetEntityIDByName(rhsEntityName);

    const std::string &primaryKey = entities[lhs].GetPrimaryKeyName();

    const TEntityField& field = entities[rhs].GetFieldByName(foreignKey);

    std::string newName = _name;
    if (_name == "unnamed_relation") {
        std::string issuer = lhsEntityName;
        std::string receiver = rhsEntityName;
        std::string forKey = foreignKey;
        newName = issuer + "_to_" + receiver + "_" + forKey;
    } else if (!isValidName(_name)) {
        throw TError(ErrorsType::INVALID_RELATIONSHIP_NAME_E);
    }

    Relationship newRel(this, type, lhsEntityName, rhsEntityName, newName);
    RelationshipID relId = idmRelationships.NewID();
    // Trying to add in graph:
    graph.AddEdge(rhs, lhs, relId);
    if (graph.HasCycle()) {
        graph.RemoveEdge(relId);
        throw TError("ERModel: Trying to add cyclic dependencies in relationships graph.");
    }
    newRel.SetPrimaryKey(primaryKey);
    newRel.SetForeignKey(foreignKey);
    if (!entities[rhs].SetForeignKeyForField(foreignKey)) {
        throw TError("Can't create such relationship! (Can't set foreign key to " + foreignKey + ")");
    }

    relationshipsToIDsMapping[info] = relId;
    relationships.insert({relId, newRel});

    return relId;
}

void ERModel::RemoveEntity(const EntityID id) {
    if (entities.count(id) == 0) {
        throw TError("Invalid entity ID!");
    }
    graph.RemoveVertex(id);
    std::vector<RelationshipID> relToDelete;
    for (const auto& p : relationships) {
        auto relId = p.first;
        if (!graph.HasEdge(relId)) {
            relToDelete.push_back(relId);
        }
    }
    for (const auto& relId : relToDelete) {
        auto& rel = relationships[relId];
        TripleString info = {
            rel.GetLhsEntityName(),
            rel.GetRhsEntityName(),
            rel.GetForeignKey()
        };
        entities[
            entitiesNamesToIDsMapping[rel.GetRhsEntityName()]
        ].UnsetForeignKeyForField(rel.GetForeignKey());
        relationshipsToIDsMapping.erase(info);
        relationships.erase(relId);
    }

    entitiesNamesToIDsMapping.erase(entities[id].GetName());
    entities.erase(id);
}

bool ERModel::RemoveEntity(const std::string& name) {
    if (entitiesNamesToIDsMapping.count(name) == 0) {
        throw TError(ErrorsType::NOT_FOUND_ENTITY_FIELD_NAME_E);
    }
    RemoveEntity(entitiesNamesToIDsMapping[name]);
    return true;
}

bool ERModel::RemoveEntityField(const std::string& name, const std::string& fieldName) {
    if (entitiesNamesToIDsMapping.count(name) == 0) {
        throw TError(ErrorsType::INVALID_ENTITY_NAME_E);
    }
    // TODO: проверка на наличие связи к полю, если он помечен FOREIGN KEY
    Entity& entity = entities[entitiesNamesToIDsMapping[name]];
    if (!entity.DeleteField(fieldName)) {
        throw TError(ErrorsType::INVALID_ENTITY_FIELD_NAME_E);
    }
    return true; 
}

void ERModel::RemoveRelationship(const RelationshipID id) {
    if (relationships.count(id) == 0) {
        throw TError("Invalid relationship ID!");
    }
    graph.RemoveEdge(id);
    auto& rel = relationships[id];
    TripleString info = {
        rel.GetLhsEntityName(),
        rel.GetRhsEntityName(),
        rel.GetForeignKey()
    };
    entities[id].UnsetForeignKeyForField(relationships[id].GetForeignKey());
    relationships.erase(id);
}

bool ERModel::RemoveRelationship(const std::string &lhsEntityName, 
                                 const std::string &rhsEntityName,
                                 const std::string &foreignKey) {
    TripleString info = {
        lhsEntityName,
        rhsEntityName,
        foreignKey
    };
    if (relationshipsToIDsMapping.count(info) == 0) {
        throw TError(ErrorsType::NOT_FOUND_RELATIONSHIP_E);
    }
    RemoveRelationship(relationshipsToIDsMapping[info]);

    return true;
}

void ERModel::RemoveAllRelationships() {
    relationships.clear();
}

// ГЕТТЕРЫ:

ERModel::EntityID ERModel::GetEntityIDByName(const std::string& name) {
    if (!HasEntityWithName(name)) {
        throw TError(ErrorsType::NOT_FOUND_ENTITY_NAME_E); 
    }
    return entitiesNamesToIDsMapping[name];
}

const Entity& ERModel::GetEntity(const std::string& name) {
    if (!HasEntityWithName(name)) {
        throw TError(ErrorsType::NOT_FOUND_ENTITY_NAME_E); 
    }
    return entities[entitiesNamesToIDsMapping[name]];
}

const Entity& ERModel::GetEntity(const EntityID id) {
    if (entities.count(id) == 0) {
        throw TError("Invalid entity ID!");
    }
    return entities[id];
}

const Relationship& ERModel::GetRelationship(const RelationshipID id) {
    if (relationships.count(id) == 0) {
        throw TError("Invalid relationship ID!");
    }
    return relationships[id];
}

const Relationship& ERModel::GetRelationship(const std::string &lhsEntityName, 
                                             const std::string &rhsEntityName,
                                             const std::string &foreignKey) {
    TripleString info = {lhsEntityName, rhsEntityName, foreignKey};
    if (relationshipsToIDsMapping.count(info) == 0) {
        throw TError(ErrorsType::NOT_FOUND_RELATIONSHIP_E);
    }
    return relationships[relationshipsToIDsMapping[info]];
}

std::vector<ERModel::EntityID> ERModel::GetEntities() const {
    std::vector<EntityID> result;
    for (const auto& p : entities) {
        result.push_back(p.first);
    }
    return result;
}

std::vector<ERModel::RelationshipID> ERModel::GetRelationships() const {
    std::vector<RelationshipID> result;
    for (const auto& p : relationships) {
        result.push_back(p.first);
    }
    return result;
}

const std::vector<ERModel::RelationshipID> ERModel::GetConnectedRelationships(const EntityID id) const {
    std::vector<RelationshipID> connectedRelationships;
    if (entities.count(id) == 0) {
        throw TError("Invalid entity ID!");
    }
    auto v1 = graph.GetEdgesFromCurrent(id);
    auto v2 = graph.GetEdgesToCurrent(id);
    for (const auto& i : v1) {
        connectedRelationships.push_back(i);
    }
    for (const auto& i : v2) {
        connectedRelationships.push_back(i);
    }
    return connectedRelationships;
}

const std::vector<ERModel::RelationshipID> ERModel::GetConnectedRelationshipsInWhichEntityIsAChild(const EntityID id) const {
    std::vector<RelationshipID> connectedRelationships;
    if (entities.count(id) == 0) {
        throw TError("Invalid entity ID!");
    }
    auto v = graph.GetEdgesFromCurrent(id);
    for (const auto& i : v) {
        connectedRelationships.push_back(i);
    }
    return connectedRelationships;
}

// ПРОВЕРКА:

bool ERModel::IsEmpty() {
    return entities.empty();
}

bool ERModel::HasEntityWithName(const std::string& name) {
    if (entitiesNamesToIDsMapping.count(name) != 0) {
        return true;
    }
    return false;
}

bool ERModel::isValidName(const std::string& name) {
    if (name.size() == 0) {
        return false;
    }
    for (int i = 0; i < name.size(); ++i) {
        if (  !((name[i] >= 'a' && name[i] <= 'z') || 
                (name[i] >= 'A' && name[i] <= 'Z') || 
                (name[i] == '_') || 
                (name[i] >= '0' && name[i] <= '9'))) 
        {
            return false;
        }
    }
    return true;
}

std::vector<ERModel::EntityID> ERModel::GetEntitesInTopologicalOrder() const {
    return graph.GetVerticesInTopologicalOrder();
}

std::vector<ERModel::EntityID> ERModel::GetEntitiesOnWhomCurrentDepends(const EntityID& id) const {
    auto s = graph.GetVerticesReachableFromCurrent(id);
    std::vector<EntityID> result;
    for (const auto& i : s) {
        result.push_back(i);
    }
    return result;
}
