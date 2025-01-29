#pragma once
//Các hàm tiện ích
#include <fstream>
#include <string>
#include "matrix_operations.h"

constexpr char INFORMATION_PATH[] = "../resource/infor.txt";
constexpr char MATRIX_CHARACTER_PATH[] = "../resource/matrixcharacter.txt";

std::vector<Character> readInformation(std::string path);
std::map<char, MATRIX_BINARY> readMatrixCharacter(std::string path);

//nhận diện kí tự
char identifyCharacter(const MATRIX& matrix, const LIST_EXPECT& expectFromCompare, const LIST_EXPECT& expectFromBoolean);
std::string remakeLine(const std::vector<MATRIX>& matrixs, std::vector<int> spaces);

//CÁC BIẾN KHÔNG THỂ THAY ĐỔI MÀ CHỈ ĐƯỢC TÌM KIẾM HOẶC TRUY XUẤT
const std::vector<Character> DATA_CHARACTER = readInformation(INFORMATION_PATH); //dữ liệu kí tự
const std::map<char, MATRIX_BINARY> DATA_MATRIX_CHARACTER = readMatrixCharacter(MATRIX_CHARACTER_PATH); //dữ liệu ma trận kí tự
const CharacterTree CHARACTER_TREE(DATA_CHARACTER); //cây kí tự
const ListCompare LIST_COMPARE(DATA_CHARACTER); //danh sách so sánh
//

std::string recoverText(const std::vector<MATRIX>& lines);