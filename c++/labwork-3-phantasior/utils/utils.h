#pragma once

#include "../lib/sandpile.h"

#include <cinttypes>

const uint64_t MAX_SANDPILE_AMOUNT = 1000;

void GenerateRandomInputFile(const Sandpile* sandpile);

void GenerateCentralInputFile(const Sandpile* sandpile, const uint32_t count);