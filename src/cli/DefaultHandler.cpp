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
    // ENTITY COMMANDS END //
    }
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
        // Entity ent = current.second.GetEntity(name);
        // Не хватает метода Clear или DeleteAllFields для класса Entity
        // Есть только удаление по имени сущности DeleteField(); 
    } else {
        std::cout << "There are no models" << std::endl;
    }
}


} // namespace erconv
