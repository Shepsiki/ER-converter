#include <gtest/gtest.h>

#include "base/ScriptGenerator.h"

namespace erconv {
    using EntityID = ScriptGenerator::EntityID;
    using RelationshipID = ScriptGenerator::RelationshipID;

    class ScriptGeneratorTest : public ::testing::Test {
        protected:
            Entity ent1;
            Entity ent2;
            Entity ent3;
            Entity ent4;
            Entity ent5;
            Entity ent6;
            ERModel erModel;

            void SetUp() override {
                ent1 = Entity("users");
                ent2 = Entity("orders");
                ent3 = Entity("Courses");
                ent4 = Entity("Students");
                ent5 = Entity("Instructors");
                ent6 = Entity("Grades");

                std::vector<erconv::ConstraintsEntity> constr1 {
                    erconv::ConstraintsEntity::PRIMARY_KEY_C
                };

                std::vector<erconv::ConstraintsEntity> constr2;

                std::vector<erconv::ConstraintsEntity> constr3{
                    erconv::ConstraintsEntity::FOREIGN_KEY_C
                };

                std::vector<erconv::ConstraintsEntity> constr4{
                    erconv::ConstraintsEntity::NOT_NULL_C,
                    erconv::ConstraintsEntity::UNIQUE_C
                };

                std::vector<erconv::ConstraintsEntity> constr5 {
                    erconv::ConstraintsEntity::DEFAULT_C,
                    erconv::ConstraintsEntity::FOREIGN_KEY_C
                };

                ent1.AddField("user_id", erconv::DataTypeEntity::INT_T, constr1);
                ent1.AddField("username", erconv::DataTypeEntity::VARCHAR_T, constr2);
                
                ent2.AddField("order_id", erconv::DataTypeEntity::INT_T, constr3);
                ent2.AddField("order_name", erconv::DataTypeEntity::VARCHAR_T, constr2);

                ent3.AddField("course_id", erconv::DataTypeEntity::INT_T, constr1);
                ent3.AddField("course_name", erconv::DataTypeEntity::VARCHAR_T, constr4);
                ent3.AddField("instructor_id", erconv::DataTypeEntity::INT_T, constr3);

                ent4.AddField("student_id", erconv::DataTypeEntity::INT_T, constr1);
                ent4.AddField("student_name", erconv::DataTypeEntity::VARCHAR_T, constr2);
                ent4.AddField("course_id", erconv::DataTypeEntity::INT_T, constr3);

                ent5.AddField("instructor_id", erconv::DataTypeEntity::INT_T, constr1);
                ent5.AddField("instructor_name", erconv::DataTypeEntity::VARCHAR_T, constr2);

                ent6.AddField("grade_id", erconv::DataTypeEntity::INT_T, constr1);
                ent6.AddField("student_id", erconv::DataTypeEntity::INT_T, constr2);
                ent6.AddField("course_id", erconv::DataTypeEntity::INT_T, constr2);
                ent6.AddField("grade", erconv::DataTypeEntity::VARCHAR_T, constr2);
            }

            void TearDown() override {

            }
    };

    TEST_F(ScriptGeneratorTest, GenerateMethodTest_1) {
        erModel.AddEntity(ent1);
        erModel.AddEntity(ent2);
        erModel.AddRelationship(TypeRelationship::ONE_TO_MANY_T, "users", "orders", "order_id");

        EXPECT_NO_THROW(
            std::vector<std::string> finalScript = ScriptGenerator::Generate(erModel);

            for (int i = 0; i < finalScript.size(); ++i) {
                std::cout << finalScript[i];
            }
        );
    }

    TEST_F(ScriptGeneratorTest, GenerateMethodTest_2) {
        erModel.AddEntity(ent3);
        erModel.AddEntity(ent4);
        erModel.AddEntity(ent5);
        erModel.AddEntity(ent6);

        erModel.AddRelationship(erconv::TypeRelationship::ONE_TO_ONE_T, "Instructors", "Courses", "instructor_id", "first");
        erModel.AddRelationship(erconv::TypeRelationship::MANY_TO_MANY_T, "Courses", "Students", "course_id", "second");
        erModel.AddRelationship(erconv::TypeRelationship::MANY_TO_MANY_T, "Students", "Grades", "student_id", "third");
        erModel.AddRelationship(erconv::TypeRelationship::ONE_TO_MANY_T, "Courses", "Grades", "course_id", "fourth");

        EXPECT_NO_THROW(
            std::vector<std::string> finalScript = ScriptGenerator::Generate(erModel);
            
            for (int i = 0; i < finalScript.size(); ++i) {
                std::cout << finalScript[i];
            }
        );
    }
};