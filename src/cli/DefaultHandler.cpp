#include "cli/DefaultHandler.h"

namespace erconv {

void DefaultHandler::Execute(const Command& args, std::ostream& out) {
    // Здесь вся обработка запросов.
    // Dummy code:
    // for (const auto& arg : args) {
    //     out << arg << " ";
    // }
    // out << std::endl;
    

    // MODEL COMMANDS BEGIN // 
    if (args[0] == "model") {

        if (args[1] == "new") {
            ModelNewName(args[2]);
        }

        if (args[1] == "delete") {
            ModelDeleteName(args[2]);
        }

        if (args[1] == "select") {
            ModelSelectName(args[2]);
        }

        if (args[1] == "selected") {
            ModelSelected();
        }

        if (args[1] == "list") {
            ModelList();
        }

        if (args[1] == "clear") {
            ModelClear();
        }
    }
    // MODEL COMMANDS END //

    // ENTITY COMMANDS BEGIN //
    if (args[0] == "entity") {
        if (args[1] == "new") {
            ModelEntityNewName(args[2]);
        }

        if (args[1] == "delete") {
            ModelEntityDeleteName(args[2]);
        }

        if (args[1] == "list") {
            ModelEntityList();
        }

        if (args[1] == "attribute") {
            if (args[2] == "add") {
                DataTypeEntity enumType = DataTypeEntity(FromStringToDataType(args[5]));
                std::vector<ConstraintsEntity> fieldConstr;
                for (int i = 6; i < args.size(); i++) {
                    ConstraintsEntity fieldType = ConstraintsEntity(FromStringToCovType(args[i]));
                    fieldConstr.push_back(fieldType);
                }
                ModelEntityAttributeAdd(args[3], args[4], enumType, fieldConstr);
            }

            if (args[2] == "delete") {
                ModelEntityAttributeDelete(args[3], args[4]);
            }

            if (args[2] == "list") {
               ModelEntityAttributeList(args[3]);
            }

            if (args[2] == "clear") {
                void ModelEntityAttributeClear();
            }
        
        }
    }
    // ENTITY COMMANDS END //

    // RELATIONSHIP COMMANDS BEGIN //

    if (args[0] == "relationship") {

        if (args[1] == "list") {
            ModelRelationList();
        }

        if (args[1] == "add") {
            ModelRelationAdd(args[3], args[4], args[2], args[5], args[6]); //тезисно

        }

        if (args[2] == "clear") {
            ModelRelationClear();
        }

        if (args[2] == "delete") {
            ModelRelationDelete(args[3], args[4], args[5]);
        }
    }

    // RELATIONSHIP COMMANDS END //

    // CONNNFIG COMMANDS BEGIN //

    if (args[0] == "config") {

        if (args[1] == "types") {
            ModelConfigTypes();
        }

        if (args[1] == "constraints") {
            ModelConfigConstraints();
        }

    }

    // CONFIG COMMANDS END //

    // HELP COMMANDS BEGIN //

    if (args[0] == "help") {

        if (args.size() != 1) {

            if (args[1] == "model") {
                ModelHelpModel();
            }

            if (args[1] == "entity") {
                ModelHelpEntity();
            }

            if(args[1] == "relationship") {
                ModelHelpRelationship();
            }
            
        } else {
            ModelHelp();
        }
    }

    // HELP COMMANDS END //
}

int DefaultHandler::FromStringToDataType(const std::string& str) {
    if (str == "SMALL_INT_T")
        return 1;
    if (str == "INT_T")
        return 2;
    if (str == "BIG_INT_T")
        return 3;
    if (str == "NUMERIC_DEC_T")
        return 4;
    if (str == "REAL_T")
        return 5;
    if (str == "DOUBLE_PRECIS_T")
        return 6;
    if (str == "FLOAT_T")
        return 7;
    if (str == "CHAR_T")
        return 8;
    if (str == "VARCHAR_T")
        return 9;
    if (str == "TEXT_T")
        return 10;
    if (str == "DATE_T")
        return 11;
    if (str == "TIME_T")
        return 12;
    if (str == "TIMESTAMP_WITH_TZ_T")
        return 13;
    if (str == "TIMESTAMP_WITHOUT_TZ_T")
        return 14;
    return 0;
}

int DefaultHandler::FromStringToCovType(const std::string str) {
    if (str == "NOT_NULL_C")
        return 1;
    if (str == "NULL_C")
        return 2;
    if (str == "UNIQUE_C")
        return 3;
    if (str == "PRIMARY_KEY_C")
        return 4;
    if (str == "FOREIGN_KEY_C")
        return 5;
    if (str == "DEFAULT_C")
        return 6;
    return 0;
}

int DefaultHandler::FromStringToRelationType(const std::string str) {
    if (str == "ONE_TO_ONE_T")
        return 1;
    if (str == "ONE_TO_MANY_T")
        return 2;
    if (str == "MANY_TO_MANY_T")
        return 3;
    return 0;
}

void DefaultHandler::ModelNewName(const std::string& name) {
    if (models.find(name) != models.end()) {
        std::cout << "Model with name " << name << " already exists!" << std::endl;
    } else {
        ERModel model;
        models.insert({name, model});
        current = {name, model};
    }
}

void DefaultHandler::ModelDeleteName(const std::string& name) {
    models.erase(name);
    if (current.first == name) {
        current.first = models.begin()->first;
        current.second = models.begin()->second;
    }
}

void DefaultHandler::ModelSelectName(const std::string& name) {
    if (models.find(name) == models.end()) {
        std::cout << "Model with name " << name << " not in list" << std::endl;
    } else {
        current.first = name;
        current.second = models[name];
    }
}

void DefaultHandler::ModelSelected() {
    if (models.size() != 0) {
        std::cout << current.first << std::endl;
    } else {
        std::cout << "There are no models" << std::endl;
    }
}

void DefaultHandler::ModelList() {
    if (models.size() != 0) {
        std::cout << "There are models: " << std::endl;
        for (const auto& mod : models) {
            std::cout << mod.first << std::endl;
        }
    } else {
        std::cout << "There are no models" << std::endl;
    }
}

void DefaultHandler::ModelClear() {
    models.clear();
    std::cout << "All models are deleted" << std::endl;
}

void DefaultHandler::ModelEntityNewName(const std::string& name) {
    if (models.size() != 0) {
        current.second.AddEntity(name);
    } else {
        std::cout << "There are no models" << std::endl;
    }
}

void DefaultHandler::ModelEntityDeleteName(const std::string& name) {
    if (models.size() != 0) {
        current.second.RemoveEntity(name);
    } else {
        std::cout << "There are no models" << std::endl;
    }
}

void DefaultHandler::ModelEntityList() {
    if (models.size() != 0) {
        std::cout << "There are list of entities of model " << current.first << std::endl;
        for (const auto ent : current.second.GetEntities()) {
            std::cout << ent.GetName() << std::endl;
        }
    } else {
        std::cout << "There are no models" << std::endl;
    }
}

void DefaultHandler::ModelEntityAttributeAdd(const std::string&   entityName, 
                                            const std::string&   fieldName, 
                                            const DataTypeEntity &fieldType, 
                                            const std::vector<ConstraintsEntity> &fieldConstr) {
    if (models.size() != 0)
        current.second.AddEntityField(entityName, fieldName, fieldType, fieldConstr);
    else
        std::cout << "There are no models" << std::endl;
}

void DefaultHandler::ModelEntityAttributeDelete(const std::string& name, const std::string& field) {
    if (models.size() != 0)
        current.second.RemoveEntityField(name, field);
    else
        std::cout << "There are no models" << std::endl;
}

void DefaultHandler::ModelEntityAttributeList(const std::string& name) {
    if (models.size() != 0) {
        Entity ent = current.second.GetEntity(name);
        const std::vector<TEntityField> vec = ent.GetAllFields();
        if (vec.size() != 0)
            std::cout << "Attributes of entity " << name << ":\n";
        for (int i = 0; i < vec.size(); i++) {
            std::cout << vec[i].Name << std::endl;
        }
    }
    else
        std::cout << "There are no models" << std::endl;
}

void DefaultHandler::ModelEntityAttributeClear(const std::string& name) {
    if (models.size() != 0) {
        Entity ent = current.second.GetEntity(name);
        ent.DeleteAllFields();
    } else {
        std::cout << "There are no models" << std::endl;
    }
}

void DefaultHandler::ModelRelationList() {
    if (models.size() != 0) {
        std::vector<Relationship> relations = current.second.GetRelationships();
        for (int i = 0; i < relations.size(); i++) {
            std::cout << relations[i].GetName();
        }
    } else {
        std::cout << "There are no models" << std::endl;
    }
}

void DefaultHandler::ModelRelationAdd(std::string strtype, //тезисно
                             const std::string &lhsEntityName, 
                             const std::string &rhsEntityName,
                             const std::string &foreignKey,
                             const std::string _name) {

    // Entity entl = current.second.GetEntity(lhsEntityName);
    // Entity entr = current.second.GetEntity(rhsEntityName);
    TypeRelationship type = TypeRelationship(FromStringToRelationType(strtype));
    if (models.size() != 0) 
        current.second.AddRelationship(type,lhsEntityName, rhsEntityName, foreignKey, _name);
    else 
        std::cout << "There are no models" << std::endl;
}

void DefaultHandler::ModelRelationClear() {
    if (models.size() != 0)
        current.second.RemoveAllRelationships();
    else
        std::cout << "There are no models" << std::endl;
}

void DefaultHandler::ModelRelationDelete(const std::string &lhsEntityName, 
                                        const std::string &rhsEntityName,
                                        const std::string &foreignKey) {
    if (models.size() != 0)
        current.second.RemoveRelationship(lhsEntityName, rhsEntityName, foreignKey);
    else
        std::cout << "There are no models" << std::endl;
    

}

void DefaultHandler::ModelConfigTypes() {
    std::cout << "There are types: " << std::endl;
    std::cout << "UNDEFINED" << std::endl;
    std::cout << "SMALL_INT_T" << std::endl;
    std::cout << "INT_T" << std::endl;
    std::cout << "BIG_INT_T" << std::endl;
    std::cout << "NUMERIC_DEC_T" << std::endl;
    std::cout << "REAL_T" << std::endl;
    std::cout << "DOUBLE_PRECIS_T" << std::endl;
    std::cout << "FLOAT_T" << std::endl;
    std::cout << "CHAR_T" << std::endl;
    std::cout << "VARCHAR_T" << std::endl;
    std::cout << "DATE_T" << std::endl;
    std::cout << "TIME_T" << std::endl;
    std::cout << "TIMESTAMP_WITH_TZ_T" << std::endl;
    std::cout << "TIMESTAMP_WITHOUT_TZ_T" << std::endl;
}

void DefaultHandler::ModelConfigConstraints() {
    std::cout << "There are constraints: " << std::endl;
    std::cout << "UNDEFINED" << std::endl;
    std::cout << "NOT_NULL_C" << std::endl;
    std::cout << "NOT_NULL_C" << std::endl;
    std::cout << "NULL_C" << std::endl;
    std::cout << "UNIQUE_C" << std::endl;
    std::cout << "PRIMARY_KEY_C" << std::endl;
    std::cout << "FOREIGN_KEY_C" << std::endl;
    std::cout << "DEFAULT_C" << std::endl;
}

void DefaultHandler::ModelHelp() {
    std::cout << "You can use commands: \n" << std::endl;
    std::cout << "model ..." << std::endl;
    std::cout << "entity ..." << std::endl;
    std::cout << "relationship ...\n" << std::endl;

    std::cout << "Enter `help ...` and one of them to know more" << std::endl;

}

void DefaultHandler::ModelHelpModel() {
    std::cout << "You can use commands: \n" << std::endl;
    std::cout << "model new <name> - to make a new model with name <name>" << std::endl;
    std::cout << "model delete <name> - to delete a model with name <name>" << std::endl;
    std::cout << "model select <name> - to select a model with name <name>" << std::endl;
    std::cout << "(default: if you didn't select a model, the last added is selected)" << std::endl;
    std::cout << "model selected - what is the model selected" << std::endl; 
    std::cout << "model list - just print names of all added models" << std::endl;
    std::cout << "model clear - just delete all models" << std::endl;
}

void DefaultHandler::ModelHelpEntity() {
    std::cout << "You can use commands: \n" << std::endl;
    std::cout << "entity new <name> - to make a new entity with name <name>" << std::endl;
    std::cout << "entity delete <name> - to delete an entity with name <name>" << std::endl;
    std::cout << "entity attrinute add <entity_name> <attribute_name> <attribute_type> [constraints] - to add a new attribute" << std::endl;
    std::cout << "entity attribute delete <entity_name> <attribute_name> - just delete attribute" << std::endl; 
    std::cout << "entity attribute list <entity_name> - just print names of all attributes of this entity" << std::endl;
    std::cout << "entity attribute clear - just delete all attributes of entity" << std::endl;
}

void DefaultHandler::ModelHelpRelationship() {
    std::cout << "You can use commands: \n" << std::endl;
    std::cout << "entity new <name> - to make a new entity with name <name>" << std::endl;
    std::cout << "entity delete <name> - to delete an entity with name <name>" << std::endl;
    std::cout << "entity attrinute add <entity1_name> <entity2_name> <attribute2_name> <type> - to add a new relationship" << std::endl;
    std::cout << "relationship delete <entity1_name> <entity2_name> <attribute2_name> - just delete relationship" << std::endl; 
    std::cout << "relationship list - just print names of all relationshups" << std::endl;
    std::cout << "relationship clear - just delete all relationships" << std::endl;
}





} // namespace erconv
 