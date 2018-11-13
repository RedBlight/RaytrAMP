#ifndef TYPEDEF_INCLUDED
#define TYPEDEF_INCLUDED

#include <amp.h>
#include <amp_math.h>
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <locale>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <random>
#include <amp.h>
#include <amp_math.h>
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <vector>
#include <algorithm>
#include <fstream>
#include <bitset>

typedef float F32;
typedef double F64;
typedef std::int32_t I32;
typedef std::int64_t I64;
typedef std::uint8_t U8;
typedef std::uint16_t U16;
typedef std::uint32_t U32;
typedef std::uint64_t U64;

#define PARFOR concurrency::parallel_for_each
typedef std::chrono::high_resolution_clock Clock;

// Float numbers can be easily set to 64 bit (double). 
typedef F32 Float;

#endif