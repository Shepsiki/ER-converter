#include <gtest/gtest.h>

#include "Model.h"

using namespace erconv;

class RelationshipTest : public ::testing::Test {
protected:
    // Declare variables here if you need them for multiple tests
    Entity ent1;
    Entity ent2;
    Entity ent3;
    Relationship rel1;
    Relationship rel2;

    void SetUp() override {
        // Code here will be called immediately after the constructor (before each test)
        ent1 = Entity("Students");
        ent2 = Entity("Facilities");
        ent3 = Entity("People");
        rel1 = Relationship(ONE_TO_ONE_T, "Works at", &ent1, &ent2);
        rel2 = Relationship(ONE_TO_MANY_T, "Studies", &ent2, &ent3);
    }

    void TearDown() override {
        // Code here will be called immediately after each test (before the destructor)
    }
};

// Test the parameterized constructor
TEST_F(RelationshipTest, ParameterizedConstructor) {
    EXPECT_EQ(rel1.GetType(), ONE_TO_ONE_T);
    EXPECT_EQ(rel1.GetName(), "Works at");
    EXPECT_EQ(rel1.GetLhsEntity(), &ent1);
    EXPECT_EQ(rel1.GetRhsEntity(), &ent2);

    EXPECT_EQ(rel2.GetType(), ONE_TO_MANY_T);
    EXPECT_EQ(rel2.GetName(), "Studies");
    EXPECT_EQ(rel2.GetLhsEntity(), &ent2);
    EXPECT_EQ(rel2.GetRhsEntity(), &ent3);
}

// Test the default constructor
TEST_F(RelationshipTest, DefaultConstructor) {
    Relationship relD;
    EXPECT_EQ(relD.GetType(), UNDEFINED_RELATIONSHIP_T);
    EXPECT_EQ(relD.GetName(), "NO_NAME");
    EXPECT_EQ(relD.GetLhsEntity(), nullptr);
    EXPECT_EQ(relD.GetRhsEntity(), nullptr);
}

// Test the GetName() method
TEST_F(RelationshipTest, GetName) {
    EXPECT_EQ(rel1.GetName(), "Works at");
    EXPECT_EQ(rel2.GetName(), "Studies");
}

// Test the GetType() method
TEST_F(RelationshipTest, GetType) {
    EXPECT_EQ(rel1.GetType(), ONE_TO_ONE_T);
    EXPECT_EQ(rel2.GetType(), ONE_TO_MANY_T);
}

// Test the GetLhsEntity() method
TEST_F(RelationshipTest, GetLhsEntity) {
    EXPECT_EQ(rel1.GetLhsEntity(), &ent1);
    EXPECT_EQ(rel2.GetLhsEntity(), &ent2);
}

// Test the GetRhsEntity() method
TEST_F(RelationshipTest, GetRhsEntity) {
    EXPECT_EQ(rel1.GetRhsEntity(), &ent2);
    EXPECT_EQ(rel2.GetRhsEntity(), &ent3);
}
