#pragma once
#include "Game.h"
#include "CompileTimeError.h"
constexpr auto MISSING_START_ERROR = 0;
constexpr auto MISSING_EVENT_ERROR = 1;
constexpr auto MISSING_LOOP_ERROR = 2;
constexpr auto UNKNOWN_CAUSETYPE_ERROR = 3;
constexpr auto NOT_AN_INT_CAUSEVALUE_ERROR = 4;
constexpr auto NOT_A_KEY_CAUSEVALUE_ERROR = 5;
constexpr auto UNEXPECTED_TOKEN_VARIABLE_ERROR = 6;
constexpr auto NOT_A_NUMBER_ERROR = 7;
/*
template<typename T>
int FindLocation(std::vector<T> vec, T search);
template<class T>std::vector<T> GetSubVector(std::vector<T> vec, int start_index, int end_index);
template<class T>const std::vector<int> GetIndexs(std::vector<T> vec, T search);
*/
int compile(std::vector<std::string> lines, Game* game, std::vector<CompileTimeErrorHandel>* errors);