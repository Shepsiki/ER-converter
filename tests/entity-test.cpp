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
    
    ASSERT_FALSE (
        Test.AddField("Id", erconv::DataTypeEntity::INT_T, constr1)
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
    
    ASSERT_FALSE(
        Test.AddField("@dog:D", erconv::DataTypeEntity::INT_T, constr2)
    );
}

TEST(TestMethodAddField, TestExceptionToInvalidDataType) {
    erconv::Entity Test("Test");
    
    std::vector<erconv::ConstraintsEntity> constr1 {
        erconv::ConstraintsEntity::UNDEFINED
    };
    
    ASSERT_FALSE(
        Test.AddField("Id", erconv::DataTypeEntity::INT_T, constr1)
    );
}

TEST(TestMethodAddField, TestExceptionToCheckNullRepeat) {
    erconv::Entity Test("Test");
    
    std::vector<erconv::ConstraintsEntity> constr1 {
        erconv::ConstraintsEntity::NULL_C,
        erconv::ConstraintsEntity::NOT_NULL_C
    };
    
    ASSERT_FALSE(
        Test.AddField("Id", erconv::DataTypeEntity::INT_T, constr1)
    );
}

TEST(TestMethodAddField, TestExceptionToCheckNotNullRepeat) {
    erconv::Entity Test("Test");
    
    std::vector<erconv::ConstraintsEntity> constr1 {
        erconv::ConstraintsEntity::NOT_NULL_C,
        erconv::ConstraintsEntity::NULL_C
    };
    
    ASSERT_FALSE(
        Test.AddField("Id", erconv::DataTypeEntity::INT_T, constr1)
    );
}

TEST(TestMethodAddField, TestExceptionToRepeatDeclaratePrimaryKey) {
    erconv::Entity Test("Test");
    
    std::vector<erconv::ConstraintsEntity> constr1 {
        erconv::ConstraintsEntity::PRIMARY_KEY_C,
    };
    Test.AddField("Id", erconv::DataTypeEntity::INT_T, constr1);
    
    ASSERT_FALSE(
        Test.AddField("Id_copy", erconv::DataTypeEntity::INT_T, constr1)
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