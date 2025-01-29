#include "image_processing.h"
#include <fstream>

constexpr uint16_t BMP_TYPE = 0x4D42;

bool BmpReader::read()
{
	// Mở file BMP
    std::ifstream file(filePath, std::ios::binary);
	// Kiểm tra file có mở được không
    if (!file.is_open())
    {
        std::cerr << "Cannot open BMP file: " << filePath << std::endl;
        return false;
    }

	// Đọc thông tin BMP
	parseHeaders(file);//đọc thông tin BMP
	loadPixelData(file);//đọc dữ liệu pixel
	file.close();//đóng file
    return true;
}

void BmpReader::parseHeaders(std::ifstream& file)
{
    // Đọc BMP Header
	//BMPHeader: 14 byte
    file.read(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPHeader));

    // Kiểm tra fileType phải là 'BM' (0x4D42)
    if (bmpHeader.fileType != BMP_TYPE)
    {
        std::cerr << "Not a BMP file." << std::endl;
        return;
    }

    // Đọc DIB Header
    file.read(reinterpret_cast<char*>(&dibHeader), sizeof(DIBHeader));
	width = dibHeader.width; //chiều rộng ảnh
	height = dibHeader.height; //chiều cao ảnh
	bitCount = dibHeader.bitCount; //số bit mỗi pixel
}

void BmpReader::loadPixelData(std::ifstream& file)
{
    // Tính toán kích thước dữ liệu pixel
    int rowSize = ((bitCount * width + 31) / 32) * 4;
    pixelData.resize(rowSize * height);

    // Di chuyển con trỏ file đến vị trí dữ liệu pixel
    file.seekg(bmpHeader.offsetData, std::ios::beg);
    file.read(reinterpret_cast<char*>(pixelData.data()), pixelData.size());
}

MATRIX BmpReader::convertToBinaryMatrix() 
{
	// Màu nền của ảnh
	uint8_t backgroundColor = 0;

	// Tạo ma trận nhị phân
    MATRIX binaryMatrix(height, std::vector<int>(width, 0));

	// Tính toán kích thước mỗi dòng
    // Phép cộng 31 được thêm vào để chuẩn bị cho phép làm tròn lên đến số byte gần nhất chia hết cho 4
    // Phép chia này tính số lượng "32-bit blocks" cần thiết để lưu trữ toàn bộ dòng pixel
    int rowSize = ((bitCount * width + 31) / 32) * 4;
	//chọn màu nền là màu góc trên cùng bên trái
	backgroundColor = pixelData[0];
    for (int y = 0; y < height; ++y) 
    {
        for (int x = 0; x < width; ++x) 
        {
            // Lấy giá trị pixel tại (x, y)
            int pixelIndex = y * rowSize + x * (bitCount / 8);
            uint8_t pixelValue = pixelData[pixelIndex];

            // Chuyển pixel thành giá trị nhị phân (0 hoặc 1)
            binaryMatrix[height - y - 1][x] = (pixelValue == backgroundColor) ? 0 : 1;
        }
    }

    return binaryMatrix;
}

std::vector<std::vector<int>> BmpReader::getBinaryMatrix() const
{
    return const_cast<BmpReader*>(this)->convertToBinaryMatrix();
}
