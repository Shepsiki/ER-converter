#include "Relationship.h"

using namespace erconv;

std::string      Relationship::GetName()      const { return relationName; }

TypeRelationship Relationship::GetType()      const { return type; }

const Entity*    Relationship::GetLhsEntity() const { return lhsEntity; }

const Entity*    Relationship::GetRhsEntity() const { return rhsEntity; }

// ПЕРЕВОД ТИПА ОТНОШЕНИЯ в СТРОКУ
std::string TypeRelationship2Str(TypeRelationship type) {
    switch (type) {
        case ONE_TO_ONE_T:             return "one-to-one";
        case ONE_TO_MANY_T:            return "one-to-many";
        case MANY_TO_MANY_T:           return "many-to-many";
        case UNDEFINED_RELATIONSHIP_T: 
        default:                       return "undefined-relationship";
    }
}

// ВЫВОД В ПОТОК
std::ostream& erconv::operator<<(std::ostream& os, const Relationship& rel) {
    std::string relType = TypeRelationship2Str( rel.GetType() );
    std::string relName = rel.GetName();
    const Entity* relLhsEntity = rel.GetLhsEntity();
    const Entity* relRhsEntity = rel.GetRhsEntity();
    // Можете менять формат вывода на своё усмотрение
    os << "{ Type: " << relType << ", Name: " << relName  
        <<", Left Entity: " << relLhsEntity->GetName() 
        << ", Right Entity: " << relRhsEntity->GetName() << " }";
        return os;
}