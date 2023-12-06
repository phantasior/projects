#include "SQLParser.h"

SQLParser::SQLParser(const std::string& query, const std::string& dbname)
    : CharSource(query), dbname_(dbname) {
    Take();
}

Response SQLParser::Parse() {
    SkipWhitespace();
    if (Take('C')) {
        return ParseCreate();
    } else if (Take('I')) { // insert or inner join
        return ParseInsert();
    } else if (Take('S')) {
        return ParseSelect();
    } else if (Take('D')) { // drop/delete
        if (Take('R')) {
            return ParseDrop();
        } else {
            return ParseDelete();
        }
    } else if (Take('U')) {
        return ParseUpdate();
    }


    return Table();
}

std::string SQLParser::ParseUpdate() {
    Expect("PDATE");
    SkipWhitespace();
    std::string table_name = ReadWord();
    SkipWhitespace();
    Expect("SET");
    std::vector<std::pair<std::string, std::string>> to_update;
    while (current_ != 'W' && current_ != ';') {
        SkipWhitespace();
        std::string key = ReadWord();
        SkipWhitespace();
        Expect('=');
        SkipWhitespace();
        std::string value;
        while (!IsSpace() && current_ != ',') {
            value += Take();
        }
        SkipWhitespace();

        to_update.emplace_back(key, value);

        if (current_ == 'W' || current_ == ';') {
            break;
        }

        Expect(',');
    }

    Table table(table_name, dbname_);
    if (!table.Load()) {
        throw std::invalid_argument("No such table");
    }

    SkipWhitespace();
    if (Take('W')) {
        Expect("HERE");
        SkipWhitespace();
        std::string expr;
        while (current_ != END && current_ != ';') {
            expr += Take();
        }

        if (!table.ValidateOnUpdate(expr, to_update)) {
            throw std::invalid_argument("Couldn't UPDATE table.");
        };
    }

    table.Save();

    return "Table was successfully updated";
}

std::string SQLParser::ParseDrop() {
    Expect("OP");
    SkipWhitespace();
    Expect("TABLE");
    SkipWhitespace();
    std::string table_name = ReadWord();

    fs::path path = dbname_;
    path = path / table_name;
    if (fs::exists(path)) {
        fs::remove(path);
        return "Table was successfully deleted";
    }  else {
        return "There is no such table";
    }

    Expect(';');
}

value_type SQLParser::ParseValue() {
    // bool int float double varchar
    value_type res;
    if (Take('"')) {
        std::string word;
        while (current_ != '"') {
            word += current_;
            Take();
        }

        Take();
        res = word;
    } else {
        std::string num;
        while ('0' <= current_ && current_ <= '9') {
            num += current_;
            Take();
        }

        res = stoi(num);
    }

    return res;
}

std::string SQLParser::ParseDelete() {
    Expect("ELETE");
    SkipWhitespace();
    Expect("FROM");
    SkipWhitespace();
    std::string table_name = ReadWord();
    SkipWhitespace();

    std::vector<std::string> to_delete;
    Table table(table_name, dbname_);
    if (!table.Load()) {
        throw std::invalid_argument("No such table");
    }

    if (Take('W')) {
        Expect("HERE");
        SkipWhitespace();
        std::string expr;
        while (current_ != END && current_ != ';') {
            expr += Take();
        }

        table.ValidateOnDelete(expr, to_delete);
    }

    table.Save();

    return "Rows were successfully deleted";
}

Table SQLParser::ParseSelect() {
    Expect("ELECT");
    SkipWhitespace();
    bool all = false;
    std::vector<std::string> to_select;
    if (Take('*')) {
        all = true;
    } else {
        std::string cur;
        while (current_ != END && cur != "FROM") {
            SkipWhitespace();
            cur = ReadWord();
            to_select.push_back(cur);
            SkipWhitespace();
            if (Take(',')) {
                continue;
            }

            SkipWhitespace();
            if (cur == "FROM") {
                break;
            }
            break;
        }
    }

    SkipWhitespace();
    Expect("FROM");
    SkipWhitespace();
    std::string table_name = ReadWord();
    SkipWhitespace();

    if (!all && to_select.size() == 0) {
        throw std::invalid_argument("No rows selected");
    }

    Table table(table_name, dbname_);
    if (!table.Load(to_select)) {
        throw std::invalid_argument("No such table");
    }

    JoinType join_type = JoinType::None;

    if (Take('I')) {
        Expect("NNER");
        join_type = JoinType::Inner;
    }

    if (Take('L')) {
        Expect("EFT");
        join_type = JoinType::Left;
    }

    if (Take('R')) {
        Expect("IGHT");
        join_type = JoinType::Right;
    }

    SkipWhitespace();

    if (Take('J')) {
        Expect("OIN");
        SkipWhitespace();
        std::string join_table_name = ReadWord();
        SkipWhitespace();
        Expect("ON");
        SkipWhitespace();

        std::string lhs;
        while (!IsSpace() && current_ != '=') {
            lhs += Take();
        }

        SkipWhitespace();
        Expect('=');
        SkipWhitespace();

        std::string rhs;
        while (!IsSpace() && current_ != ';') {
            rhs += Take();
        }

        Table join_table(join_table_name, dbname_);
        if (!join_table.Load(to_select)) {
            throw std::invalid_argument("No such table");
        }

        lhs = lhs.substr(std::find(lhs.begin(), lhs.end(), '.') - lhs.begin() + 1);
        rhs = rhs.substr(std::find(rhs.begin(), rhs.end(), '.') - rhs.begin() + 1);

        table.Join(join_table, lhs, rhs);

        SkipWhitespace();
    } else {
        if (join_type != JoinType::None) {
            throw std::invalid_argument("Expected JOIN keyword");
        }
    }

    SkipWhitespace();

    if (Take('W')) {
        Expect("HERE");
        SkipWhitespace();
        std::string expr;
        while (current_ != END && current_ != ';') {
            expr += Take();
        }

        table.ValidateOnSelect(expr);
    }

    return table;
}

std::string SQLParser::ParseInsert() {
    Expect("NSERT");
    SkipWhitespace();
    Expect("INTO");
    SkipWhitespace();

    std::string table_name = ReadWord();
    SkipWhitespace();
    Expect('(');

    std::vector<std::string> table_info;
    while (current_ != ')') {
        SkipWhitespace();
        std::string cur = ReadWord();
        SkipWhitespace();

        table_info.push_back(cur);

        if (current_ == ')') {
            break;
        }

        Expect(',');
    }

    Take(); // ')'
    SkipWhitespace();
    Expect("VALUES");
    SkipWhitespace();
    Expect('(');

    std::vector<std::string> table_values;
    while (current_ != ')') {
        SkipWhitespace();
        std::string cur;
        while (current_ != ',' && current_ != ')') {
            cur += current_;
            Take();
        }

        SkipWhitespace();
        table_values.push_back(cur);

        if (current_ == ')') {
            break;
        }

        Expect(',');
    }

    Expect(')');
    SkipWhitespace();
    Expect(';');

    if (!Table::InsertIntoTable(dbname_, table_name, table_info, table_values)) {
        throw std::invalid_argument("No table " + table_name + " in " + dbname_);
    }

    return "successfully inserted";
}

std::string SQLParser::ParseCreate() {
    Expect("REATE");
    SkipWhitespace();
    Expect("TABLE");
    SkipWhitespace();

    std::string table_name = ReadWord();
    SkipWhitespace();
    Expect('(');
    SkipWhitespace();

    Table table(table_name, dbname_);

    bool was_primary = false;
    while (current_ != ')') {
        InfoBlock info_block;
        info_block.name = ReadWord();
        SkipWhitespace();
        info_block.type = MapKeyType(ReadWord());
        SkipWhitespace();

        if (Take('P')) {
            if (was_primary) {
                throw Error("Second primary key");
            }

            Expect("RIMARY");
            SkipWhitespace();
            Expect("KEY");
            was_primary = true;

            info_block.is_primary = true;
        }

        if (Take('F')) {
            Expect("OREIGN");
            SkipWhitespace();
            Expect("KEY");
            SkipWhitespace();
            Expect("REFERENCES");
            SkipWhitespace();

            std::string foreign_table_name;
            while (current_ != '(') {
                foreign_table_name += Take();
            }

            Expect('(');
            SkipWhitespace();
            std::string foreign_row_key = ReadWord();
            Expect(')');

            info_block.is_foreign = true;
            info_block.foreign_table = foreign_table_name;
        }

        table.AddColumn(info_block);

        SkipWhitespace();

        if (current_ == ')') {
            break;
        }

        Expect(',');
        SkipWhitespace();
    }

    table.Save();

    Take();
    Expect(';');

    return "successfully created";
}

SQLType SQLParser::MapKeyType(const std::string& key) {
    if (key == "BOOL") {
        return SQLType::Bool;
    } else if (key == "INT") {
        return SQLType::Int;
    } else if (key == "FLOAT") {
        return SQLType::Float;
    } else if (key == "DOUBLE") {
        return SQLType::Double;
    } else if (key == "VARCHAR") {
        return SQLType::Varchar;
    }

    return SQLType::None;
}

//////////// TABLE methods

Table::Table(const std::string& table_name, const std::string& dbname)
    : dbname_(dbname), table_name_(table_name) {}

void Table::AddColumn(const InfoBlock& info_block) {
    info_.push_back(info_block);
    std::for_each(data_.begin(), data_.end(), [](auto& el) { el.push_back(MyMonostate{}); });
}

void Table::Join(const Table& other, const std::string& lhs, const std::string& rhs) {
    for (const auto& i : other.info_) {
        if (std::find_if(info_.begin(), info_.end(), [i](const auto& el) { return i.name ==  el.name; }) == info_.end()) {
            AddColumn(i);
        }
    }

    size_t join_col = 0;
    for (size_t i = 0; i < info_.size(); ++i) {
        if (lhs == info_[i].name) {
            join_col = i;
            break;
        }
    }

    size_t other_join_col = 0;
    for (size_t i = 0; i < other.info_.size(); ++i) {
        if (rhs == other.info_[i].name) {
            other_join_col = i;
            break;
        }
    }

    for (const auto& row : other.data_) {
        value_type need = row[other_join_col];

        for (size_t i = 0; i < data_.size(); ++i) {
            if (need != data_[i][join_col]) {
                continue;
            }

            for (size_t j = 0; j < other.info_.size(); ++j) {
                auto it = std::find_if(info_.begin(), info_.end(), [other, j](auto&& el) { return el.name == other.info_[j].name; });
                if (it == info_.end()) {
                    continue;
                }

                data_[i][it - info_.begin()] = row[j];
            }

            break;
        }
    }

    cout << lhs << '|' <<rhs << endl;
}

bool Table::Save() {
    fs::path path = dbname_;
    path = path / table_name_;
    std::ofstream file(path, std::ios::trunc);

    if (!file.is_open()) {
        return false;
    }

    file << data_.size() << ' ' << info_.size() << std::endl; // n m
    for (size_t i = 0; i < info_.size(); ++i) {
        file << info_[i].name;
        if (info_[i].is_primary) {
            file << "$ ";
        } else if (info_[i].is_foreign) {
            file << "$" << info_[i].foreign_table << "# ";
        } else {
            file << " ";
        }

        switch (info_[i].type) {
            case SQLType::Bool:
                file << 0;
                break;
            case SQLType::Int:
                file << 1;
                break;
            case SQLType::Float:
                file << 2;
                break;
            case SQLType::Double:
                file << 3;
                break;
            case SQLType::Varchar:
                file << 4;
                break;
            case SQLType::None:
                file << 5;
                break;
        }

        file << ' ';
    }

    file << std::endl;
    for (size_t i = 0; i < data_.size(); ++i) {
        for (int j = 0; j < data_[i].size(); ++j) {
            std::visit([&](auto&& el) {file << el << ' ';}, data_[i][j]);
        }

        file << std::endl;
    }

    file.close();

    return true;
}

bool Table::Load(const std::vector<std::string>& to_select) {
    fs::path path = dbname_;
    path = path / table_name_;

    std::ifstream file(path, std::ios_base::in);
    if (!file.is_open()) {
        return false;
    }

    int n, m;
    file >> n >> m;


    for (size_t i = 0; i < m; ++i) {
        InfoBlock info_block;

        file >> info_block.name;
        int type;
        file >> type;
        info_block.type = SQLType(type);

        if (info_block.name.back() == '$') {
            info_block.name.pop_back();
            info_block.is_primary = true;
        } else if (info_block.name.back() == '#') {
            info_block.name.pop_back();
            info_block.is_foreign = true;
            size_t dollar_pos = std::find(info_block.name.begin(), info_block.name.end(), '$') - info_block.name.begin();
            info_block.foreign_table = info_block.name.substr(dollar_pos + 1);
            info_block.name = info_block.name.substr(0, dollar_pos);
        }

        info_.push_back(info_block);
    }

    data_.resize(n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            if (to_select.empty() || std::find(to_select.begin(), to_select.end(), info_[j].name) != to_select.end()) {
                data_[i].push_back(ParseValue(file, info_[j].type));
            } else {
                ParseValue(file, info_[j].type);
            }
        }
    }

    return true;
}

void Table::ValidateOnSelect(const std::string& expr) {
    std::vector<std::vector<value_type>> temp;
    for (auto it = data_.begin(); it < data_.end(); ++it) {
        CharSource src(expr);
        src.Take();
        src.Expect('(');
        if (ValidateRow(*it, expr,src)) {
            temp.push_back(*it);
        }
    }

    data_ = temp;
}

bool Table::ValidateOnUpdate(const std::string& expr, const std::vector<std::pair<std::string, std::string>>& to_update) {
    for (auto it = data_.begin(); it < data_.end(); ++it) {
        CharSource src(expr);
        src.Take();
        src.Expect('(');
        if (ValidateRow(*it, expr,src)) {
            if (!Update(*it, to_update)) {
                return false;
            };
        }
    }

    return true;
}

bool Table::ValidateOnDelete(const std::string& expr, const std::vector<std::string>& to_delete) {
    std::vector<std::vector<value_type>> temp;
    for (auto it = data_.begin(); it < data_.end(); ++it) {
        CharSource src(expr);
        src.Take();
        src.Expect('(');
        if (!ValidateRow(*it, expr,src)) {
            temp.push_back(*it);
        }
    }

    data_ = temp;

    return true;
}

bool Table::Update(std::vector<value_type>& row, const std::vector<std::pair<std::string, std::string>>& to_update) {
    for (size_t i = 0; i < to_update.size(); ++i) {
        auto it = std::find_if(info_.begin(), info_.end(), [to_update, i](auto&& el) { return el.name == to_update[i].first; });
        if (it == info_.end()) {
            return false;
        }

        size_t pos = it - info_.begin();
        if (it->type == SQLType::Int) {
            row[pos] = stoi(to_update[i].second);
        } else if (it->type == SQLType::Float) {
            row[pos] = stof(to_update[i].second);
        } else if (it->type == SQLType::Double) {
            row[pos] = stod(to_update[i].second);
        } else if (it->type == SQLType::Bool) {
            row[pos] = to_update[i].second == "true";
        } else {
            row[pos] = to_update[i].second;
        }
    }

    return true;
}

bool Table::ValidateRow(const std::vector<value_type>& row, const std::string& expr, CharSource& src) {
    src.SkipWhitespace();

    while (src.current_ != ')') {
        src.SkipWhitespace();
        if (src.Take('(')) {
            bool lhs = ValidateRow(row, expr, src);
            src.SkipWhitespace();
            std::string op = src.ReadWord();
            if (op == "AND" || op == "OR") {
                src.SkipWhitespace();
                src.Expect('(');
                bool rhs =  ValidateRow(row, expr, src);

                src.Expect(')');
                return (op == "OR" ? lhs || rhs : lhs && rhs);
            } else {
                throw std::invalid_argument("Incorrect WHERE clause");
            }
        }

        std::string lhs_str;
        while (!src.IsSpace() && src.current_ != '=') {
            lhs_str += src.Take();
        }

        src.SkipWhitespace();
        std::string op;
        while (!src.IsSpace() && !std::isalpha(src.current_)) {
            op += src.Take();
        }

        src.SkipWhitespace();
        std::string rhs_str;
        while (!src.IsSpace() && src.current_ != ')') {
            rhs_str += src.Take();
        }

        src.SkipWhitespace();

        value_type lhs = lhs_str;
        value_type rhs = rhs_str;

        SQLType main_type = SQLType::Varchar;

        for (size_t i = 0; i < info_.size(); ++i) {
            if (info_[i].name == lhs_str) {
                lhs = row[i];
                main_type = SQLType(lhs.index() - 1);
                break;
            }
        }

        for (size_t i = 0; i < info_.size(); ++i) {
            if (info_[i].name == rhs_str) {
                rhs = row[i];
                main_type = SQLType(rhs.index() - 1);
                break;
            }
        }

        if (main_type == SQLType::Bool) {
            if (lhs.index() != 1) lhs = lhs_str == "true";
            if (rhs.index() != 1) rhs = rhs_str == "true";
        } else if (main_type == SQLType::Int) {
            if (lhs.index() != 2) lhs = stoi(lhs_str);
            if (rhs.index() != 2) rhs = stoi(rhs_str);
        } else if (main_type == SQLType::Float) {
            if (lhs.index() != 3) lhs = stof(lhs_str);
            if (rhs.index() != 3) rhs = stof(rhs_str);
        } else if (main_type == SQLType::Double) {
            if (lhs.index() != 4) lhs = stod(lhs_str);
            if (rhs.index() != 4) rhs = stod(rhs_str);
        } else {
            lhs = std::get<std::string>(lhs)[0] == '"' ? std::get<std::string>(lhs).substr(1, std::get<std::string>(lhs).size() - 2) : lhs;
            rhs = std::get<std::string>(rhs)[0] == '"' ? std::get<std::string>(rhs).substr(1, std::get<std::string>(rhs).size() - 2) : rhs;
        }

        src.Expect(')');

        return std::visit([&rhs_str, &op, &lhs, &rhs](value_type&& a, value_type&& b) -> bool {
            if (op == "=") {
                return a == b;
            } else if (op == "!=") {
                return a != b;
            } else if (op == "<") {
                return a < b;
            } else if (op == ">") {
                return a > b;
            } else if (op == "<=") {
                return a <= b;
            } else if (op == ">=") {
                return a >= b;
            } else if (op == "IS") {
                if (rhs_str == "NOT NULL") {
                    return lhs.index() != 0;
                } else if (rhs_str == "NULL"){
                    return lhs.index() == 0;
                } else {
                    throw std::invalid_argument("IS only supported with (NOT) NULL");
                }
            } else {
                throw std::invalid_argument("Unknown operand");
            }
        }, lhs, rhs);
    }

    return false;
}

value_type Table::ParseValue(std::ifstream& file, SQLType type) {
    char current = file.get();
    while (current == ' ' || current == '\n' || current == '\r' || current == '\t') {
        current = file.get();
    }

    if (type == SQLType::Varchar) {
        if (current != '"') {
            throw std::invalid_argument("This is not a string");
        }

        std::string value;
        while ((current = file.get()) != '"') {
            value += current;
        }

        return value;
    } else if (type == SQLType::Int) {
        if (!std::isdigit(current)) {
            throw std::invalid_argument("This is not a number");
        }

        int num = current - '0';
        while (std::isdigit((current = file.get()))) {
            num *= 10;
            num += current - '0';
        }

        return num;
    } else if (type == SQLType::Bool) {
        std::string value;
        file >> value;
        if (current == 't' && value == "rue") {
            return true;
        } else if (current == 'f' && value == "alse") {
            return false;
        }

        return "Null";
    } else if (type == SQLType::Double || type == SQLType::Float) {
        if (!std::isdigit(current)) {
            throw std::invalid_argument("This is not a double/float");
        }

        bool was_dot = false;
        std::string num;
        while (std::isdigit(current) || current == '.') {
            if (current == '.') {
                if (was_dot) {
                    throw std::invalid_argument("Incorrect double/float number");
                }

                was_dot = true;
            }

            num += current;
            current = file.get();
        }

        if (type == SQLType::Float) {
            return stof(num);
        } else {
            return stod(num);
        }
    }
    return "Null";
}

bool Table::InsertIntoTable(const std::string& dbname, const std::string& table_name,
                            const std::vector<std::string>& table_info, const std::vector<std::string>& table_values) {
    fs::path path = dbname;
    path = path / table_name;
    std::fstream file(path, std::ios_base::binary | std::ios_base::out | std::ios_base::in);

    if (!file.is_open()) {
        return false;
    }

    size_t row;
    file >> row;

    file.seekp(std::ios::beg);
    file << row + 1;

    file.close();
    file.open(path, std::ios::app);

    for (const auto& i : table_values) {
        file << i << ' ';
    }

    file << std::endl;
    file.close();

    return true;
}

std::ostream& operator<<(std::ostream& out, const Table& table) {
    auto get = [](const auto& a) -> size_t {
        size_t ret = a.name.size();
        ret += a.is_primary ? 1 : 0;
        ret += a.is_foreign ? (2 + a.foreign_table.size()) : 0;
        return ret;
    };

    const size_t COLUMN_WIDTH = get(*std::max_element(table.info_.begin(), table.info_.end(), [get](auto&& a, auto&& b) {
        return get(a) < get(b);
    }));

    out << std::setw(COLUMN_WIDTH * (table.data_[0].size() / 2))
        << "-----Database: " << table.dbname_.substr(1)
        << ", table name: " << table.table_name_ << "-----" << std::endl;
    for (const auto& i : table.info_) {
        std::string output;
        output += i.name;
        if (i.is_primary) {
            output += "$ | ";
        } else if (i.is_foreign) {
            output += "(" + i.foreign_table + ") | ";
        } else {
            output += " | ";
        }

        out << std::setw(COLUMN_WIDTH + 3) << output;
    }

    for (const auto& i : table.data_) {
        out << std::endl;
        for (size_t j = 0; j < COLUMN_WIDTH * table.data_[0].size() + 10; ++j) {
            out << '-';
        }
        out << std::endl;
        for (size_t j = 0; j < i.size(); ++j) {
            auto current = i[j];

//            out << std::setw(std::max(COLUMN_WIDTH, table.info_[j].name.size() + 1));
            out << std::setw(COLUMN_WIDTH);
            if (current.index() == 0) {
                out << std::get<MyMonostate>(current);
            } else if (current.index() == 1) {
                out << (std::get<bool>(current) ? "true" : "false");
            } else if (current.index() == 2) {
                out << std::get<int>(current);
            } else if (current.index() == 3 || current.index() == 4) {
                out << std::get<double>(current);
            } else {
                out << std::get<std::string>(current);
            }
            out << " | ";
        }
    }

    return out;
}

///////////////// RESPONSE METHODS

Response::Response(std::string response) : response_(std::move(response)) {}

Response::Response(Table table) : response_(std::move(table)) {}

std::ostream& operator<<(std::ostream& out, const Response& response) {
    std::visit([&out](auto&& val) { out << val; }, response.response_);

    return out;
}