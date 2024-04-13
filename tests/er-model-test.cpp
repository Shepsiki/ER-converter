#include <gtest/gtest.h>

#include "Model.h"
#include "Relationship.h"

namespace erconv {

class ERModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize common resources here
        ent1 = Entity("Students");
        ent2 = Entity("Facilities");
        ent3 = Entity("3");
        ent4 = Entity("4");

        std::vector<erconv::ConstraintsEntity> constr1 {erconv::ConstraintsEntity::PRIMARY_KEY_C};
        std::vector<erconv::ConstraintsEntity> constr2;
        std::vector<erconv::ConstraintsEntity> constr3 {erconv::ConstraintsEntity::NOT_NULL_C};

        ent1.AddField("Id1", erconv::DataTypeEntity::INT_T, constr1);
        ent1.AddField("Name1", erconv::DataTypeEntity::VARCHAR_T, constr2);
        ent1.AddField("Address1", erconv::DataTypeEntity::TEXT_T, constr3);

        ent2.AddField("Id2", erconv::DataTypeEntity::INT_T, constr1);
        ent2.AddField("Name2", erconv::DataTypeEntity::VARCHAR_T, constr2);
        ent2.AddField("Address2", erconv::DataTypeEntity::TEXT_T, constr3);

        rel1 = Relationship(ONE_TO_ONE_T, "Works", &ent1, &ent2);
        rel2 = Relationship(ONE_TO_MANY_T, "Studies", &ent1, &ent2);
    }

    void TearDown() override {
        // Clean up common resources here
    }

    // Declare any common variables here
    Entity ent1;
    Entity ent2;
    Entity ent3;
    Entity ent4;
    Relationship rel1;
    Relationship rel2;
    ERModel erModel;
};

// Creation/Deletion tests

TEST_F(ERModelTest, AddEntityFirst_ValidEntity) {
    ASSERT_TRUE(erModel.AddEntity("Students"));
}

TEST_F(ERModelTest, AddEntitySecond_ValidEntity) {
    ASSERT_TRUE(erModel.AddEntity(ent1));
}

TEST_F(ERModelTest, AddEntity_DuplicateName) {
    erModel.AddEntity("Students"); // Add an entity with the same name
    erModel.AddEntity("Facilities");
    ASSERT_THROW(erModel.AddEntity("Facilities"), TError);
}

TEST_F(ERModelTest, AddEntity_InvalidName) {
    ASSERT_THROW(erModel.AddEntity("InvalidName+-[]"), TError);
}

TEST_F(ERModelTest, AddEntityField_Success) {
    erModel.AddEntity("Students"); // Add an entity to the model
    ASSERT_TRUE(erModel.AddEntityField("Students", "Field1", DataTypeEntity::INT_T, {}));
}

TEST_F(ERModelTest, AddEntityField_EntityNotFound) {
    ASSERT_THROW(erModel.AddEntityField("NonExistentEntity", "Field1", DataTypeEntity::INT_T, {}), TError);
}

TEST_F(ERModelTest, AddRelationship1_ValidRelationship) {
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);

    // Add a valid relationship
    EXPECT_TRUE(erModel.AddRelationship(ONE_TO_ONE_T, "Works", ent1, ent2));
}

TEST_F(ERModelTest, AddRelationship2_ValidRelationship) {
    EXPECT_TRUE(erModel.AddRelationship(rel1));
}

TEST_F(ERModelTest, AddRelationship_DuplicateName) {
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);

    // Add a relationship with the same name twice
    erModel.AddRelationship(ONE_TO_ONE_T, "Works", ent1, ent2);
    EXPECT_THROW(erModel.AddRelationship(ONE_TO_MANY_T, "Works", ent1, ent2), TError);
}

TEST_F(ERModelTest, AddRelationship_InvalidName) {
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);

    // Add a relationship with an invalid name
    EXPECT_THROW(erModel.AddRelationship(ONE_TO_MANY_T, "", ent1, ent2), TError);
}

TEST_F(ERModelTest, RemoveEntity_ValidEntity) {
    // Remove an existing entity
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);
    erModel.AddEntity(ent3);
    erModel.AddEntity(ent4);
    ASSERT_TRUE(erModel.RemoveEntity("Students"));
    // Check if the entity was removed successfully
    EXPECT_EQ(erModel.GetEntities().size(), 3);
    // Add more assertions if necessary
}

TEST_F(ERModelTest, RemoveEntity_NonExistingEntity) {
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);
    erModel.AddEntity(ent3);
    erModel.AddEntity(ent4);
    // Remove a non-existing entity
    EXPECT_THROW(erModel.RemoveEntity("NonExistingEntity"), TError);
    // Add more assertions if necessary
}

TEST_F(ERModelTest, RemoveEntityField_ValidEntityAndField) {
    erModel.AddEntity(ent1);
    // Remove a field from an existing entity
    ASSERT_TRUE(erModel.RemoveEntityField("Students", "Id1"));
    // Check if the field was removed successfully
}

TEST_F(ERModelTest, RemoveEntityField_NonExistingEntity) {
    erModel.AddEntity(ent1);
    // Remove a field from a non-existing entity
    EXPECT_THROW(erModel.RemoveEntityField("NonExistingEntity", "Field"), TError);
}

TEST_F(ERModelTest, RemoveEntityField_NonExistingField) {
    erModel.AddEntity(ent1);
    // Remove a non-existing field from an existing entity
    EXPECT_THROW(erModel.RemoveEntityField("Students", "NonExistingField"), TError);
    // Add more assertions if necessary
}

TEST_F(ERModelTest, RemoveRelationship_ValidRelationship) {
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);
    erModel.AddRelationship(rel1);
    // Remove an existing relationship
    ASSERT_TRUE(erModel.RemoveRelationship("Works"));
    // Check if the relationship was removed successfully
}

TEST_F(ERModelTest, RemoveRelationship_NonExistingRelationship) {
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);
    erModel.AddEntity(ent3);
    erModel.AddEntity(ent4);
    erModel.AddRelationship(rel1);
    erModel.AddRelationship(rel2);
    // Remove a non-existing relationship
    EXPECT_THROW(erModel.RemoveRelationship("NonExistingRelationship"), TError);
}

// Search tests

TEST_F(ERModelTest, FindEntity_ExistingEntity) {
    erModel.AddEntity("Students"); // Add an entity to the model
    erModel.AddEntity("Facilities");
    const Entity* entityPtr = erModel.FindEntity("Students");
    ASSERT_NE(entityPtr, nullptr);
}

TEST_F(ERModelTest, FindEntity_NonExistentEntity) {
    const Entity* entityPtr = erModel.FindEntity("NonExistentEntity");
    ASSERT_EQ(entityPtr, nullptr);
}

TEST_F(ERModelTest, FindRelationship_ExistingRelationship) {
    erModel.AddRelationship(rel1);
    erModel.AddRelationship(rel2);
    erModel.AddRelationship(ONE_TO_ONE_T, "TestRel", ent1, ent2);
    const Relationship* relPtr = erModel.FindRelationship("Works");
    ASSERT_NE(relPtr, nullptr);
    relPtr = erModel.FindRelationship("TestRel");
    ASSERT_NE(relPtr, nullptr);
}

TEST_F(ERModelTest, FindRelationship_NonExistentRelationship) {
    const Relationship* relPtr = erModel.FindRelationship("BYE_BYE");
    ASSERT_EQ(relPtr, nullptr);
}

// Getters tests

TEST_F(ERModelTest, GetEntity_ExistingEntity) {
    erModel.AddEntity("Students"); // Add an entity to the model
    erModel.AddEntity("Facilities");
    const Entity& entity = erModel.GetEntity("Students");
    // Assert that the correct entity is returned
    ASSERT_EQ(entity.GetName(), "Students");
}

TEST_F(ERModelTest, GetEntity_NonExistingEntity) {
    erModel.AddEntity("Students"); // Add an entity to the model
    erModel.AddEntity("Facilities");
    // Assert that the correct exception is thrown when the entity doesn't exist
    ASSERT_THROW(erModel.GetEntity("NonExistingEntity"), TError);
}

TEST_F(ERModelTest, GetRelationship_ExistingRelationship) {
    erModel.AddRelationship(rel1);
    erModel.AddRelationship(rel2);
    erModel.AddRelationship(ONE_TO_ONE_T, "TestRel", ent1, ent2);
    const Relationship& relationship = erModel.GetRelationship("Works");
    // Assert that the correct relationship is returned
    ASSERT_EQ(relationship.GetName(), "Works");
}

TEST_F(ERModelTest, GetRelationship_NonExistingRelationship) {
    // Assert that the correct exception is thrown when the relationship doesn't exist
    ASSERT_THROW(erModel.GetRelationship("NonExistingRelationship"), TError);
}


TEST_F(ERModelTest, GetEntities) {
    // Add entities to the ERModel
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);
    erModel.AddEntity(ent3);
    erModel.AddEntity(ent4);

    // Get the entities from the ERModel
    const std::vector<Entity>& entities = erModel.GetEntities();

    // Assert the size of the vector
    ASSERT_EQ(entities.size(), 4);

    // Assert the content of the vector
    ASSERT_TRUE(std::find_if(entities.begin(), entities.end(), [&](const Entity& entity) {
        return entity.GetName() == "Students";
    }) != entities.end());

    ASSERT_TRUE(std::find_if(entities.begin(), entities.end(), [&](const Entity& entity) {
        return entity.GetName() == "Facilities";
    }) != entities.end());

    ASSERT_TRUE(std::find_if(entities.begin(), entities.end(), [&](const Entity& entity) {
        return entity.GetName() == "3";
    }) != entities.end());

    ASSERT_TRUE(std::find_if(entities.begin(), entities.end(), [&](const Entity& entity) {
        return entity.GetName() == "4";
    }) != entities.end());
}


TEST_F(ERModelTest, GetRelationships) {
    // Add relationships to the ERModel
    erModel.AddRelationship(rel1);
    erModel.AddRelationship(rel2);

    // Get the relationships from the ERModel
    const std::vector<Relationship>& relationships = erModel.GetRelationships();

    // Assert the size of the vector
    ASSERT_EQ(relationships.size(), 2);

    // Assert the content of the vector
    ASSERT_TRUE(std::find_if(relationships.begin(), relationships.end(), [&](const Relationship& relationship) {
        return relationship.GetName() == "Works";
    }) != relationships.end());

    ASSERT_TRUE(std::find_if(relationships.begin(), relationships.end(), [&](const Relationship& relationship) {
        return relationship.GetName() == "Studies";
    }) != relationships.end());
}


TEST_F(ERModelTest, GetConnectedRelationships) {
    // Add entities to the ERModel
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);
    erModel.AddEntity(ent3);
    erModel.AddEntity(ent4);

    // Add relationships to the ERModel
    erModel.AddRelationship(rel1);
    erModel.AddRelationship(rel2);

    // Get the connected relationships for entity ent1
    const std::vector<Relationship>& connectedRelationships = erModel.GetConnectedRelationships(ent1);

    // Assert the size of the vector
    ASSERT_EQ(connectedRelationships.size(), 2);

    // Assert the content of the vector
    ASSERT_TRUE(std::find_if(connectedRelationships.begin(), connectedRelationships.end(), [&](const Relationship& relationship) {
        return relationship.GetName() == "Works";
    }) != connectedRelationships.end());

    ASSERT_TRUE(std::find_if(connectedRelationships.begin(), connectedRelationships.end(), [&](const Relationship& relationship) {
        return relationship.GetName() == "Studies";
    }) != connectedRelationships.end());
}


// Check tests

TEST_F(ERModelTest, IsEmpty) {
    // Initially, the ERModel should be empty
    ASSERT_TRUE(erModel.IsEmpty());

    // Add an entity to the ERModel
    erModel.AddEntity(ent1);

    // Now, the ERModel should not be empty
    ASSERT_FALSE(erModel.IsEmpty());
}

} // namespace erconv
