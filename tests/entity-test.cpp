#include <gtest/gtest.h>

#include "Entity.h"

TEST(TestMethodAddField, BasedTest) {
    erconv::Entity Test("Test");
    
    std::vector<erconv::ConstraintsEntity> constr1 {
        erconv::ConstraintsEntity::PRIMARY_KEY_C
    };
    Test.AddField("Id", erconv::DataTypeEntity::INT_T, constr1);

    std::vector<erconv::ConstraintsEntity> constr2;
    Test.AddField("Name", erconv::DataTypeEntity::VARCHAR_T, constr2);

    std::vector<erconv::ConstraintsEntity> constr3 {
        erconv::ConstraintsEntity::NOT_NULL_C
    };
    ASSERT_TRUE(
        Test.AddField("Address", erconv::DataTypeEntity::TEXT_T, constr3)
    );
}

TEST(TestMethodAddField, TestExceptionToRepeatName) {
    erconv::Entity Test("Test");
    
    std::vector<erconv::ConstraintsEntity> constr1 {
        erconv::ConstraintsEntity::PRIMARY_KEY_C
    };
    Test.AddField("Id", erconv::DataTypeEntity::INT_T, constr1);

    std::vector<erconv::ConstraintsEntity> constr2;
    Test.AddField("Name", erconv::DataTypeEntity::VARCHAR_T, constr2);
    
    EXPECT_THROW (
        Test.AddField("Id", erconv::DataTypeEntity::INT_T, constr1),
        erconv::TError
    );
}

TEST(TestMethodAddField, TestExceptionToIvalidLetterInName) {
    erconv::Entity Test("Test");
    
    std::vector<erconv::ConstraintsEntity> constr1 {
        erconv::ConstraintsEntity::PRIMARY_KEY_C
    };
    Test.AddField("Id", erconv::DataTypeEntity::INT_T, constr1);

    std::vector<erconv::ConstraintsEntity> constr2;
    Test.AddField("Name", erconv::DataTypeEntity::VARCHAR_T, constr2);
    
    ASSERT_THROW(
        Test.AddField("@dog:D", erconv::DataTypeEntity::INT_T, constr2),
        erconv::TError
    );
}

TEST(TestMethodAddField, TestExceptionToInvalidDataType) {
    erconv::Entity Test("Test");
    
    std::vector<erconv::ConstraintsEntity> constr1 {
        erconv::ConstraintsEntity::UNDEFINED
    };
    
    ASSERT_THROW(
        Test.AddField("Id", erconv::DataTypeEntity::INT_T, constr1),
        erconv::TError
    );
}

TEST(TestMethodAddField, TestExceptionToCheckNullRepeat) {
    erconv::Entity Test("Test");
    
    std::vector<erconv::ConstraintsEntity> constr1 {
        erconv::ConstraintsEntity::NULL_C,
        erconv::ConstraintsEntity::NOT_NULL_C
    };
    
    ASSERT_THROW(
        Test.AddField("Id", erconv::DataTypeEntity::INT_T, constr1),
        erconv::TError
    );
}

TEST(TestMethodAddField, TestExceptionToCheckNotNullRepeat) {
    erconv::Entity Test("Test");
    
    std::vector<erconv::ConstraintsEntity> constr1 {
        erconv::ConstraintsEntity::NOT_NULL_C,
        erconv::ConstraintsEntity::NULL_C
    };
    
    ASSERT_THROW(
        Test.AddField("Id", erconv::DataTypeEntity::INT_T, constr1),
        erconv::TError
    );
}

TEST(TestMethodAddField, TestExceptionToRepeatDeclaratePrimaryKey) {
    erconv::Entity Test("Test");
    
    std::vector<erconv::ConstraintsEntity> constr1 {
        erconv::ConstraintsEntity::PRIMARY_KEY_C,
    };
    Test.AddField("Id", erconv::DataTypeEntity::INT_T, constr1);
    
    ASSERT_THROW(
        Test.AddField("Id_copy", erconv::DataTypeEntity::INT_T, constr1),
        erconv::TError
    );
}

TEST(TestMethodDeleteField, BasedTest) {
    erconv::Entity Test("Test");
    
    std::vector<erconv::ConstraintsEntity> constr1 {
        erconv::ConstraintsEntity::PRIMARY_KEY_C
    };
    Test.AddField("Id", erconv::DataTypeEntity::INT_T, constr1);

    std::vector<erconv::ConstraintsEntity> constr2;
    Test.AddField("Name", erconv::DataTypeEntity::VARCHAR_T, constr2);

    std::vector<erconv::ConstraintsEntity> constr3 {
        erconv::ConstraintsEntity::NOT_NULL_C
    };
    Test.AddField("Address", erconv::DataTypeEntity::TEXT_T, constr3);

    ASSERT_TRUE(Test.DeleteField("Address"));
}

TEST(TestMethodDeleteField, TestExceptionToNotFoundField) {
    erconv::Entity Test("Test");
    
    std::vector<erconv::ConstraintsEntity> constr1 {
        erconv::ConstraintsEntity::PRIMARY_KEY_C
    };
    Test.AddField("Id", erconv::DataTypeEntity::INT_T, constr1);

    std::vector<erconv::ConstraintsEntity> constr2;
    Test.AddField("Name", erconv::DataTypeEntity::VARCHAR_T, constr2);

    std::vector<erconv::ConstraintsEntity> constr3 {
        erconv::ConstraintsEntity::NOT_NULL_C
    };
    Test.AddField("Address", erconv::DataTypeEntity::TEXT_T, constr3);

    ASSERT_FALSE(Test.DeleteField("SecondName"));
}

TEST(TestMethodGetName, DamnIlyaNameTheseTestsProperly1) {
    erconv::Entity Test("Test");
    ASSERT_TRUE(Test.GetName() == "Test");
}

TEST(TestMethodOperatorEquality, DamnIlyaNameTheseTestsProperly2) {
        // Create two entities with the same name
    erconv::Entity entity1("TestEntity");
    erconv::Entity entity2("TestEntity");

    // Check if the operator correctly identifies them as equal
    ASSERT_TRUE(entity1 == entity2);
}

TEST(TestMethodOperatorEquality, DamnIlyaNameTheseTestsProperly3) {
        // Create two entities with the same name
    erconv::Entity entity1("TestEntity1");
    erconv::Entity entity2("TestEntity2");

    // Check if the operator correctly identifies them as equal
    ASSERT_FALSE(entity1 == entity2);
}