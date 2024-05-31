#include "CharSource.h"

CharSource::CharSource(const std::string source) : source_(source), pos_(0) {}


bool CharSource::HasNext() const {
    return pos_ < source_.size();
}

bool CharSource::IsSpace() const {
    return current_ == '\n' || current_ == ' ' || current_ == '\t' || current_ == '\r';
};

char CharSource::Next() {
    return source_[pos_++];
}

std::invalid_argument CharSource::Error(const std::string& error) const {
    return std::invalid_argument(std::to_string(pos_) + ": " + error);
}

char CharSource::Take() {
    const char res = current_;
    current_ = HasNext() ? Next() : END;
    return res;
}

std::string CharSource::ReadWord() {
    std::string res;
    while (std::isalpha(current_) || current_ == '_') {
        res += Take();
    }

    return res;
}

void CharSource::SkipWhitespace() {
    while (Take(' ') || Take('\n') || Take('\t') || Take('\r')) {}
}


bool CharSource::Take(char expected) {
    if (current_ == expected) {
        Take();
        return true;
    }

    return false;
}

void CharSource::Expect(char expected) {
    if (!Take(expected)) {
        throw Error("Expected '" + std::string(1, expected) + "', found '" + std::string(1, Take() ) + "'.");
    }
}

void CharSource::Expect(const std::string& expected) {
    for (char chr : expected) {
        Expect((chr));
    }
}