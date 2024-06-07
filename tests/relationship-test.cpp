#include <gtest/gtest.h>

#include "base/Model.h"

using namespace erconv;

class RelationshipTest : public ::testing::Test {
protected:
    // Declare variables here if you need them for multiple tests
    ERModel model1;
    ERModel model2;
    Entity ent1;
    Entity ent2;
    Entity ent3;

    void SetUp() override {
        model1 = ERModel();
        model2 = ERModel();
    }

    void TearDown() override {
        // Code here will be called immediately after each test (before the destructor)
    }
};

// Test the default constructor
TEST_F(RelationshipTest, DefaultConstructor) {
    Relationship relD(&model1, ONE_TO_ONE_T, "Students", "Facilities", "default");
    EXPECT_EQ(relD.GetType(), ONE_TO_ONE_T);
    EXPECT_EQ(relD.GetLhsEntityName(), "Students");
    EXPECT_EQ(relD.GetRhsEntityName(), "Facilities");
    EXPECT_EQ(relD.GetName(), "default");
}

TEST_F(RelationshipTest, GetModelPointer) {
    Relationship rel1(&model1, ONE_TO_ONE_T, "Students", "Facilities", "default");
    Relationship rel2(&model2, ONE_TO_ONE_T, "Students", "Facilities", "default");
    Relationship rel3(&model1, ONE_TO_ONE_T, "1", "2", "default");

    EXPECT_EQ(rel1.GetModelPtr(), &model1);
    EXPECT_EQ(rel2.GetModelPtr(), &model2);
    EXPECT_EQ(rel1.GetModelPtr(), rel3.GetModelPtr());
}

