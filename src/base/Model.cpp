#include "base/Model.h"

using namespace erconv;

// СОЗДАНИЕ/УДАЛЕНИЕ:

ERModel::EntityID ERModel::AddEntity(const std::string name) {
    Entity newEntity(name);
    AddEntity(newEntity);
    return true;
}

ERModel::EntityID ERModel::AddEntity(Entity& entity) {
    // Check if an entity with the same name already exists
    if (HasEntityWithName(entity.GetName())) {
        throw TError(ErrorsType::EXISTING_ENTITY_NAME_E);
    }
    entities.push_back(entity);
    EntityID newId = idmEntities.NewID();
    entitiesMapping[newId] = entity;
    namesToIDsMapping[entity.GetName()] = newId;
    graph.AddVertex(newId);
    return true;
}

bool ERModel::AddEntityField(const std::string entityName, 
                             const std::string fieldName, 
                             const DataTypeEntity & fieldType, 
                             const std::vector<ConstraintsEntity> & fieldConstr) 
{
    if (!HasEntityWithName(entityName)) {
        throw TError(ErrorsType::NOT_FOUND_ENTITY_FIELD_NAME_E);
    }
    Entity& ent = entitiesMapping[namesToIDsMapping[entityName]];
    for (const auto& c : fieldConstr) {
        if (c == ConstraintsEntity::FOREIGN_KEY_C) {
            throw TError("ERModel: Trying to add entity with foreign key.");
        }
    }
    return ent.AddField(fieldName, fieldType, fieldConstr);
}

bool ERModel::AddRelationship(TypeRelationship type,
                              const std::string &lhsEntityName, 
                              const std::string &rhsEntityName,
                              const std::string &foreignKey,
                              const std::string _name)
{
    for (const auto& p : relationshipsMapping) {
        if (
            p.second.GetLhsEntityName() == lhsEntityName &&
            p.second.GetRhsEntityName() == rhsEntityName &&
            p.second.GetForeignKey() == foreignKey
        ) {
            throw TError(ErrorsType::EXISTING_RELATIONSHIP_E);
        }
    }

    EntityID lhs = GetEntityIDByName(lhsEntityName);
    EntityID rhs = GetEntityIDByName(rhsEntityName);

    const std::string &primaryKey = entitiesMapping[lhs].GetPrimaryKeyName();
    const TEntityField& field = entitiesMapping[rhs].GetFieldByName(foreignKey);
    if (!field.HasConstraint(ConstraintsEntity::FOREIGN_KEY_C)) {
        throw TError("Trying to create relationship to non foreign key attribute.");
    }

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
    if (!entitiesMapping[rhs].SetForeignKeyForField(foreignKey)) {
        // <ENDED HERE> %%%
    }
    // Meow
    relationshipsMapping[relId] = newRel;

    relationships.push_back(newRel);

    return true;
}

bool ERModel::RemoveEntity(const std::string& name) {
    auto it = findEntityIter(name);

    if (it != entities.end()) {
        entities.erase(it);
        // Remove connected relationships
        for (auto& relationship : relationships) {
            if ( name == relationship.GetLhsEntityName() || 
                name == relationship.GetRhsEntityName() ) 
            {
                removeRelationshipByRef(relationship);
            }
        }
        return true; // Entity removed successfully
    } else {
        throw TError(ErrorsType::NOT_FOUND_ENTITY_NAME_E);
        return false; // Entity not found
    }
}

bool ERModel::RemoveEntityField(const std::string& name, const std::string& fieldName) {
    auto it = findEntityIter(name);

    if (it != entities.end()) {
        if (it->DeleteField(fieldName)) {
            return true; // Entity field removed successfully
        } else {
            throw TError(ErrorsType::NOT_FOUND_ENTITY_FIELD_NAME_E);
        }
    } else {
        throw TError(ErrorsType::NOT_FOUND_ENTITY_NAME_E);
        // Entity not found
    }
    return false; 
}

bool ERModel::RemoveRelationship(const std::string &lhsEntityName, 
                                 const std::string &rhsEntityName,
                                 const std::string &foreignKey) 
{
    auto it = findRelationshipIter(lhsEntityName, rhsEntityName, foreignKey);

    if (it != relationships.end()) {
        relationships.erase(it);
        return true; // Relationship removed successfully
    } else {
        throw TError(ErrorsType::NOT_FOUND_RELATIONSHIP_E);
        return false; // Relationship not found
    }
}

// ГЕТТЕРЫ:

ERModel::EntityID ERModel::GetEntityIDByName(const std::string& name) {
    if (!HasEntityWithName(name)) {
        throw TError(ErrorsType::NOT_FOUND_ENTITY_NAME_E); 
    }
    return namesToIDsMapping[name];
}

const Entity& ERModel::GetEntity(const std::string& name) {
    if (!HasEntityWithName(name)) {
        throw TError(ErrorsType::NOT_FOUND_ENTITY_NAME_E); 
    }
    return entitiesMapping[namesToIDsMapping[name]];
}

const Relationship& ERModel::GetRelationship(const std::string &lhsEntityName, 
                                             const std::string &rhsEntityName,
                                             const std::string &foreignKey) 
{
    const Relationship* rel = findRelationship(lhsEntityName, rhsEntityName, foreignKey);
    if (rel != nullptr) {
        return *rel;
    }

    throw TError(ErrorsType::NOT_FOUND_RELATIONSHIP_E); //
}

const std::vector<Entity>& ERModel::GetEntities() const {
    return entities;
}

const std::vector<Relationship>& ERModel::GetRelationships() const {
    return relationships;
}

const std::vector<Relationship> ERModel::GetConnectedRelationships(const Entity& entity) const {
    std::vector<Relationship> connectedRelationships;
    const std::string entityName = entity.GetName();

    for (const auto& relationship : relationships) {
        if ( entityName == relationship.GetLhsEntityName() || 
             entityName == relationship.GetRhsEntityName() ) 
        {
            // If the entity is involved in the relationship, add it to the vector
            connectedRelationships.push_back(relationship);
        }
    }

    return connectedRelationships;
}


// ПРОВЕРКА:

bool ERModel::IsEmpty() {
    return entities.empty();
}

bool ERModel::HasEntityWithName(const std::string& name) {
    if (namesToIDsMapping.count(name) != 0) {
        return true;
    }
    return false;
}

// PRIVATE SCOPE:

const Entity* ERModel::findEntity(const std::string& name) {
    auto it = findEntityIter(name);
    if (it != entities.end()) {
        return std::addressof(*it);
    } else {
        return nullptr;
    }
}

const Relationship* ERModel::findRelationship(const std::string &lhsEntityName, 
                                              const std::string &rhsEntityName,
                                              const std::string &foreignKey) 
{
    auto it = findRelationshipIter(lhsEntityName, rhsEntityName, foreignKey);
    if (it != relationships.end()) {
        return std::addressof(*it);
    } else {
        return nullptr;
    }
}

std::vector<Entity>::iterator ERModel::findEntityIter(const std::string& name) {
    return std::find_if(entities.begin(), entities.end(), [name](const Entity& entity) {
        return entity.GetName() == name;
    });
}

std::vector<Relationship>::iterator ERModel::findRelationshipIter(const std::string &lhsEntityName, 
                                                                  const std::string &rhsEntityName,
                                                                  const std::string &foreignKey) 
{
    return std::find_if(relationships.begin(), relationships.end(), 
        [&](const Relationship& relationship) {
            return relationship.GetLhsEntityName() == lhsEntityName &&
                   relationship.GetRhsEntityName() == rhsEntityName &&
                   relationship.GetForeignKey()    == foreignKey;
        });
}

bool ERModel::removeRelationshipByRef(Relationship &rel) {
    auto it = findRelationshipIter(rel.GetLhsEntityName(), rel.GetRhsEntityName(), rel.GetForeignKey());

    if (it != relationships.end()) {
        relationships.erase(it);
        return true; // Relationship removed successfully
    } else {
        throw TError(ErrorsType::NOT_FOUND_RELATIONSHIP_E);
        return false; // Relationship not found
    }
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
