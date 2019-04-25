#pragma once

#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <mutex>
#include <optional>
#include <queue>
#include <regex>
#include <shared_mutex>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <random>

#define let const auto

using namespace std;
using i32 = int32_t;
using u32 = uint32_t;
using u16 = uint16_t;
using i16 = int16_t;
using usize = size_t;
using isize = ptrdiff_t;
using f32 = float;
using f64 = double;

#define RECORD_BOILERPLATE(ClassName)\
    public: ClassName(ClassName&&) = default; ClassName(const ClassName&) = delete;\
            ClassName& operator=(ClassName&&) = default;\
    private: void assert_move_constructor_is_present(ClassName& instance) { ClassName(std::move(instance)); }
