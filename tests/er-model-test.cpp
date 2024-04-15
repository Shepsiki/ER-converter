#include <gtest/gtest.h>

#include "Model.h"

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
        std::vector<erconv::ConstraintsEntity> constr3 {erconv::ConstraintsEntity::FOREIGN_KEY_C};

        ent1.AddField("Id1", erconv::DataTypeEntity::INT_T, constr1);
        ent1.AddField("Name1", erconv::DataTypeEntity::VARCHAR_T, constr2);
        ent1.AddField("Address1", erconv::DataTypeEntity::TEXT_T, constr3);

        ent2.AddField("Id2", erconv::DataTypeEntity::INT_T, constr1);
        ent2.AddField("Name2", erconv::DataTypeEntity::VARCHAR_T, constr2);
        ent2.AddField("Address2", erconv::DataTypeEntity::TEXT_T, constr3);

        ent3.AddField("Id2", erconv::DataTypeEntity::INT_T, constr1);
    }

    void TearDown() override {
        // Clean up common resources here
    }

    // Declare any common variables here
    Entity ent1;
    Entity ent2;
    Entity ent3;
    Entity ent4;
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

TEST_F(ERModelTest, AddRelationship_ValidRelationship) {
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);
    // Add a valid relationship
    EXPECT_TRUE(erModel.AddRelationship(ONE_TO_ONE_T, "Students", "Facilities", "Address2"));
}

TEST_F(ERModelTest, AddRelationship_DuplicateName) {
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);

    // Add a relationship with the same name twice
    erModel.AddRelationship(ONE_TO_ONE_T, "Students", "Facilities", "Address2");
    EXPECT_THROW(erModel.AddRelationship(ONE_TO_ONE_T, "Students", "Facilities", "Address2"), TError);
}

TEST_F(ERModelTest, AddRelationship_InvalidName) {
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);

    // Add a relationship with an invalid name
    EXPECT_THROW(erModel.AddRelationship(ONE_TO_ONE_T, "Students", "Facilities", "Address2", ""), TError);
}

TEST_F(ERModelTest, AddRelationship_NoPrimaryKey) {
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);
    erModel.AddEntity(ent3);
    erModel.AddEntity(ent4);

    // Add a relationship with an invalid name
    EXPECT_THROW(erModel.AddRelationship(ONE_TO_ONE_T, "4", "Facilities", "Address2", ""), TError);
}

TEST_F(ERModelTest, AddRelationship_WrongForeignKey) {
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);
    erModel.AddEntity(ent3);
    erModel.AddEntity(ent4);

    // Add a relationship with an invalid name
    EXPECT_THROW(erModel.AddRelationship(ONE_TO_ONE_T, "3", "Facilities", "KtoYa?", ""), TError);
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
    erModel.AddRelationship(ONE_TO_ONE_T, "Students", "Facilities", "Address2");
    // Remove an existing relationship
    ASSERT_TRUE(erModel.RemoveRelationship("Students", "Facilities", "Address2"));
    // Check if the relationship was removed successfully
}

TEST_F(ERModelTest, RemoveRelationship_NonExistingRelationship) {
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);
    erModel.AddRelationship(ONE_TO_ONE_T, "Students", "Facilities", "Address2");
    // Remove non existing relationship
    EXPECT_THROW(erModel.RemoveRelationship("Studentosiki", "Facilities", "Address2"), TError);
    // Check if the relationship was removed successfully
}

// Search tests

TEST_F(ERModelTest, GetEntity_EntityExists) {
    // Retrieve an existing entity
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);
    const Entity& retrievedEntity = erModel.GetEntity("Facilities");

    // Check if the retrieved entity matches the expected entity
    EXPECT_EQ(retrievedEntity.GetName(), "Facilities");
}

TEST_F(ERModelTest, GetEntity_NonExistentEntity) {
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);
    ASSERT_THROW(erModel.GetEntity("KtoYa?"), TError);
}

TEST_F(ERModelTest, GetRelationship_ExistingRelationship) {
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);
    erModel.AddRelationship(ONE_TO_ONE_T, "Students", "Facilities", "Address2");
    const Relationship& rel = erModel.GetRelationship("Students", "Facilities", "Address2");
    EXPECT_EQ(rel.GetName(), "Students_to_Facilities_Address2");
    EXPECT_EQ(rel.GetType(), ONE_TO_ONE_T);
    EXPECT_EQ(rel.GetLhsEntityName(), "Students");
    EXPECT_EQ(rel.GetRhsEntityName(), "Facilities");
    EXPECT_EQ(rel.GetForeignKey(), "Address2");
    EXPECT_EQ(rel.GetPrimaryKey(), ent1.GetPrimaryKeyName());
}

TEST_F(ERModelTest, GetRelationship_NonExistentRelationship) {
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);
    erModel.AddRelationship(ONE_TO_ONE_T, "Students", "Facilities", "Address2");
    EXPECT_THROW(erModel.GetRelationship("Studentosik", "Facilities", "Address2"), TError);
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
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);
    erModel.AddEntity(ent3);
    erModel.AddEntity(ent4);
    erModel.AddRelationship(ONE_TO_ONE_T, "Students", "Facilities", "Address2");
    erModel.AddRelationship(ONE_TO_ONE_T, "3", "Facilities", "Address2");

    // Get the relationships from the ERModel
    const std::vector<Relationship>& relationships = erModel.GetRelationships();

    // Assert the size of the vector
    ASSERT_EQ(relationships.size(), 2);
}


TEST_F(ERModelTest, GetConnectedRelationships) {
    erModel.AddEntity(ent1);
    erModel.AddEntity(ent2);
    erModel.AddEntity(ent3);
    erModel.AddEntity(ent4);
    erModel.AddRelationship(ONE_TO_ONE_T, "Students", "Facilities", "Address2");
    erModel.AddRelationship(ONE_TO_ONE_T, "3", "Facilities", "Address2");

    // Get the connected relationships for entity ent1
    const std::vector<Relationship>& connectedRelationships = erModel.GetConnectedRelationships(ent2);

    // Assert the size of the vector
    ASSERT_EQ(connectedRelationships.size(), 2);
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
