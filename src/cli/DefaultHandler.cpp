#include "cli/DefaultHandler.h"

namespace erconv {

void DefaultHandler::Execute(const Command& args, std::ostream& out) {
    // Здесь вся обработка запросов.
    // Dummy code:
    // for (const auto& arg : args) {
    //     out << arg << " ";
    // }
    // out << std::endl;
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
    }
}

void DefaultHandler::ModelNewName(const std::string name) {
    if (models.find(name) != models.end()) {
        std::cout << "Model with name " << name << " already exists!" << std::endl;
    } else {
        ERModel model;
        models.insert({name, model});
        current = {name, model};
    }
}

void DefaultHandler::ModelDeleteName(const std::string name) {
    models.erase(name);
    if (current.first == name) {
        current.first = models.begin()->first;
        current.second = models.begin()->second;
    }
}

void DefaultHandler::ModelSelectName(const std::string name) {
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

void DefaultHandler::ModelEntityNewName(const std::string name) {
    if (models.size() != 0) {
        std::cout << "There are no models" << std::endl;
    } else {
        current.second.AddEntity(name);
    }
}

void DefaultHandler::ModelEntityDeleteName(const std::string name) {
    if (models.size() != 0) {
        std::cout << "There are no models" << std::endl;
    } else {
        current.second.RemoveEntity(name);
    }
}

void DefaultHandler::ModelEntityList() {
    if (models.size() != 0) {
        std::cout << "There are no models" << std::endl;
    } else {
        std::cout << "There are list of entities of model " << current.first << std::endl;
        for (const auto ent : current.second.GetEntities()) {
            std::cout << ent.GetName() << std::endl;
        }
    }
}



} // namespace erconv
