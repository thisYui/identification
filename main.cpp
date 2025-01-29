#include "image_processing.h"
#include "utils.h"

void printCharacter(Character character)
{
    std::cout << "Character: " << character.character << std::endl;
    std::cout << "Width height ratio: " << character.widthHeightRatio << std::endl;
    std::cout << "Symmetry horizontal: " << character.symmetryHorizontal << std::endl;
    std::cout << "Symmetry vertical: " << character.symmetryVertical << std::endl;
    std::cout << "Symmetry center: " << character.symmertryCenter << std::endl;
    std::cout << "Up part large: " << character.upPartLarge << std::endl;
    std::cout << "Down part large: " << character.downPartLarge << std::endl;
    std::cout << "Left part large: " << character.leftPartLarge << std::endl;
    std::cout << "Right part large: " << character.rightPartLarge << std::endl;
    std::cout << "Have ring: " << character.haveCircle << std::endl;
    std::cout << "----------------" << std::endl;
}

//error test ijlVO
int main()
{
    while (true)
    {
        std::string imagePath = "D:/test.bmp";
        //std::cout << "Enter image path (press q to quit): ";
        //std::cin >> imagePath;

        if (imagePath == "q")
        {
            break;
        }

        // đọc ảnh từ file
        BmpReader IMAGE(imagePath);
        // kiểm tra ảnh có tồn tại không
        if (IMAGE.read() == false)
        {
            std::cerr << "Cannot open BMP file: " << IMAGE.getFilePath() << std::endl;
            return 1;
        }
        // chuyển ảnh sang ma trận nhị phân
        MATRIX binaryImage = IMAGE.getBinaryMatrix();
        // lọc nhiễu
        MATRIX matrixImage = cleanMatrix(binaryImage);

        //chia dòng
        std::vector<MATRIX> lines = devideLine(matrixImage);
        //chuyển ma trận thành text
        std::string text = recoverText(lines);
        std::cout << text; break;
    }

    return 0;
}