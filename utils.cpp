#include "utils.h"

std::vector<Character> readInformation(std::string path)
{
	std::vector<Character> dataInformation;
	std::ifstream file(path);
	//kiểm tra file mở thành công hay không
	if (!file.is_open())
	{
		std::cerr << "Cannot open file " << path << std::endl;
		return dataInformation;
	}

	//đọc dữ liệu từ file
	std::string buffer;
	std::getline(file, buffer);//bỏ qua dòng đầu tiên chứ thông tin

	//đọc dữ liệu từ file
	while (getline(file, buffer))
	{
		//dữ liệu gồn A-Z, a-z, 0-9, và các kí tự đặc biệt
		//ví dụ: A,0.714285714,0,1,0,0,1,0,0,
		Character line;
		line.character = buffer[0];
		line.widthHeightRatio = std::stof(buffer.substr(2, 7));
		bool* ptr = &line.symmetryHorizontal;
		for (int i = 9; i < buffer.length(); i++)
		{
			if (buffer[i] != ',')
			{
				*ptr = buffer[i] - '0';
				ptr++;
			}
		}
		//đọc xong 1 dòng, lưu vào vector
		dataInformation.push_back(line);
	}
	return dataInformation;
}
std::map<char, MATRIX_BINARY> readMatrixCharacter(std::string path)
{
	std::map<char, MATRIX_BINARY> characterMap;
	std::ifstream file(path);
	//kiểm tra file mở thành công hay không
	if (!file.is_open())
	{
		std::cerr << "Cannot open file " << path << std::endl;
		return characterMap;
	}

	//đọc dữ liệu từ file
	char character;
	while (file >> character)
	{
		//dữ liệu gồn A-Z, a-z, 0-9, và các kí tự đặc biệt
		std::vector<std::vector<char>> matrixCharacter(STANDARD_SIZE, std::vector<char>(STANDARD_SIZE));
		for (int i = 0; i < STANDARD_SIZE; i++)
		{
			for (int j = 0; j < STANDARD_SIZE; j++)
			{
				file >> matrixCharacter[i][j];//đọc từng phần tử của ma trận, cách nhau 1 space
			}
		}
		//đọc xong 1 kí tự, lưu vào map
		characterMap[character] = matrixCharacter;
	}
	return characterMap;
}

char identifyCharacter(
	const MATRIX& matrix, 
	const LIST_EXPECT& expectFromCompare, 
	const LIST_EXPECT& expectFromBoolean)
{
	//đọc BOOLEAN trước
	float maxRatio = 0;
	char key = UNDEFINED;
	for (int i = 0; i < expectFromBoolean.size(); i++)
	{
		if (maxRatio >= STANDARD_TRUE)
		{
			return key;
		}

		float ratioTrue = compareMatrix(DATA_MATRIX_CHARACTER.at(expectFromBoolean[i]), matrix);
		if (ratioTrue >= STANDARD_RATIO && ratioTrue > maxRatio)
		{
			maxRatio = ratioTrue;
			key = expectFromBoolean[i];
		}
	}

	//đọc COMPARE sau
	for (int i = 0; i < expectFromCompare.size(); i++)
	{
		if (maxRatio >= STANDARD_TRUE)
		{
			return key;
		}

		float ratioTrue = 0;
		try //bắt UNKNOW
		{
			ratioTrue = compareMatrix(DATA_MATRIX_CHARACTER.at(expectFromCompare[i]), matrix);
		}
		catch (const std::out_of_range& e) 
		{
			return key;
		}
		
		if (ratioTrue >= STANDARD_RATIO && ratioTrue > maxRatio)
		{
			maxRatio = ratioTrue;
			key = expectFromBoolean[i];
		}
	}
	return key;
}


/*
* Tham số matrixs là tất cả các ma trận kí tự đã được chuẩn hóa
* với mỗi kí tự hay mỗi phần tử matrix ta sẽ có 2 list_expect tương ứng
* đầu tiên chạy qua list_expect_from_boolean nếu không có kết quả thì chạy qua list_expect_from_compare
* các list_expect đã được xắp kí tự theo độ ưu tiên
* lấy các kí tự trong list, ta sẽ so sánh matrix với ma trận của kí tự đó
* nếu tỉ lệ chính xác lớn hơn hoặc bằng STANDARD_RATIO thì trả về kí tự đó
* trả về kí tự đầu tiên tìm thấy
* nếu không nó sẽ được gán là UNKNOW
*/
std::string remakeLine(const std::vector<MATRIX>& matrixs, std::vector<int> spaces)
{
	std::string result = "";
	
	//tạo các ma trận chuẩn
	std::vector<MATRIX> scaledMatrixs;
	for (int i = 0; i < matrixs.size(); i++)
	{
		scaledMatrixs.push_back(pixelAveraging(matrixs[i]));
	}
	
	int j = 0;
	for (int i = 0; i < matrixs.size(); i++)
	{
		//data dựa trên ma trận ban đầu chưa
		Character data = processingMatrix(matrixs[i]);
		
		LIST_EXPECT expectFromBoolean = CHARACTER_TREE.expect(data);
		LIST_EXPECT expectFromCompare = LIST_COMPARE.expect(data);		
		
		//so sánh dựa trên các ma trận đã chuẩn hóa
		char ch = identifyCharacter(scaledMatrixs[i], expectFromCompare, expectFromBoolean);
		if (j < spaces.size() && spaces[j] == i + 1)
		{
			result += " ";
			j++;
		}
	
		if (ch == UNKNOW)
		{
			result += '?';
		}
		else
		{
			result += ch;
		}
	}

	return result;
}

std::string recoverText(const std::vector<MATRIX>& lines)
{
	std::string text = "";
	for (int i = 0; i < lines.size(); i++)
	{
		//label là nhãn của các vùng kết nối trong line thứ i
		std::vector<int> labelsLine = getLabelsLine(lines[i]);
		/*
		tìm kiếm hộp bao quanh của các vùng kết nối
		boundingBoxes là mảng chứa các hộp bao quanh của các vùng kết nối
		boundingBoxes[i] chứa hộp bao quanh của vùng kết nối thứ i
		boundingBoxes[i].first chứa tọa độ của điểm trái trên của hộp bao quanh tọa độ điểm là <minRow, minCol>
		boundingBoxes[i].second chứa tọa độ của điểm phải dưới của hộp bao quanh tọa độ điểm là <maxRow, maxCol>
		*/
		std::vector<BOX> boundingBoxes;
		for (int j = 0; j < labelsLine.size(); j++)
		{
			// đưa từng hộp vào vector boundingBoxes
			boundingBoxes.push_back(getBoundingBox(lines[i], labelsLine[j]));
		}

		//vector lưu trữ các vị trị của khoảng trắng
		//bắt đầu từ 1, vị trí thứ i là space
		std::vector<int> spaces = findSpace(boundingBoxes);


		// subMatrices là mảng chứa các ma trận con của ma trận gốc
		// mỗi ma trận con là một vùng kết nối
		// đây là các kí tự cần được xử lí

		std::vector<MATRIX> subMatrices;
		for (int j = 0; j < boundingBoxes.size(); j++)
		{
			// đưa từng ma trận con vào vector subMatrices
			subMatrices.push_back(extractSubMatrix(lines[i], boundingBoxes[j]));
		}

		//tạo chuỗi kí tự từ các ma trận con
		std::string destinationString = remakeLine(subMatrices, spaces);
		//nối chuỗi kí tự vào chuỗi kết quả
		
		text += destinationString + "\n";
	}
	text.pop_back();//xóa dấu xuống dòng cuối cùng
	return text;
}
