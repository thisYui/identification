#pragma once
//Các hàm xử lí ảnh
#include "common.h"
#include <cstdint>

#pragma pack(push, 1) // Đảm bảo các cấu trúc được nén chính xác
/*
* phần BMP header gồm 14 byte
* bytefileType (2 byte): Xác định loại file, phải có giá trị là "BM" để chỉ định đây là file BMP.
* fileSize (4 byte): Kích thước tổng của file BMP (bao gồm cả header và dữ liệu ảnh).
* reserved1 (2 byte): Dành cho các giá trị dự phòng, thường là 0.
* reserved2 (2 byte): Dành cho các giá trị dự phòng, thường là 0.
* offsetData (4 byte): Vị trí bắt đầu của dữ liệu pixel trong file. Xác định nơi bắt đầu phần dữ liệu hình ảnh.
*/
struct BMPHeader
{
    uint16_t fileType;        // Loại file, phải là 0x4D42 (BMP)
    uint32_t fileSize;        // Kích thước file BMP
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offsetData;      // Vị trí bắt đầu dữ liệu ảnh
};
/*
* phần DIB header gồm 40 byte
* headerSize (4 byte): Kích thước của header, phải là 40 byte.
* width (4 byte): Chiều rộng của ảnh.
* height (4 byte): Chiều cao của ảnh.
* planes (2 byte): Số lượng mặt phẳng, phải là 1.
* bitCount (2 byte): Số bit mỗi pixel, phải là 1, 4, 8, 24.
* compression (4 byte): Phương thức nén, phải là 0 nếu không nén.
* imageSize (4 byte): Kích thước dữ liệu ảnh.
* xPixelsPerM (4 byte): Số pixel mỗi mét theo chiều ngang (không quan trọng).
* yPixelsPerM (4 byte): Số pixel mỗi mét theo chiều dọc (không quan trọng).
* colorsUsed (4 byte): Số màu được sử dụng.
* colorsImportant (4 byte): Số màu quan trọng.
*
* Lưu ý: Có thể có thêm các trường khác tùy thuộc vào kích thước của header.
*
*/
struct DIBHeader 
{ 
    uint32_t headerSize;      // Kích thước của header
    int32_t width;            // Chiều rộng ảnh
    int32_t height;           // Chiều cao ảnh
    uint16_t planes;          // Số plane (phải là 1)
    uint16_t bitCount;        // Số bit mỗi pixel (1, 4, 8, 24)
    uint32_t compression;     // Phương thức nén (0 = BI_RGB, 1 = BI_RLE8, ...)
    uint32_t imageSize;       // Kích thước dữ liệu ảnh
    int32_t xPixelsPerM;      // Số pixel mỗi mét (không quan trọng)
    int32_t yPixelsPerM;      // Số pixel mỗi mét (không quan trọng)
    uint32_t colorsUsed;      // Số màu được sử dụng
    uint32_t colorsImportant; // Số màu quan trọng
};

#pragma pack(pop)

class BmpReader
{
public:
    BmpReader(std::string filePath) : filePath(filePath), width(0), height(0), bitCount(0) {};
	bool read(); //đã đọc file BMP thành công hay không
	MATRIX getBinaryMatrix() const; //lấy ma trận nhị phân
	std::string getFilePath() const { return filePath; } //lấy đường dẫn file BMP

private:
    std::string filePath; //đường dẫn tới file BMP
    BMPHeader bmpHeader; //header file
    DIBHeader dibHeader; //phần nội dung
	std::vector<uint8_t> pixelData; //dữ liệu pixel
	int width; //chiều rộng ảnh
	int height; //chiều cao ảnh
	int bitCount; //số bit mỗi pixel

	void parseHeaders(std::ifstream& file); //đọc header
	void loadPixelData(std::ifstream& file); //đọc dữ liệu pixel
	MATRIX convertToBinaryMatrix(); //chuyển dữ liệu pixel thành ma trận nhị phân
};
