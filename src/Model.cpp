#include "Model.h"

using namespace erconv;

// СОЗДАНИЕ/УДАЛЕНИЕ:

bool ERModel::AddEntity(const std::string name) {
    // Check if an entity with the same name already exists
    if (FindEntity(name) != nullptr) {
        throw TError(ErrorsType::EXISTING_ENTITY_NAME_E);
        return false;
    }
    if (!isValidName(name)) {
        throw TError(ErrorsType::INVALID_ENTITY_NAME_E);
        return false;
    }

    // Create and add the new entity
    Entity newEntity(name);
    entities.push_back(newEntity);
    return true;
}

bool ERModel::AddEntity(Entity& entity) {
    // Check if an entity with the same name already exists
    if (FindEntity(entity.GetName()) != nullptr) {
        throw TError(ErrorsType::EXISTING_ENTITY_NAME_E);
        return false;
    }
    if (!isValidName(entity.GetName())) {
        throw TError(ErrorsType::INVALID_ENTITY_NAME_E);
        return false;
    }
    entities.push_back(entity);
    return true;
}

bool ERModel::AddEntityField(const std::string entityName, 
                    const std::string fieldName, 
                    const DataTypeEntity & fieldType, 
                    const std::vector<ConstraintsEntity> & fieldConstr) 
    {
    auto entIter = findEntityIter(entityName);
    if (entIter != entities.end()) {
        Entity& ent = *entIter;
        return ent.AddField(fieldName, fieldType, fieldConstr);
    } else {
        throw TError(ErrorsType::NOT_FOUND_ENTITY_FIELD_NAME_E);
        return false;
    }
}

bool ERModel::AddRelationship(TypeRelationship type, const std::string name, Entity &lhsEntity, Entity &rhsEntity) {
    // Check if a relationship with the same name already exists
    if (FindRelationship(name) != nullptr) {
        throw TError(ErrorsType::EXISTING_RELATIONSHIP_NAME_E);
        return false;
    }
    if (!isValidName(name)) {
        throw TError(ErrorsType::INVALID_RELATIONSHIP_NAME_E);
        return false;
    }

    // Create and add the new relationship
    Relationship newRelationship(type, name, &lhsEntity, &rhsEntity);
    relationships.push_back(newRelationship);
    return true;
}

bool ERModel::AddRelationship(Relationship& rel) {
    // Check if a relationship with the same name already exists
    if (FindRelationship(rel.GetName()) != nullptr) {
        throw TError(ErrorsType::EXISTING_RELATIONSHIP_NAME_E);
        return false;
    }
    if (!isValidName(rel.GetName())) {
        throw TError(ErrorsType::INVALID_RELATIONSHIP_NAME_E);
        return false;
    }

    relationships.push_back(rel);
    return true;
}

bool ERModel::RemoveEntity(const std::string& name) {
    auto it = findEntityIter(name);

    if (it != entities.end()) {
        entities.erase(it);
        for (const auto &rel : GetConnectedRelationships(*it)) {
            RemoveRelationship(rel.GetName()); // Удаляем все связанные отношения
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

bool ERModel::RemoveRelationship(const std::string& name) {
    auto it = findRelationshipIter(name);

    if (it != relationships.end()) {
        relationships.erase(it);
        return true; // Relationship removed successfully
    } else {
        throw TError(ErrorsType::NOT_FOUND_RELATIONSHIP_NAME_E);
        return false; // Relationship not found
    }
}


// ПОИСК:

const Entity* ERModel::FindEntity(const std::string& name) {
    auto it = findEntityIter(name);
    if (it != entities.end()) {
        return std::addressof(*it);
    } else {
        return nullptr;
    }
}

const Relationship* ERModel::FindRelationship(const std::string& name) {
    auto it = findRelationshipIter(name);
    if (it != relationships.end()) {
        return std::addressof(*it);
    } else {
        return nullptr;
    }
}


// ГЕТТЕРЫ:

const Entity& ERModel::GetEntity(const std::string& name) {
    const Entity* ent = FindEntity(name);
    if (ent != nullptr) {
        return *ent;
    }

    throw TError(ErrorsType::NOT_FOUND_ENTITY_NAME_E); //
}

const Relationship& ERModel::GetRelationship(const std::string& name) {
    const Relationship* rel = FindRelationship(name);
    if (rel != nullptr) {
        return *rel;
    }

    throw TError(ErrorsType::NOT_FOUND_RELATIONSHIP_NAME_E); //
}

const std::vector<Entity>& ERModel::GetEntities() const {
    return entities;
}

const std::vector<Relationship>& ERModel::GetRelationships() const {
    return relationships;
}

const std::vector<Relationship> ERModel::GetConnectedRelationships(const Entity& entity) const {
    std::vector<Relationship> connectedRelationships;

    for (const auto& relationship : relationships) {
        if (entity == *relationship.GetLhsEntity() || entity == *relationship.GetRhsEntity()) {
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


// PRIVATE SCOPE:

std::vector<Entity>::iterator ERModel::findEntityIter(const std::string& name) {
    return std::find_if(entities.begin(), entities.end(), [name](const Entity& entity) {
        return entity.GetName() == name;
    });
}

std::vector<Relationship>::iterator ERModel::findRelationshipIter(const std::string& name) {
    return std::find_if(relationships.begin(), relationships.end(), [name](const Relationship& relationship) {
        return relationship.GetName() == name;
    });
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