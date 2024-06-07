#pragma once

#include "cli/Handler.h"
#include "base/Model.h"
#include "base/Entity.h"
#include "base/Relationship.h"
#include "base/ScriptGenerator.h"
#include <unordered_map>

namespace erconv {

class DefaultHandler : public IHandler {
public:
    DefaultHandler() : models(), current() {};
    void Execute(const Command& args, std::ostream& out) override;

private:
    std::unordered_map<std::string, ERModel> models;
    std::pair<std::string, ERModel> current;
    //std::unordered_map<std::string, ERModel>::iterator current;

private:
    int FromStringToDataType(const std::string& str);
    int FromStringToCovType(const std::string str);
    int FromStringToRelationType(const std::string str);

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

    void ModelRelationList();
    void ModelRelationAdd(std::string strtype, 
                            const std::string &lhsEntityName, 
                             const std::string &rhsEntityName,
                             const std::string &foreignKey);
    void ModelRelationClear();
    void ModelRelationDelete(const std::string &lhsEntityName, 
                            const std::string &rhsEntityName,
                            const std::string &foreignKey);
    
    void ModelGenerator();

    void ModelConfigTypes();
    void ModelConfigConstraints();

    void ModelHelp();
    void ModelHelpModel();
    void ModelHelpEntity();
    void ModelHelpRelationship();
    void ModelHelpConfig();


};

} // namespace erconv
