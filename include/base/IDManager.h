#pragma once

#include "base/Errors.h"

#include <limits>

namespace erconv {
    
class IDManager {
private:
    using IntType = size_t;

    inline constexpr static IntType MAX_ID = std::numeric_limits<IntType>::max(); 

    IntType maxID = 0;
public:
    IntType NewID() {
        if (maxID == MAX_ID) {
            throw TError("Maximum number of IDs is exceeded.");
        }
        return maxID++;
    }
};

} // namespace erconv
