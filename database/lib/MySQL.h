#pragma once

#include "Parser/SQLParser.h"

#include <algorithm>
#include <filesystem>
#include <string>
#include <iostream>

namespace fs = std::filesystem;

//                      Table:
//     -----------------------------------------
//     |InfoBlock|InfoBlock|InfoBlock|InfoBlock|     <- Info
//     -----------------------------------------
//     |         |         |         |         |     <- Entry
//     -----------------------------------------
//     |         |         |         |         |     <- Entry
//     -----------------------------------------
//     |         |         |         |         |     <- Entry
//     -----------------------------------------
//

class MySQL {
public:
    MySQL(const std::string& dbname);

    Response  Query(const std::string& query) const;

private:
    std::string dbname_;
};