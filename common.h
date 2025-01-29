/*
* common.h
* Các hằng số và kiểu dữ liệu chung
* 
*/

#pragma once
#include <vector>
#include <iostream>
#define BLACK 0
#define WHITE 1
#define UNDEFINED -1
#define UNKNOW -1
using MATRIX = std::vector<std::vector<int>>;
using BOX = std::pair<std::pair<int, int>, std::pair<int, int>>;
using MATRIX_BINARY = std::vector<std::vector<char>>;
using LIST_EXPECT = std::vector<char>; //danh sách dự đoán theo thứ tự ưu tiên

constexpr int STANDARD_SIZE = 32;//kích thước ma trận chuẩn
constexpr float STANDARD_RATIO = 0.5f;//tỉ lệ đúng
constexpr float STANDARD_TRUE = 0.9f;//tỉ lệ đúng
constexpr float STANDARD_RATIO_ERROR = 0.3f;//sai số cho phép

