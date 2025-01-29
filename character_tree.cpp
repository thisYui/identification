#include "character_tree.h"
constexpr int COUNT_BOOLEAN = 8;

static void quickSort(std::vector<Character>& characters, int low, int high)
{
	if (low >= high)
	{
		return;
	}
	int pi = low;
	for (int i = low; i < high; i++)
	{
		if (characters[i].widthHeightRatio < characters[high].widthHeightRatio)
		{
			Character temp = characters[i];
			characters[i] = characters[pi];
			characters[pi] = temp;
			pi++;
		}
	}
	Character temp = characters[high];
	characters[high] = characters[pi];
	characters[pi] = temp;

	quickSort(characters, low, pi - 1);
	quickSort(characters, pi + 1, high);
}
static Character informationOfCharacter(
	const std::vector<Character>& characters, 
	char character)
{
	for (int i = 0; i < characters.size(); i++)
	{
		if (characters[i].character == character)
		{
			return characters[i];
		}
	}
	return Character();
}
static bool checkAttribute(
	const Character& character, 
	CharacterAttribute attribute, bool condition)
{
	const bool* ptr = &character.symmetryHorizontal;
	ptr += (int)attribute - 1;
	return *ptr == condition;
}
static CharacterNode* createNode()
{
	CharacterNode* node = new CharacterNode();
	node->left = nullptr;
	node->right = nullptr;
	return node;
}
CharacterAttribute operator++(CharacterAttribute& attribute, int)
{
	CharacterAttribute oldValue = attribute;

	if (attribute == CharacterAttribute::Circle) {
		attribute = CharacterAttribute::HorizontalSymmetry;
	}
	else {
		attribute = static_cast<CharacterAttribute>(static_cast<int>(attribute) + 1);
	}

	return oldValue;
}

std::vector<Character> ListCompare::sortWidthHeightRatio(
	const std::vector<Character>& characters)
{
	//sắp xếp mảng theo tỉ lệ chiều rộng chiều cao bao gồm toàn bộ kí tự
	std::vector<Character> sortedCharacters = characters;
	//sắp xếp mảng theo tỉ lệ chiều rộng chiều cao
	quickSort(sortedCharacters, 0, sortedCharacters.size() - 1);
	//trả về mảng đã sắp xếp
	return sortedCharacters;
}

ListCompare::ListCompare(const std::vector<Character>& characters)
{
	this->listWithWidthHeightRatio = sortWidthHeightRatio(characters);
}

int ListCompare::binarySearch(float ratioOfData) const
{
	for (int i = 0; i < listWithWidthHeightRatio.size(); i++)
	{
		if (listWithWidthHeightRatio[i].widthHeightRatio >= ratioOfData)
		{
			return i;//trả về vị trí
		}
	}
	//nếu tỉ lệ không nằm trong mảng thì trả về vị trí cuối cùng
	return listWithWidthHeightRatio.size() - 1;
}

LIST_EXPECT ListCompare::expect(Character data) const 
{
	LIST_EXPECT result;
	int index = this->binarySearch(data.widthHeightRatio);
	//sai số bằng khoảng cách giữa 2 giá trị chia chia tỉ lệ giá trị
	float ratio = abs(data.widthHeightRatio - listWithWidthHeightRatio[index].widthHeightRatio)
		/ data.widthHeightRatio;
	if (ratio > STANDARD_RATIO_ERROR)
	{
		//nếu sai số lớn hơn sai số cho phép thì trả về kí tự không xác định
		result.push_back(UNKNOW);
		return result;
	}
	//nếu thõa mãn thêm vào vector
	result.push_back(listWithWidthHeightRatio[index].character);
	int i = 1;
	bool check = true;
	while (index - i >= 0 || index + i < listWithWidthHeightRatio.size() && check)
	{
		check = false; // đặt lại cờ
		if (index - i >= 0)
		{
			ratio = abs(data.widthHeightRatio - listWithWidthHeightRatio[index - i].widthHeightRatio)
				/ data.widthHeightRatio;
			if (ratio < STANDARD_RATIO_ERROR)
			{
				result.push_back(listWithWidthHeightRatio[index - i].character);
				check = true;
			}
		}
		if (index + i < listWithWidthHeightRatio.size())
		{
			ratio = abs(data.widthHeightRatio - listWithWidthHeightRatio[index + i].widthHeightRatio)
				/ data.widthHeightRatio;
			if (ratio < STANDARD_RATIO_ERROR)
			{
				result.push_back(listWithWidthHeightRatio[index + i].character);
				check = true;
			}
		}
		i++;//tăng gap size
	}
	//trả về vector
	return result;
}

CharacterTree::~CharacterTree()
{
	//hủy cây
	std::queue<CharacterNode*> q;
	q.push(root);
	while (!q.empty())
	{
		CharacterNode* current = q.front();
		q.pop();
		if (current->left != nullptr)
		{
			q.push(current->left);
		}
		if (current->right != nullptr)
		{
			q.push(current->right);
		}
		delete current;
	}
}

CharacterTree::CharacterTree(const std::vector<Character>& listCharacters)
{
	//tạo cây với root là nullptr
	root = createNode();
	std::queue<CharacterNode*> q;
	for (int i = 0; i < listCharacters.size(); i++)
	{
		root->listCharacters.push_back(listCharacters[i].character);
	}
	q.push(root);

	// tạo cây với 8 mức (không tính root) tương ứng với 8 thuộc tính
	// bắt đầu từ thuộc tính đối xứng theo chiều ngang
	CharacterAttribute currentAttribute = CharacterAttribute::HorizontalSymmetry;
	for (int i = 0; i < COUNT_BOOLEAN; i++)
	{
		int count = 1 << i;
		while (count-- > 0)
		{
			CharacterNode* current = q.front();//lấy node đầu tiên
			q.pop();//loại bỏ node đầu tiên
			current->left = createNode();//tạo node con trái
			current->right = createNode();//tạo node con phải
			for (int j = 0; j < current->listCharacters.size(); j++)
			{
				//tìm thông tin của kí tự
				Character ch = informationOfCharacter(listCharacters, current->listCharacters[j]);
				if (checkAttribute(ch, currentAttribute, true))
				{
					//nếu thuộc tính đúng thì thêm vào node con trái
					current->left->listCharacters.push_back(ch.character);
				}
				else
				{
					//nếu thuộc tính sai thì thêm vào node con phải
					current->right->listCharacters.push_back(ch.character);
				}
			}
			q.push(current->left);//thêm vào hàng đợi
			q.push(current->right);//thêm vào hàng đợi
		}
		currentAttribute++;//tăng thuộc tính => chuyển sang thuộc tính tiếp theo
	}
}

std::stack<CharacterNode*> CharacterTree::findCharacter(Character data) const
{
	//tìm kiếm kí tự trong cây
	std::stack<CharacterNode*> s;
	s.push(root);
	bool* currentAttribute = &data.symmetryHorizontal;// bắt đầu từ thuộc tính đối xứng ngang
	for (CharacterNode* current = root; current->left != nullptr; currentAttribute++)
	{
		if (*currentAttribute == true)
		{
			current = current->left;
		}
		else
		{
			current = current->right;
		}
		s.push(current);
	}

	//stack gồm các giá trị boolean
	//các node phía trên là con nút ở ngay dưới nó
	//nếu dữ liệu nhận diện sai thì pop phần tử và xem nút cha => return đến khi đúng
	return s;
}

std::vector<char> CharacterTree::characterAttribute(
	const std::vector<Character>& listCharacters, 
	CharacterAttribute attribute, 
	bool condition) const
{
	std::vector<char> orther;
	for (int i = 0; i < listCharacters.size(); i++)
	{
		/*
		tất cả các biến bool có địa chỉ cách đều 1 byte
		ptr trỏ đến địa chỉ của thuộc tính đầu tiên
		dựa vào độ lớn của attribute để tìm đến thuộc tính cần tìm
		xem *ptr có phải true không, có thì thêm vào vector
		*/
		const bool* ptr = &listCharacters[i].symmetryHorizontal;
		ptr += (int)attribute - 1;//attribute bắt đầu từ 1, xem lại enum
		if (*ptr == condition)
		{
			//nếu thuộc tính cần tìm là true thì thêm vào vector tìm kiếm
			orther.push_back(listCharacters[i].character);
		}
	}
	//trả về vector 
	return orther;
}

LIST_EXPECT CharacterTree::expect(Character data) const
{
	std::stack<CharacterNode*> s = findCharacter(data);
	//stack đảm bảo không rỗng toàn bộ

	LIST_EXPECT result;
	CharacterNode* current = s.top();
	s.pop();

	for (int i = 0; i < current->listCharacters.size(); i++)
	{
		//bậc của node thể hiện độ ưu tiên của kí tự
		result.push_back(current->listCharacters[i]);
	}

	/*
	* để không lặp lại các giá trị đã được thêm ta sẽ xét nút con của nó
	* nếu phần tử đứng trước nó nằm bên nhánh trái thì ta sẽ ghi thêm con bên phải của node cha và ngược lại
	* riêng node lá không có con nên phải ghi tách biệt
	*/

	bool* currentAttribute = &data.haveCircle;
	while (!s.empty())
	{
		current = s.top();
		s.pop();
		
		if (*currentAttribute == true)
		{
			current = current->right;
		}
		else
		{
			current = current->left;
		}
		currentAttribute--;//lùi lên node cha

		for (int i = 0; i < current->listCharacters.size(); i++)
		{
			//bậc của node thể hiện độ ưu tiên của kí tự
			result.push_back(current->listCharacters[i]);
		}
	}

	return result;
}