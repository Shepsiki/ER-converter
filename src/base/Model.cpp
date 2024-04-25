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
    EntityID newId = idmEntities.NewID();
    entitiesMapping[newId] = entity;
    entitiesNamesToIDsMapping[entity.GetName()] = newId;
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
    Entity& ent = entitiesMapping[entitiesNamesToIDsMapping[entityName]];
    for (const auto& c : fieldConstr) {
        if (c == ConstraintsEntity::FOREIGN_KEY_C) {
            throw TError("ERModel: Trying to add entity with foreign key.");
        }
    }
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
    if (entitiesMapping[rhs].GetFieldByName(foreignKey).HasConstraint(ConstraintsEntity::FOREIGN_KEY_C)) {
        throw TError("Right entity already has reference in the field " + foreignKey);
    }
    if (!entitiesMapping[rhs].SetForeignKeyForField(foreignKey)) {
        throw TError("Can't create such relationship! (Can't set foreign key to " + foreignKey + ")");
    }
    relationshipsMapping[relId] = newRel;

    return true;
}

void ERModel::RemoveEntity(const EntityID id) {
    if (entitiesMapping.count(id) == 0) {
        throw TError("Invalid entity ID!");
    }
    graph.RemoveVertex(id);
    std::vector<RelationshipID> relToDelete;
    for (const auto& p : relationshipsMapping) {
        auto relId = p.first;
        if (!graph.HasEdge(relId)) {
            relToDelete.push_back(relId);
        }
    }
    for (const auto& relId : relToDelete) {
        auto& rel = relationshipsMapping[relId];
        TripleString info = {
            rel.GetLhsEntityName(),
            rel.GetRhsEntityName(),
            rel.GetForeignKey()
        };
        entitiesMapping[
            entitiesNamesToIDsMapping[rel.GetRhsEntityName()]
        ].UnsetForeignKeyForField(rel.GetForeignKey());
        relationshipsToIDsMapping.erase(info);
        relationshipsMapping.erase(relId);
    }

    entitiesNamesToIDsMapping.erase(entitiesMapping[id].GetName());
    entitiesMapping.erase(id);
}

bool ERModel::RemoveEntity(const std::string& name) {
    if (entitiesNamesToIDsMapping.count(name) == 0) {
        throw TError(ErrorsType::NOT_FOUND_ENTITY_FIELD_NAME_E);
    }
    RemoveEntity(entitiesNamesToIDsMapping[name]);
}

bool ERModel::RemoveEntityField(const std::string& name, const std::string& fieldName) {
    if (entitiesNamesToIDsMapping.count(name) == 0) {
        throw TError(ErrorsType::INVALID_ENTITY_NAME_E);
    }
    // TODO: проверка на наличие связи к полю, если он помечен FOREIGN KEY
    Entity& entity = entitiesMapping[entitiesNamesToIDsMapping[name]];
    entity.DeleteField(fieldName);
    return false; 
}

void ERModel::RemoveRelationship(const RelationshipID id) {
    if (relationshipsMapping.count(id) == 0) {
        throw TError("Invalid relationship ID!");
    }
    graph.RemoveEdge(id);
    auto& rel = relationshipsMapping[id];
    TripleString info = {
        rel.GetLhsEntityName(),
        rel.GetRhsEntityName(),
        rel.GetForeignKey()
    };
    entitiesMapping[id].UnsetForeignKeyForField(relationshipsMapping[id].GetForeignKey());
    relationshipsMapping.erase(id);
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
    return entitiesMapping[entitiesNamesToIDsMapping[name]];
}

const Entity& ERModel::GetEntity(const EntityID id) {
    if (entitiesMapping.count(id) == 0) {
        throw TError("Invalid entity ID!");
    }
    return entitiesMapping[id];
}

const Relationship& ERModel::GetRelationship(const RelationshipID id) {
    if (relationshipsMapping.count(id) == 0) {
        throw TError("Invalid relationship ID!");
    }
    return relationshipsMapping[id];
}

const Relationship& ERModel::GetRelationship(const std::string &lhsEntityName, 
                                             const std::string &rhsEntityName,
                                             const std::string &foreignKey) {
    // TODO: Implement method
    // const Relationship* rel = findRelationship(lhsEntityName, rhsEntityName, foreignKey);
    // if (rel != nullptr) {
    //     return *rel;
    // }

    throw TError(ErrorsType::NOT_FOUND_RELATIONSHIP_E); //
}

std::vector<ERModel::EntityID> ERModel::GetEntities() const {
    std::vector<EntityID> result;
    for (const auto& p : entitiesMapping) {
        result.push_back(p.first);
    }
    return result;
}

std::vector<ERModel::RelationshipID> ERModel::GetRelationships() const {
    std::vector<EntityID> result;
    for (const auto& p : relationshipsMapping) {
        result.push_back(p.first);
    }
    return result;
}

const std::vector<ERModel::RelationshipID> ERModel::GetConnectedRelationships(const EntityID id) const {
    std::vector<RelationshipID> connectedRelationships;
    if (relationshipsMapping.count(id) == 0) {
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


// ПРОВЕРКА:

bool ERModel::IsEmpty() {
    return entitiesMapping.empty();
}

bool ERModel::HasEntityWithName(const std::string& name) {
    if (entitiesNamesToIDsMapping.count(name) != 0) {
        return true;
    }
    return false;
}

// PRIVATE SCOPE:

// const Entity* ERModel::findEntity(const std::string& name) {
//     auto it = findEntityIter(name);
//     if (it != entities.end()) {
//         return std::addressof(*it);
//     } else {
//         return nullptr;
//     }
// }

// const Relationship* ERModel::findRelationship(const std::string &lhsEntityName, 
//                                               const std::string &rhsEntityName,
//                                               const std::string &foreignKey) 
// {
//     auto it = findRelationshipIter(lhsEntityName, rhsEntityName, foreignKey);
//     if (it != relationships.end()) {
//         return std::addressof(*it);
//     } else {
//         return nullptr;
//     }
// }

// std::vector<Entity>::iterator ERModel::findEntityIter(const std::string& name) {
//     return std::find_if(entities.begin(), entities.end(), [name](const Entity& entity) {
//         return entity.GetName() == name;
//     });
// }

// std::vector<Relationship>::iterator ERModel::findRelationshipIter(const std::string &lhsEntityName, 
//                                                                   const std::string &rhsEntityName,
//                                                                   const std::string &foreignKey) 
// {
//     return std::find_if(relationships.begin(), relationships.end(), 
//         [&](const Relationship& relationship) {
//             return relationship.GetLhsEntityName() == lhsEntityName &&
//                    relationship.GetRhsEntityName() == rhsEntityName &&
//                    relationship.GetForeignKey()    == foreignKey;
//         });
// }

// bool ERModel::removeRelationshipByRef(Relationship &rel) {
//     auto it = findRelationshipIter(rel.GetLhsEntityName(), rel.GetRhsEntityName(), rel.GetForeignKey());

//     if (it != relationships.end()) {
//         relationships.erase(it);
//         return true; // Relationship removed successfully
//     } else {
//         throw TError(ErrorsType::NOT_FOUND_RELATIONSHIP_E);
//         return false; // Relationship not found
//     }
// }

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
