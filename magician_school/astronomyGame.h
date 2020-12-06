#pragma once
#include <bangtal>
#include <vector>
#include <algorithm>

using namespace bangtal;
using namespace std;


int ariesLines[4][2] = { {0,1},{1,2},{2,3},{3,4} };
int cancerLines[4][2] = { {0,2},{1,2},{2,3},{3,4} };
int capricornusLines[8][2] = { {1,2}, {2,3}, {3,4},{4,5},{5,6},{7,5},{7,8},{8,0} };

vector<int> ariesAdd{ 0 };
vector<int> cancerAdd{ 0 };
vector<int> capricornusAdd{ 0, 1 };

vector<int> Ans;
vector<int> Ans8;

int ariesLinesLocation[4][2] = { {484, 306},{490, 401},{652, 493},{711, 455} };
int cancerLinesLocation[4][2] = { {495, 378},{579, 378},{600, 312},{627, 250} };
int capricornusLinesLocation[8][2] = { {495, 217},{702, 224},{732, 257},{737, 403},{747, 449},{604, 342},{493, 348},{445, 348} };


int ariesLocation[5][2] = { {504, 286}, {470, 381},{632, 497},{691, 473}, {705, 443} };
int cancerLocation[5][2] = { {487, 408},{564, 470},{592, 348},{612, 295},{725, 233} };
int capricornusLocation[9][2] = { {425, 355},{482, 261},{683, 202},{723, 245},{721, 381},{735, 435},{732, 510},{594, 326},{475, 335} };


class Star;
typedef std::shared_ptr<Star> StarPtr;

class Star : public Object {
	ObjectPtr starLocation;
	int starNum;
protected:
	Star(const int _x, const int _y, const int _n, const ScenePtr scene, const std::string& image, bool shown)
		: Object(image, scene, _x, _y, shown) {
		this->starNum = _n;
	}
public:
	static StarPtr create(int _x, int _y, int _n, ScenePtr scene, const std::string& image, bool shown = true) {
		auto object = StarPtr(new Star(_x, _y, _n, scene, image, shown));
		Object::add(object);
		return object;
	}
	void clickStar(vector<int>& arr, int ans[][2], int _n);
	int getStarNum();
	void checkLines(int x, int y, int ans[][2], int _n);
};

void Star::clickStar(vector<int>& arr, int ans[][2], int _n) {

	if (find(arr.begin(), arr.end(), this->starNum) == arr.end()) {
		arr.push_back(this->starNum);
		int size = arr.size();
		for (int i = 0; i < size; i++) {
			checkLines(arr.at(i), this->starNum, ans, _n);
		}
	}
}
int Star::getStarNum() {
	return this->starNum;
}
void Star::checkLines(int x, int y, int ans[][2], int _n) {
	//cout << *ans << endl;
	for (int i = 0; i < _n; i++) {
		if (x == ans[i][0] && y == ans[i][1] || x == ans[i][1] && y == ans[i][0]) {
			_n == 8 ? Ans8.push_back(i) : Ans.push_back(i);
		}
	}


}
