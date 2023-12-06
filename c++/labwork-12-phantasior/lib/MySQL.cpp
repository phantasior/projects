#include "MySQL.h"

MySQL::MySQL(const std::string& dbname) : dbname_("." + dbname) {
    if (!fs::exists(dbname_)) {
        fs::create_directory(dbname_);
    }
}

Response MySQL::Query(const std::string& query) const {
    SQLParser parser(query, dbname_);
    return parser.Parse();
}

