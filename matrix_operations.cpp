#include "matrix_operations.h"
#define MINROW first.first
#define MINCOL first.second
#define MAXROW second.first
#define MAXCOL second.second

const float STANDARD_RATIO_SYM = 0.9f;
const int NOISE = 10;

//đánh dấu các pixel kết nối
static void markMatrix(MATRIX& matrix, int row, int col)
{
	std::queue<std::pair<int, int>> q;
	q.push(std::make_pair(row, col));
	matrix[row][col] = WHITE;//đánh dấu vùng kết nối thành WHITE
	while (!q.empty())
	{
		std::pair<int, int> currentPixel = q.front();
		q.pop();//lấy ra khỏi queue

		int x = currentPixel.first;
		int y = currentPixel.second;
		//kiểm tra 4 hướng xung quanh
		//nếu pixel đó là pixel đen thì đánh dấu thành WHITE
		//hướng lên
		if (x - 1 >= 0 && matrix[x - 1][y] == BLACK)
		{
			matrix[x - 1][y] = WHITE;
			q.push(std::make_pair(x - 1, y));
		}
		//hướng xuống
		if (x + 1 < matrix.size() && matrix[x + 1][y] == BLACK)
		{
			matrix[x + 1][y] = WHITE;
			q.push(std::make_pair(x + 1, y));
		}
		//hướng trái
		if (y - 1 >= 0 && matrix[x][y - 1] == BLACK)
		{
			matrix[x][y - 1] = WHITE;
			q.push(std::make_pair(x, y - 1));
		}
		//hướng phải
		if (y + 1 < matrix[0].size() && matrix[x][y + 1] == BLACK)
		{
			matrix[x][y + 1] = WHITE;
			q.push(std::make_pair(x, y + 1));
		}
	}
}


void printMatrix(MATRIX matrix)
{
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
		{
			std::cout << matrix[i][j];
		}
		std::cout << std::endl;
	}
}
/*
	0 is black, 1 is white
	hàm sẽ tìm kiếm các vùng kí tự và gán nhãn chúng theo số thứ tự
	nhãn của các vùng kết nối sẽ được gán vào mảng labels
	labels[i][j] = 0 nếu pixel (i, j) là pixel đen
	labels[i][j] = k nếu pixel (i, j) là pixel trắng và thuộc vùng kết nối thứ k
	vùng số 0 sẽ 
*/
int labelConnectedComponents(const MATRIX& binaryImage, MATRIX& labels)
{
	int rows = binaryImage.size();
	int cols = binaryImage[0].size();
	int currentLabel = 0;
	/*
	curentLabel là nhãn của vùng kết nối hiện tại
	bắt đầu từ 0 vé sẽ cộng đến vùng thứ k
	*/
	
	//duyệt qua toàn bộ ma trận ảnh
	for (int j = 0; j < cols; j++)
	{
		for (int i = 0; i < rows; i++)
		{
			/*
			gặp pixel đen thì bỏ qua
			gặp pixel trắng và chưa được gán nhãn thì bắt đầu tìm vùng kết nối
			nếu pixel đó đã tồn tại trong labels thì bỏ qua
			*/
			if (binaryImage[i][j] == WHITE && labels[i][j] == 0)  //white pixel
			{
				/*
				khi gặp pixel trắng, ta sẽ xem xét các pixel xung quanh nó
				khởi tạo một queue và đưa pixel hiện tại vào queue
				sau đó duyệt qua 8 pixel xung quanh nó
				*/
				std::queue<std::pair<int, int>> q;
				labels[i][j] = ++currentLabel; //gán nhãn cho pixel hiện tại và tăng nhãn lên 1
				q.push(std::make_pair(i, j));

				//duyệt để đảm bảo thu được toàn bộ vùng kết nối
				//kết nối thoe hàng ngang, dọc hay cả đường chéo
				while (!q.empty())
				{
					std::pair<int, int> current = q.front();
					q.pop();

					int x = current.first;
					int y = current.second;

					//check 8 neighbors
					for (int k = -1; k <= 1; k++)
					{
						for (int l = -1; l <= 1; l++)
						{
							if (k == 0 && l == 0) continue; //skip the current pixel
							if (x + k >= 0 && x + k < rows && y + l >= 0 && y + l < cols)
							{
								//nếu pixel đó là pixel trắng và chưa được gán nhãn thì gán nhãn cho nó và đưa vào queue
								if (binaryImage[x + k][y + l] == WHITE && labels[x + k][y + l] == 0)
								{
									labels[x + k][y + l] = currentLabel;
									q.push(std::make_pair(x + k, y + l));
								}
							}
						}
					}
				}
			}
		}
	}
	//trả về số lượng vùng kết nối
	return currentLabel;
}
/*
	0 is black, 1 is white
	hàm sẽ tính toán diện tích của các vùng kết nối
*/
std::vector<int> calculateComponentAreas(const MATRIX& labels, int numLabels) 
{
	std::vector<int> areas(numLabels + 1, 0); // Chỉ số từ 1 đến numLabels

	for (int i = 0; i < labels.size(); ++i)
	{
		for (int j = 0; j < labels[i].size(); ++j)
		{
			if (labels[i][j] > 0)
			{
				// tăng diện tích của vùng kết nối thứ labels[i][j] = k lên 1
				// k là nhãn của vùng kết nối
				areas[labels[i][j]]++; 
			}
		}
	}
	/*
	areas[0] không đại diện cho bất kì vùng kết nối nào mà được gán bằng 0
	ta xóa đi vùng 0
	*/

	//diện tích vùng thứ k sẽ được lưu tại vị trí k - 1 của vector
	return areas;
}
/*
    các vùng có diện tích quá nhỏ, không đáng kể sẽ bị lọc đi
	minArea là diện tích nhỏ nhất mà một vùng kết nối cần phải có
	tính minAreas bằng trung vị của các vùng kết nối
*/
MATRIX filterNoise(const MATRIX& labels, const std::vector<int>& areas)
{
	int minArea = 0; //tính minArea
	int n = areas.size() - 1; // bỏ đi vùng 0
	for (int i = 1; i <= n; i++)
	{
		minArea += areas[i];
	}
	minArea = minArea / (n + NOISE);//khử nhiễu với ngưỡng minArea


	//

	minArea = 1;

	// 





	//sao chép labels
	MATRIX filteredLabels = labels;
	for (int i = 0; i < labels.size(); ++i)
	{
		for (int j = 0; j < labels[i].size(); ++j)
		{
			int label = labels[i][j];
			if (label > 0 && areas[label] < minArea)
			{
				// Nếu diện tích của vùng có nhãn `label` nhỏ hơn ngưỡng, thì coi là nhiễu
				filteredLabels[i][j] = 0;
			}
		}
	}

	return filteredLabels;
}
//chia image thành các dòng
std::vector<MATRIX> devideLine(const MATRIX& image)
{
	std::vector<MATRIX> lines;
	std::vector<int> sumOfLine(image.size(), 0);

	//ta cộng tất cả các pixel trên mỗi hàng
	//nếu tổng là 0 thì hàng đó là hàng trắng
	for (int i = 0; i < image.size(); i++)
	{
		for (int j = 0; j < image[i].size(); j++)
		{
			sumOfLine[i] += image[i][j];
		}
	}

	bool inTheLine = false;
	int countLine = 0;
	for (int i = 0; i < sumOfLine.size(); i++)
	{
		if (sumOfLine[i] != 0 && inTheLine == false)
		{
			inTheLine = true;
			lines.push_back(MATRIX());
			countLine++;
		}
		if (inTheLine == true)
		{
			lines[countLine - 1].push_back(image[i]);
		}
		if (sumOfLine[i] == 0 && inTheLine == true)
		{
			inTheLine = false;
		}
	}

	return lines;
}
std::vector<int> getLabelsLine(const MATRIX& labels)
{
	std::vector<int> labelsLine;
	int midRow = labels.size() / 2;
	int previous = BLACK;
	for (int i = 0; i < labels[midRow].size(); i++)
	{
		if (labels[midRow][i] != 0 && labels[midRow][i] != previous)
		{
			labelsLine.push_back(labels[midRow][i]);
			previous = labels[midRow][i];
		}
	}
	return labelsLine;
}
/*
	tìm kiếm hộp bao quanh của một vùng kết nối
	đây là vùng - hình chữ nhật nhỏ nhất chứa toàn bộ vùng kết nối
*/
BOX getBoundingBox(const MATRIX& labels, int label)
{
	int minRow = UNDEFINED;	
	int maxRow = UNDEFINED;
	int minCol = UNDEFINED;
	int maxCol = UNDEFINED;

	/*
	duyệt từ trên xuống và từ dưới lên để tìm minRow và maxRow
	*/
	for (int i = 0; i < labels.size(); i++)
	{
		bool check = false;
		for (int j = 0; j < labels[i].size(); j++)
		{
			//nếu pixel đó thuộc vùng kết nối và chưa tìm được minRow thì gán minRow = i
			if (labels[i][j] == label && minRow == UNDEFINED)
			{
				minRow = i;
			}
			//khi đã tìm được minRow ta tìm maxRow
			if (labels[i][j] == label && minRow != UNDEFINED)
			{
				check = true;//hàng có nhãn
				break;
			}
		}
		//nếu không có nhãn thì maxRow là hàng trước đó
		if (check == false && minRow != UNDEFINED)
		{
			maxRow = i - 1;
			break;
		}
	}
	if (maxRow == UNDEFINED)
	{
		maxRow = labels.size() - 1; //nếu không tìm thấy maxRow thì gán maxRow = số hàng - 1
	}
	/*
	duyệt từ trái qua và từ phải qua để tìm minCol và maxCol
	*/
	for (int j = 0; j < labels[0].size(); j++)
	{
		bool check = false;
		for (int i = 0; i < labels.size(); i++)
		{
			//nếu pixel đó thuộc vùng kết nối và chưa tìm được minCol thì gán minCol = j
			if (labels[i][j] == label && minCol == UNDEFINED)
			{
				minCol = j;
			}
			//khi đã tìm được minCol ta tìm maxCol
			if (labels[i][j] == label && minCol != UNDEFINED)
			{
				check = true;//cột có nhãn
				break;
			}
		}
		//nếu không có nhãn thì maxCol là cột trước đó
		if (check == false && minCol != UNDEFINED)
		{
			maxCol = j - 1;
			break;
		}
	}
	if (maxCol == UNDEFINED)
	{
		maxCol = labels[0].size() - 1; //nếu không tìm thấy maxCol thì gán maxCol = số cột - 1
	}

	if (minRow != UNDEFINED && minCol != UNDEFINED) //chỉ cần xét một trong 2 điều kiện
	{
		// minRow  = first.first, minCol = first.second
		// maxRow = second.first, maxCol = second.second
		return BOX(std::pair<int, int>(minRow, minCol), std::pair<int, int>(maxRow, maxCol));
	}
	//nếu không tìm thấy vùng kết nối thì trả về UNDEFINED
	return BOX(std::pair<int, int>(UNDEFINED, UNDEFINED), std::pair<int, int>(UNDEFINED, UNDEFINED));
}
/*
	tìm các khoảng trắng giữ các từ
*/
std::vector<int> findSpace(const std::vector<BOX>& boudingBoxs)
{
	//tìm khoảng cách giữa các hộp
	//khoảng trắng có khoảng cách lớn hơn
	std::vector<int> spaces;
	if (boudingBoxs.size() < 2)
	{
		return spaces;
	}

	//cho khoảng cách giữa 2 kí tự đầu tiên
	int size = ( boudingBoxs[0].MAXCOL - boudingBoxs[0].MINCOL) / 2;
	for (int i = 1; i < boudingBoxs.size(); i++)
	{
		int space = boudingBoxs[i].MINCOL - boudingBoxs[i - 1].MAXCOL;
		if (space > size)
		{
			spaces.push_back(i + 1);
		}
	}

	return spaces;
}

/*
	chuyển ma trận thành ma trận con
	ma trận con là ma trận chứa các pixel của một kí tự
*/
MATRIX extractSubMatrix(const MATRIX& matrix, BOX boudingBox) 
{
	//lấy tọa độ của hộp bao quanh
	int top = boudingBox.first.first;
	int left = boudingBox.first.second;
	int bottom = boudingBox.second.first;
	int right = boudingBox.second.second;

	/*
	chuyển thành các ma trận con tương ứng với mỗi kí tự
	*/
	MATRIX subMatrix(bottom - top + 1, std::vector<int>(right - left + 1));
	for (int i = top; i <= bottom; ++i)
	{
		for (int j = left; j <= right; ++j)
		{
			subMatrix[i - top][j - left] =(matrix[i][j] != 0) ; //chuyền từ các số lớn hơn 1 thành 1
		}
	}
	//trả về ma trận con là kí tự cần xử lí
	return subMatrix;
}

/*
* clear sẽ gán nhãn các vùng kết nối và lọc nhiễu trả về ma trận đã đánh dấu
*/
MATRIX cleanMatrix(const MATRIX& binaryImage)
{
	// label là mang chứa nhãn của các vùng kết nối
	MATRIX labels(binaryImage.size(), std::vector<int>(binaryImage[0].size(), 0));

	//trả về số lượng vùng kết nối
	int numberOfConnectionAreas = labelConnectedComponents(binaryImage, labels);

	//tính toán diện tích của các vùng kết nối
	std::vector<int> areas = calculateComponentAreas(labels, numberOfConnectionAreas);
	//printMatrix(labels);
	/*
	lọc nhiễu
	và chỉ chứa nhãn của vùng kết nối sau khi lọc nhiễu
	filteredLabels là ma trận đã lọc nhiễu
	*/

	MATRIX filteredLabels = filterNoise(labels, areas);

	return filteredLabels;
}


//tính tỉ lệ dài rộng
static void calculateRatioWidthHeight(const MATRIX& matrix, Character& input)
{
	input.widthHeightRatio = static_cast<float>(matrix[0].size()) / matrix.size();
}
//tính đối xứng ngang
static void isSymmetryHorizontal(const MATRIX& matrix, Character& input)
{
	/*
	* nếu các pixel tồn tại bên trên cũng tồn tại bên dưới thì cho rằng ma trận là đối xứng ngang
	* nếu ma trận có tỉ lệ đối xứng đủ lớn ta cho rằng nó đối xứng
	*
	* ta xem như phần trên là phần chính tổng số pixel là gấp đôi phần trên
	* ta không chạy hết ma trận để kiểm tra diện tích để tiết kiệm thời gian
	* độ chênh lệch không lớn nếu thật sự đối xứng
	*/

	int midRow = (matrix.size()  + 1) / 2;
	int countPixel = 0;
	int countWhite = 0;
	for (int i = 0; i < midRow; i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
		{
			if (matrix[i][j] == BLACK)
			{
				continue;
			}
			countWhite++;//tăng số pixel trắng
			if (matrix[i][j] == matrix[matrix.size() - i - 1][j])
			{
				countPixel++;
			}
		}
	}

	//nếu tỉ lệ đối xứng đủ lớn
	if (float(countPixel) / countWhite >= STANDARD_RATIO_SYM)
	{
		input.symmetryHorizontal = true;
	}
	else
	{
		input.symmetryHorizontal = false;
	}
}
//tính đối xứng dọc
static void isSymmetryVertical(const MATRIX& matrix, Character& input)
{
	/*
	* nếu các pixel tồn tại bên trái cũng tồn tại bên phải thì cho rằng ma trận là đối xứng dọc
	* nếu ma trận có tỉ lệ đối xứng đủ lớn ta cho rằng nó đối xứng
	*
	* ta xem như phần trái là phần chính tổng số pixel là gấp đôi phần trái
	* ta không chạy hết ma trận để kiểm tra diện tích để tiết kiệm thời gian
	* độ chênh lệch không lớn nếu thật sự đối xứng
	*/

	int midCol = (matrix[0].size() + 1) / 2;
	int countPixel = 0;
	int countWhite = 0;
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < midCol; j++)
		{
			if (matrix[i][j] == BLACK)
			{
				continue;
			}
			countWhite++;//tăng số pixel trắng
			if (matrix[i][j] == matrix[i][matrix[0].size() - j - 1])
			{
				countPixel++;
			}
		}
	}

	//nếu tỉ lệ đối xứng đủ lớn
	if (float(countPixel / countWhite) >= STANDARD_RATIO_SYM)
	{
		input.symmetryVertical = true;
	}
	else
	{
		input.symmetryVertical = false;
	}
}
//tính đối xứng tâm
static void isSymmetryCenter(const MATRIX& matrix, Character& input)
{
	//nếu nó vừa đối xứng ngang vừa đối xứng dọc thì nó đối xứng tâm
	input.symmertryCenter = input.symmetryHorizontal && input.symmetryVertical;
}
//nếu phần trên có diện tích lớn hơn
static void isUpPartLarge(const MATRIX& matrix, const int& area, Character& input)
{
	/*
	* nếu phần trên có diện tích lớn hơn phần dưới thì cho rằng phần trên lớn
	*/
	int midRow = matrix.size() / 2;
	int countUp = 0;
	for (int i = 0; i < midRow; i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
		{
			if (matrix[i][j] != BLACK)
			{
				countUp++;
			}
		}
	}
	//nếu phần trên lớn hơn phần dưới
	input.upPartLarge = 2 * countUp > area;
}
//nếu phần dưới có diện tích lớn hơn
static void isDownPartLarge(const MATRIX& matrix, const int& area, Character& input)
{
	/*
	* nếu phần dưới có diện tích lớn hơn phần trên thì cho rằng phần dưới lớn
	*/
	int midRow = matrix.size() / 2;
	int countDown = 0;
	for (int i = midRow; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
		{
			if (matrix[i][j] != BLACK)
			{
				countDown++;
			}
		}
	}
	//nếu phần dưới lớn hơn phần trên
	input.downPartLarge = 2 * countDown > area;
}
//nếu phần trái có diện tích lớn hơn
static void isLeftPartLarge(const MATRIX& matrix, const int& area, Character& input)
{
	/*
	* nếu phần trái có diện tích lớn hơn phần phải thì cho rằng phần trái lớn
	*/
	int midCol = matrix[0].size() / 2;
	int countLeft = 0;
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < midCol; j++)
		{
			if (matrix[i][j] != BLACK)
			{
				countLeft++;
			}
		}
	}
	//nếu phần trái lớn hơn phần phải
	input.leftPartLarge = 2 * countLeft > area;
}
//nếu phần phải có diện tích lớn hơn
static void isRightPartLarge(const MATRIX& matrix, const int& area, Character& input)
{
	/*
	* nếu phần phải có diện tích lớn hơn phần trái thì cho rằng phần phải lớn
	*/
	int midCol = matrix[0].size() / 2;
	int countRight = 0;
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = midCol; j < matrix[i].size(); j++)
		{
			if (matrix[i][j] != BLACK)
			{
				countRight++;
			}
		}
	}
	//nếu phần phải lớn hơn phần trái
	input.rightPartLarge = 2 * countRight > area;
}
//tìm kí tự có vòng
static void isHaveCircle(const MATRIX& matrix, Character& input)
{
	/*
	* dùng DFS để tìm vòng
	* ta sẽ sao chép matrix ra một matrix mới
	* ta xem xét các biên của matrix, ta sẽ lật 4 mặt của matrix nếu chúng giữ giá trị BLACK
	* tất cả các vùng kết nối với 4 góc sẽ biens thành 1 nếu đồ thị có vòng sẽ tồn tại 1 vùng 0
	* nếu không có vòng toàn ma trận là 1
	*/
	MATRIX current = matrix;
	//xét biên trên và biên dưới
	for (int i = 0; i < matrix[0].size(); i++)
	{
		//biên trên
		if (current[0][i] == BLACK)
		{
			markMatrix(current, 0, i);
		}
		//biên dưới
		if (current[matrix.size() - 1][i] == BLACK)
		{
			markMatrix(current, matrix.size() - 1, i);
		}
	}
	//xét biên trái và biên phải
	for (int i = 0; i < matrix.size(); i++)
	{
		//biên trái
		if (current[i][0] == BLACK)
		{
			markMatrix(current, i, 0);
		}
		//biên phải
		if (current[i][matrix[0].size() - 1] == BLACK)
		{
			markMatrix(current, i, matrix[0].size() - 1);
		}
	}
	//kiểm tra vùng 0
	//nếu trong matrix có vùng 0 thì có vòng
	for (int i = 0; i < current.size(); i++)
	{
		for (int j = 0; j < current[i].size(); j++)
		{
			if (current[i][j] == BLACK)
			{
				input.haveCircle = true; //có vòng
				return;
			}
		}
	}
	input.haveCircle = false;//không có vòng
}
//xử lí thông tin ma trận
Character processingMatrix(const MATRIX& matrix)
{
	Character output;

	int area = 0;
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
		{
			if (matrix[i][j] != BLACK)
			{
				area++;
			}
		}
	}

	output.character = UNKNOW;
	calculateRatioWidthHeight(matrix, output);
	isSymmetryHorizontal(matrix, output);
	isSymmetryVertical(matrix, output);
	isSymmetryCenter(matrix, output);
	isUpPartLarge(matrix, area, output);
	isDownPartLarge(matrix, area, output);
	isLeftPartLarge(matrix, area, output);
	isRightPartLarge(matrix, area, output);
	isHaveCircle(matrix, output);
	
	return output;
}



/*
	gộp điểm ảnh, scale còn 32x32
	dựa trên kích thước chuẩn để so khớp mẫu
	ma trận đầu vào sẽ được scale về kích thước chuẩn và có padding
	scale cạnh dài nhất về 32 cạnh còn lại phụ thuộc tỉ lệ để đảm bảo tỉ lệ của kí tự không bị thay đổi
*/
MATRIX pixelAveraging(const MATRIX& matrix)
{
	MATRIX result(STANDARD_SIZE, std::vector<int>(STANDARD_SIZE, 0));// Khởi tạo tất cả giá trị bằng 0
	int width = matrix[0].size(); // Chiều rộng
	int height = matrix.size(); // Chiều cao

	// Tính toán tỷ lệ scale dựa trên cạnh lớn nhất, max sẽ scale về STANDARD_SIZE
	float scaleFactor = static_cast<double>(STANDARD_SIZE) / std::max(height, width); // Tỷ lệ scale
	int newHeight = static_cast<int>(height * scaleFactor);
	int newWidth = static_cast<int>(width * scaleFactor);

	// Tính toán padding
	int padTop = (STANDARD_SIZE - newHeight) / 2;
	int padLeft = (STANDARD_SIZE - newWidth) / 2;
	// Thực hiện scale bằng cách sao chép các giá trị
	for (int i = 0; i < newHeight; i++)
	{
		for (int j = 0; j < newWidth; j++)
		{
			// Lấy giá trị từ ma trận ban đầu theo tỷ lệ
			//nội suy tuyến tính
			int originalY = static_cast<int>(i / scaleFactor);
			int originalX = static_cast<int>(j / scaleFactor);
			result[i + padTop][j + padLeft] = matrix[originalY][originalX];
		}
	}
	
	return result;
}

float compareMatrix(const MATRIX_BINARY& data, const MATRIX& matrix)
{
	int countData = 0;
	int countMatrix = 0;
	int countEqual = 0;
	for (int i = 0; i < data.size(); i++)
	{
		for (int j = 0; j < data[i].size(); j++)
		{
			//data là mảng char không trực tiếp so sánh với int
			countData += ('0' != data[i][j]);
			countMatrix += (matrix[i][j] != BLACK);
			if (matrix[i][j] != BLACK && '0' != data[i][j])
			{
				countEqual++;
			}
		}
	}
	//tỉ lệ là trung bình cộng của số pixel giống nhau trên tổng số pixel của từng ma trận
	return static_cast<float>(countEqual * 2) / (countData + countMatrix);
}
