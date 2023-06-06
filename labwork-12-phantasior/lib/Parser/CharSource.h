#pragma once

#include <string>
#include <exception>

class CharSource {
public:
    const char END = 0;

    CharSource(const std::string source);

    bool HasNext() const;

    bool IsSpace() const;

    char Next();

    std::string ReadWord();

    void SkipWhitespace();

    char Take();

    bool Take(char expected);

    void Expect(char expected);

    void Expect(const std::string& expected);

    std::invalid_argument Error(const std::string& error) const;

public:
    char current_;
    std::string source_;
    size_t pos_;
};