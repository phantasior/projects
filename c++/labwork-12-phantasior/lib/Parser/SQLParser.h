#pragma once

#include "CharSource.h"

#include <algorithm>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <variant>
#include <unordered_map>

struct MyMonostate : public std::monostate {
    friend std::ostream& operator<<(std::ostream& out, const MyMonostate& monostate) {
        out << "NULL";
        return out;
    }
};

using std::cout;
using std::endl;

using value_type = std::variant<MyMonostate, bool, int, float, double, std::string>;

namespace fs = std::filesystem;

const std::vector<std::string> empty_vector;

enum class SQLType {
    Bool = 0,
    Int = 1,
    Float = 2,
    Double = 3,
    Varchar = 4,
    None = 5
};

enum class JoinType {
    Left,
    Inner,
    Right,
    None
};

struct InfoBlock {
    InfoBlock() : is_primary(false), is_foreign(false) {}
    InfoBlock(std::string name, SQLType type, bool is_primary, bool is_foreign, const std::string& table = "")
            : name(std::move(name))
            , type(type)
            , is_primary(is_primary)
            , is_foreign(is_foreign)
            , foreign_table(table)
    {}
    std::string name;
    std::string foreign_table;
    SQLType type;
    bool is_primary;
    bool is_foreign;
};

class Table {
public:
    Table() {}

    Table(const std::string& table_name, const std::string& dbname);

    void AddColumn(const InfoBlock& info_block);

    bool Save();

    void Join(const Table& other, const std::string& lhs, const std::string& rhs);

    bool Load(const std::vector<std::string>& to_select = empty_vector);

    void ValidateOnSelect(const std::string& expr);

    bool ValidateOnUpdate(const std::string& expr, const std::vector<std::pair<std::string, std::string>>& to_update);

    bool ValidateOnDelete(const std::string& expr, const std::vector<std::string>& to_delete);

    bool Update(std::vector<value_type>& row, const std::vector<std::pair<std::string, std::string>>& to_update);

    bool ValidateRow(const std::vector<value_type>& row, const std::string& expr, CharSource& src);

    value_type ParseValue(std::ifstream& file, SQLType type);

    static bool InsertIntoTable(const std::string& dbname, const std::string& table_name,
                                    const std::vector<std::string>& table_info, const std::vector<std::string>& table_values);

    friend std::ostream& operator<<(std::ostream& out, const Table& table);

private:
    std::string dbname_;
    std::string table_name_;
    std::vector<InfoBlock> info_;
    std::vector<std::vector<value_type>> data_;

};

// either Table, or string like "successfully inserted"
class Response {
public:
    Response(std::string response);

    Response(Table table);

    friend std::ostream& operator<<(std::ostream& out, const Response& response);

private:
    std::variant<Table, std::string> response_;
};


class SQLParser : private CharSource {
public:
    static const char END = 0;

    SQLParser(const std::string& query, const std::string& dbname);

    Response Parse();

    value_type ParseValue();

    std::string ParseCreate();

    std::string ParseInsert();

    std::string ParseUpdate();

    std::string ParseDrop();

    std::string ParseDelete();

    Table ParseSelect();

    static SQLType MapKeyType(const std::string& key);

private:
    std::string dbname_;
};
