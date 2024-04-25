#pragma once

#include "cli/Handler.h"
#include "base/Model.h"
#include "base/Entity.h"
#include <unordered_map>

namespace erconv {

class DefaultHandler : public IHandler {
public:
    DefaultHandler() : models(), current() {};
    void Execute(const Command& args, std::ostream& out) override;

private:
    std::unordered_map<std::string, ERModel> models;
    std::pair<std::string, ERModel> current;

private:
    int FromStringToDataType(const std::string& str);
    int FromStringToCovType(const std::string str);


    void ModelNewName(const std::string& name);
    void ModelDeleteName(const std::string& name);
    void ModelSelectName(const std::string& name);
    void ModelSelected();
    void ModelList();
    void ModelClear();

    void ModelEntityNewName(const std::string& name);
    void ModelEntityDeleteName(const std::string& name);
    void ModelEntityList();
    void ModelEntityAttributeAdd(const std::string&   entityName, 
                                const std::string&   fieldName, 
                                const DataTypeEntity &fieldType, 
                                const std::vector<ConstraintsEntity> &fieldConstr);
    void ModelEntityAttributeDelete(const std::string& name, const std::string& field);
    void ModelEntityAttributeList(const std::string& name);
    void ModelEntityAttributeClear(const std::string& name);

};

} // namespace erconv
