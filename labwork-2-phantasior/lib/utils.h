#pragma once

bool operator<=(const std::vector<uint32_t>& lhs, const std::vector<uint32_t>& rhs);

uint16_t GetDecimalLength(uint32_t i);

void AddDecimal(std::vector<uint32_t>& lhs, const std::vector<uint32_t>& rhs);

void SubstractDecimal(std::vector<uint32_t>& lhs, const std::vector<uint32_t>& rhs);

void DoubleDecimal(std::vector<uint32_t>& decimal);

void HalveDecimal(std::vector<uint32_t>& decimal);

void ShiftBits(uint2022_t& bs);

void Print(std::ostream& out, const std::vector<uint32_t>& decimal);