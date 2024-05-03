#pragma once

#include "base/Model.h"

namespace erconv {
    
    class ScriptGenerator {
        public:
            using EntityID = size_t;
            using RelationshipID = size_t;

            ScriptGenerator() = default;

            static std::vector<std::string> Generate(ERModel& model);

            ~ScriptGenerator() = default;
        private:
            static std::string getEntityFieldDataType(const DataTypeEntity& dataType);
            static std::string getEntityFieldCurrentConstraint(const ConstraintsEntity& constraint);
    };

};
