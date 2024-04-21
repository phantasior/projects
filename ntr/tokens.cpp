#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <variant>
#include <unordered_map>

struct OpeningBracket {
    friend std::ostream& operator<<(std::ostream& out, const OpeningBracket& token) {
        out << "(";
        return out;
    }
};

struct ClosingBracket {
    friend std::ostream& operator<<(std::ostream& out, const ClosingBracket& token) {
        out << ")";
        return out;
    }
};

struct Number {
  int value;

  friend std::ostream& operator<<(std::ostream& out, const Number& token) {
        out << token.value;
        return out;
    }
};

struct UnknownToken {
  std::string value;

  friend std::ostream& operator<<(std::ostream& out, const UnknownToken& token) {
        out << '[' << token.value << ']';
        return out;
    }
};

struct MaxToken {
    friend std::ostream& operator<<(std::ostream& out, const MaxToken& token) {
        out << "MAX";
        return out;
    }
};

struct MinToken {
    friend std::ostream& operator<<(std::ostream& out, const MinToken& token) {
        out << "MIN";
        return out;
    }
};

struct AbsToken {
    friend std::ostream& operator<<(std::ostream& out, const AbsToken& token) {
        out << "ABS";
        return out;
    }
};

struct Plus {
    friend std::ostream& operator<<(std::ostream& out, const Plus& token) {
        out << "+";
        return out;
    }
};

struct Minus {
    friend std::ostream& operator<<(std::ostream& out, const Minus& token) {
        out << "-";
        return out;
    }
};

struct Multiply {
    friend std::ostream& operator<<(std::ostream& out, const Multiply& token) {
        out << "*";
        return out;
    }
};

struct Modulo {
    friend std::ostream& operator<<(std::ostream& out, const Modulo& token) {
        out << "%";
        return out;
    }
};

struct Divide {
    friend std::ostream& operator<<(std::ostream& out, const Divide& token) {
        out << "/";
        return out;
    }
};

struct Comma {
    friend std::ostream& operator<<(std::ostream& out, const Comma& token) {
        out << ",";
        return out;
    }
};

struct SqrToken {
    friend std::ostream& operator<<(std::ostream& out, const SqrToken& token) {
        out << "SQR";
        return out;
    }
};

struct SqrtToken {
    friend std::ostream& operator<<(std::ostream& out, const SqrtToken& token) {
        out << "SQRT";
        return out;
    }
};

struct UnexpectedChar {
    char chr;

    friend std::ostream& operator<<(std::ostream& out, const UnexpectedChar& token) {
        out << "[[" << token.chr << "]]";
        return out;
    }
};

using Token = std::variant<
            OpeningBracket, 
            ClosingBracket, 
            Number, 
            UnknownToken,
            MinToken,
            AbsToken,
            Plus, 
            Minus,
            Multiply,
            Modulo,
            Divide,
            Comma,
            SqrToken,
            SqrtToken,
            MaxToken,
            UnexpectedChar>;

const std::unordered_map<char, Token> kSymbol2Token{
    {'+', Plus{}}, 
    {'-', Minus{}}, 
    {'*', Multiply{}}, 
    {'/', Divide{}},
    {'%', Modulo{}},
    {',', Comma{}},
    {'(', OpeningBracket{}},
    {')', ClosingBracket{}}};

const std::unordered_map<std::string, Token> kString2Token{
    {"sqr", SqrToken{}},
    {"sqrt", SqrtToken{}},
    {"max", MaxToken{}},
    {"abs", AbsToken{}},
    {"min", MinToken{}}};

int ToDigit(unsigned char symbol) {
  return symbol - '0';
}

Number ParseNumber(const std::string& input, size_t& pos) {
  int value = 0;
  auto symbol = static_cast<unsigned char>(input[pos]);
  while (std::isdigit(symbol)) {
    value = value * 10 + ToDigit(symbol);
    ++pos;
    if (pos == input.size()) {
      break;
    }

    symbol = static_cast<unsigned char>(input[pos]);
  }

  return Number{value};
}

std::string str_tolower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), 
                   [](unsigned char c){ return std::tolower(c); } 
                  );
    return s;
}

Token ParseName(const std::string& input, size_t& pos) {
    std::string result;
    auto symbol = static_cast<unsigned char>(input[pos]);
    while (std::isalpha(symbol)) {
        result.push_back(symbol);
        ++pos;
        if (pos == input.size()) {
            break;
        }

        symbol = static_cast<unsigned char>(input[pos]);
    }
    
    if (auto it = kString2Token.find(str_tolower(result)); it != kString2Token.end()) {
        return it->second;
    } else {
        return UnknownToken{result};
    }
}

std::vector<Token> Tokenize(const std::string& input) {
  std::vector<Token> tokens;
  const size_t size = input.size();
  size_t pos = 0;
  while (pos < size) {
    const auto symbol = static_cast<unsigned char>(input[pos]);

    if (std::isspace(symbol)) {
      ++pos;
      continue;
    } 
    
    if (std::isdigit(symbol)) {
      tokens.emplace_back(ParseNumber(input, pos));
      continue;
    }
    
    if (auto it = kSymbol2Token.find(symbol); it != kSymbol2Token.end()) {
      tokens.emplace_back(it->second);
      ++pos;
      continue;
    }

    if (std::isalpha(symbol)) {
        tokens.push_back(ParseName(input, pos));
        continue;
    }

    tokens.push_back(UnexpectedChar{input[pos++]}); // for example ';' or '#'
  }

  return tokens;
}

int main() {
    std::string data = "12     + + + - / sqrt 3 ! @ # $ % ^ & * ( ) hello sqr (3) + abc";
    auto tokens = Tokenize(data);
    for (const auto& token : tokens) {
        std::visit([](const auto& value) {
            std::cout << value << ' ';
        }, token);
    }
}