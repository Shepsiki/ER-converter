#include "base/Relationship.h"

using namespace erconv;
// СЕТТЕРЫ

void Relationship::SetPrimaryKey(const std::string &keyName) { primaryKeyName = keyName; }

void Relationship::SetForeignKey(const std::string &keyName) { foreignKeyName = keyName; }

// ГЕТТЕРЫ

const TypeRelationship Relationship::GetType() const { return type; }

const std::string& Relationship::GetLhsEntityName() const { return lhsEntityName; }

const std::string& Relationship::GetRhsEntityName() const { return rhsEntityName; }

const std::string& Relationship::GetPrimaryKey() const { return primaryKeyName; }

const std::string& Relationship::GetForeignKey() const { return foreignKeyName; }

const std::string& Relationship::GetName() const { return name; }

const ERModel* Relationship::GetModelPtr() const { return parentModel; }
