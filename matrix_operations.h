#pragma once
//Các hàm xử lí ma trận
#include "character_tree.h"

void printMatrix(MATRIX matrix);

//tìm kiếm vùng kết nối và gán nhãn cho các vùng kết nối
int labelConnectedComponents(const MATRIX& binaryImage, MATRIX& labels);
//tính toán diện tích của các vùng kết nối
std::vector<int> calculateComponentAreas(const MATRIX& labels, int numLabels);
//lọc nhiễu
MATRIX filterNoise(const MATRIX& labels, const std::vector<int>& areas);
//tìm kiếm hộp bao quanh của một vùng kết nối
// // chia dòng
std::vector<MATRIX> devideLine(const MATRIX& image);
//lấy các nhãn của 1 line
std::vector<int> getLabelsLine(const MATRIX& labels);
BOX getBoundingBox(const MATRIX& labels, int label);
//tìm các khoảng trắng giữ các từ
std::vector<int> findSpace(const std::vector<BOX>& boudingBox);
//chuyển từng vùng thành các ma trận con
MATRIX extractSubMatrix(const MATRIX& matrix, BOX boudingBox);
//làm sạch thông tin
MATRIX cleanMatrix(const MATRIX& binaryimage);

/*
* một số kí tự như i, l,.. có diện tích nhỏ hơn so với các kí tự khác
* một số kí tự như W, M,.. có diện tích lớn hơn so với các kí tự khác
*
* hầu hết các thông tin trước khi chuẩn hóa sẽ không còn ý nghĩa như
*  tỉ lệ diện tích
*  tỉ lệ dài / rộng
* - nhãn của các vùng kết nối
*  khi chuẩn hóa ta chỉ còn một ma trận kích thước chuẩn
* các bước dự đoán sẽ xảy ra trước khi chuẩn hóa
* ta xem xét các giá trị như
* - tỉ lệ dài/ rộng
* - đối xứng ngang, dọc, tâm
* - diện tích từng phân: một số kí tự có diện tích phần trên dưới, trái phải khác nhau
*
* xử lí các thông tin trên và lưu vào một character có char = UNKNOW
* các thông tin còn lại được định nghĩa từ ma trận
* các hàm nên là static
*/
//tính tỉ lệ dài rộng
static void calculateRatioWidthHeight(const MATRIX& matrix, Character& input);
//tính đối xứng ngang
static 
void isSymmetryHorizontal(const MATRIX& matrix, Character& input);
//tính đối xứng dọc
static 
void isSymmetryVertical(const MATRIX& matrix, Character& input);
//tính đối xứng tâm
static void isSymmetryCenter(const MATRIX& matrix, Character& input);
//nếu phần trên có diện tích lớn hơn
static void isUpPartLarge(const MATRIX& matrix, const int& area, Character& input);
//nếu phần dưới có diện tích lớn hơn
static void isDownPartLarge(const MATRIX& matrix, const int& area, Character& input);
//nếu phần trái có diện tích lớn hơn
static void isLeftPartLarge(const MATRIX& matrix, const int& area, Character& input);
//nếu phần phải có diện tích lớn hơn
static void isRightPartLarge(const MATRIX& matrix, const int& area, Character& input);
//tìm kí tự có vòng
static void isHaveCircle(const MATRIX& matrix, Character& input);
//xử lí thông tin ma trận
Character processingMatrix(const MATRIX& matrix);


//gộp điểm ảnh, scale còn 32x32
MATRIX pixelAveraging(const MATRIX& matrix);
//so sánh 2 ma trận
float compareMatrix(const MATRIX_BINARY& data, const MATRIX& matrix);