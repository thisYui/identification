#pragma once
/*
* ta sẽ đọc dữ liệu và đưa vào vector 
* các cấu trúc dữ liệu này là từ data không thể bị thay đổi
*/

#include "common.h"
#include <map>
#include <queue>
#include <stack>

enum class CharacterAttribute
{
	WidthHeightRatio,
	HorizontalSymmetry,
	VerticalSymmetry,
	CenterSymmetry,
	UpPartLarge,
	DownPartLarge,
	LeftPartLarge,
	RightPartLarge,
	Circle
};
//nạp chồng toán tử cộng cho enum
CharacterAttribute operator++(CharacterAttribute& attribute, int);
struct Character 
{
	float widthHeightRatio; // Tỉ lệ dài rộng
	//spread gồm 4 chữ số 2 chữ số đầu là tọa độ x, 2 chữ số sau là tọa độ y
	//theo ma trận chuẩn 32x32
	//int spread; // độ phân tán
	//đặt trước char để tiết kiệm dữ liệu do struct lưu theo số byte lớn nhất
    char character;           // Ký tự
    bool symmetryHorizontal; // Đối xứng ngang
    bool symmetryVertical;   // Đối xứng dọc
	bool symmertryCenter;   // Đối xứng tâm
	bool upPartLarge;       // Phần trên lớn
	bool downPartLarge;     // Phần dưới lớn
	bool leftPartLarge;     // Phần trái lớn
	bool rightPartLarge;    // Phần phải lớn
	bool haveCircle; 		// Có vòng tròn
	// độ phân tán

    // Các hàm tạo
    Character() : character(UNKNOW), widthHeightRatio(0.0f), symmetryHorizontal(false), 
        symmetryVertical(false), symmertryCenter(false), upPartLarge(false), 
        downPartLarge(false), leftPartLarge(false), rightPartLarge(false) {}
	Character(char characterData, float widthHeightRatioData, bool symmetryHorizontalData, 
        bool symmetryVerticalData, bool symmertryCenterData, bool upPartLargeData, bool downPartLargeData, 
        bool leftPartLargeData, bool rightPartLargeData) : character(characterData), 
        widthHeightRatio(widthHeightRatioData), symmetryHorizontal(symmetryHorizontalData), 
        symmetryVertical(symmetryVerticalData), symmertryCenter(symmertryCenterData), 
        upPartLarge(upPartLargeData), downPartLarge(downPartLargeData), 
        leftPartLarge(leftPartLargeData), rightPartLarge(rightPartLargeData) {}
};
class ListCompare 
{
public:
	ListCompare(const std::vector<Character>& characters);
	// các kì vọng từ dữ liệu
	LIST_EXPECT expect(Character data) const;
private:
	std::vector<Character> listWithWidthHeightRatio;
	std::vector<Character> sortWidthHeightRatio(const std::vector<Character>& characters);
	// Hàm tìm kiếm nhị phân và trả về index
	// hàm trả về vị trí sao cho W/H[index - 1] < ratioOfData <= W/H[index]
	int binarySearch(float ratioOfData) const;
};

//cây nhị phân
struct CharacterNode
{
	std::vector<char> listCharacters;
	CharacterNode* left;
	CharacterNode* right;
	CharacterNode() : left(nullptr), right(nullptr) {};
};
class CharacterTree
{
public:
	CharacterTree(const std::vector<Character>& listCharacters);
	~CharacterTree();//hủy cây
	/*
	* từ ma trận kí tự ta sẽ xử lí dữ liệu từ các thông số đã có
	* ma trận trả về vector<char, float> với char là kí tự và float là tỉ lệ chính xác
	* đây là hàm xử lí dữ liệu chính nhưng vẫn là hàm dự đoán
	*/
	LIST_EXPECT expect(Character data) const;

private:
	/*
	* Node root là NULL chỉ có tác dụng chia dữ liệu
	* ở mỗi mức tiếp theo sẽ chia dữ liệu theo các thuộc tính khác nhau
	* bên trái là thuộc tính đúng, bên phải là thuộc tính sai
	* mức của cây sẽ tương ứng theo thứ tự của các giá trị bool trong struct Character
	* có 7 mức tương ứng với 8 thuộc tính không tính root
	* cây chỉ chứa các thuộc tính bool của character
	*
	* root gốc bao gồm tất cả kí tự
	* các node con sẽ chia dữ liệu theo các thuộc tính
	* các node char sẽ có dữ liệu là tổng 2 node con của nó
	*/
	CharacterNode* root;
	/*
	*từ enum ta sẽ xem xét số lượng cây tương ứng
	*/
	// stack trả về là là các node trong cây, phần đầu ngăn xếp là lá sau đó là các node cha của node phía trước
	std::stack<CharacterNode*> findCharacter(Character data) const;
	// trả về vector chứa các kí tự có thuộc tính tương ứng
	std::vector<char> characterAttribute(const std::vector<Character>& listCharacters, CharacterAttribute attribute, bool condition) const;
};
