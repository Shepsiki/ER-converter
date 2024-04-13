#include "Model.h"

using namespace erconv;

int main() { 
    Entity ent1("Students");
    Entity ent2("Facilities");
    Entity ent3("3");
    Entity ent4("4");

    std::vector<erconv::ConstraintsEntity> constr1 {erconv::ConstraintsEntity::PRIMARY_KEY_C};
    std::vector<erconv::ConstraintsEntity> constr2;
    std::vector<erconv::ConstraintsEntity> constr3 {erconv::ConstraintsEntity::NOT_NULL_C};

    ent1.AddField("Id1", erconv::DataTypeEntity::INT_T, constr1);
    ent1.AddField("Name1", erconv::DataTypeEntity::VARCHAR_T, constr2);
    ent1.AddField("Address1", erconv::DataTypeEntity::TEXT_T, constr3);

    ent2.AddField("Id2", erconv::DataTypeEntity::INT_T, constr1);
    ent2.AddField("Name2", erconv::DataTypeEntity::VARCHAR_T, constr2);
    ent2.AddField("Address2", erconv::DataTypeEntity::TEXT_T, constr3);

    Relationship rel1(ONE_TO_ONE_T, "Works", &ent1, &ent2);
    Relationship rel2(ONE_TO_MANY_T, "Studies", &ent1, &ent2);

    ERModel erModel;

    erModel.AddEntity("Students"); // Add an entity with the same name
    erModel.AddEntity("Facilities");
    erModel.AddEntity("3");
    erModel.AddEntity("4");

    erModel.FindEntity("4");

    std::cout << rel1 << std::endl << rel2 << std::endl;

    // ПОЧИСТИТЕ, ЕСЛИ НАДО

    return 0;
}
