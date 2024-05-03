#include "base/ScriptGenerator.h"

using namespace erconv;

std::string ScriptGenerator::getEntityFieldDataType(const DataTypeEntity& dataType) {
    switch (dataType) {
        case DataTypeEntity::SMALL_INT_T:
            return "SMALLINT";
        case DataTypeEntity::INT_T:
            return "INT";
        case DataTypeEntity::BIG_INT_T:
            return "BIGINT";
        case DataTypeEntity::NUMERIC_DEC_T:
            return "NUMERIC";
        case DataTypeEntity::REAL_T:
            return "REAL";
        case DataTypeEntity::DOUBLE_PRECIS_T:
            return "DOUBLE PRECISION";
        case DataTypeEntity::FLOAT_T:
            return "FLOAT";
        case DataTypeEntity::CHAR_T:
            return "CHAR";
        case DataTypeEntity::VARCHAR_T:
            return "VARCHAR";
        case DataTypeEntity::TEXT_T:
            return "TEXT";
        case DataTypeEntity::DATE_T:
            return "DATE";
        case DataTypeEntity::TIME_T:
            return "TIME";
        case DataTypeEntity::TIMESTAMP_WITH_TZ_T:
            return "TIMESTAMP WITH TIME ZONE";
        case DataTypeEntity::TIMESTAMP_WITHOUT_TZ_T:
            return "TIMESTAMP WITHOUT TIME ZONE";
        default:
            break;
    }

    return "";
}

std::string ScriptGenerator::getEntityFieldCurrentConstraint(const ConstraintsEntity& constraint) {
    switch (constraint) {
        case ConstraintsEntity::NOT_NULL_C:
            return "NOT NULL";
        case ConstraintsEntity::UNIQUE_C:
            return "UNIQUE";
        case ConstraintsEntity::PRIMARY_KEY_C:
            return "PRIMARY KEY";
        case ConstraintsEntity::FOREIGN_KEY_C:
            return "FOREIGN KEY";
        case ConstraintsEntity::DEFAULT_C:
            return "DEFAULT";
        default:
            break;
    }

    return "";
}

/*
    The method returns a 
    vector of requests for 
    creating tables in the database, 
    arranged in the topological order 
    of table construction.
*/

std::vector<std::string> ScriptGenerator::Generate(ERModel& model) {
    std::vector<EntityID> entitiesID = model.GetEntitesInTopologicalOrder();

    std::vector<std::string> finalScript;

    for (int entitiesIter = 0; entitiesIter < entitiesID.size(); ++entitiesIter) {
        const Entity& ent = model.GetEntity(entitiesID[entitiesIter]);
        const std::vector<TEntityField>& currentEntityFields = ent.GetAllFields();
        const std::vector<RelationshipID> currentEntityRelationships = 
            model.GetConnectedRelationshipsInWhichEntityIsAChild(entitiesID[entitiesIter]);

        std::string fieldDataType;
        std::string fieldCurrentConstraint;
        std::string currentEntityScript("CREATE TABLE ");
        currentEntityScript += ent.GetName() + " (\n";

        for (int fieldsIter = 0; fieldsIter < currentEntityFields.size(); ++fieldsIter) {
            currentEntityScript += "\t" + currentEntityFields[fieldsIter].Name + " ";
            fieldDataType = ScriptGenerator::getEntityFieldDataType(currentEntityFields[fieldsIter].DataType);

            if (!fieldDataType.size()) {
                throw TError(ErrorsType::DATA_TYPE_NOT_DECLARED_E);
            }

            currentEntityScript += fieldDataType;

            for (int constraintsIter = 0; constraintsIter < currentEntityFields[fieldsIter].Constraints.size(); ++constraintsIter) {
                fieldCurrentConstraint = ScriptGenerator::getEntityFieldCurrentConstraint(
                        currentEntityFields[fieldsIter].Constraints[constraintsIter]
                    );

                if (!fieldCurrentConstraint.size()) {
                    throw TError("Constraint type is not declared\n");
                }
                
                if (fieldCurrentConstraint == "FOREIGN KEY") {
                    continue;
                }

                currentEntityScript += " " + fieldCurrentConstraint;
            }

            if (fieldsIter < currentEntityFields.size() - 1 || currentEntityRelationships.size()) {
                currentEntityScript += ",";
            }

            currentEntityScript += "\n";
        }

        for (int relationshipIter = 0; relationshipIter < currentEntityRelationships.size(); ++relationshipIter) {
            const Relationship& currentRelationShip = model.GetRelationship(currentEntityRelationships[relationshipIter]);
            currentEntityScript += "\tFOREIGN KEY ("
                + currentRelationShip.GetForeignKey()
                + ") REFERENCES "
                + currentRelationShip.GetLhsEntityName()
                + "("
                + currentRelationShip.GetPrimaryKey()
                + ")";

            if (relationshipIter < currentEntityRelationships.size() - 1) {
                currentEntityScript += ",";
            }

            currentEntityScript += "\n";
        }

        currentEntityScript += ");\n";

        finalScript.push_back(currentEntityScript);
    }

    return finalScript;
}