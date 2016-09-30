//#include "judge.h"
#include <cstring>
#include <string>
#include <algorithm>
#include <cstdlib>
using namespace std;

//这是一个玩家子类的样例 
 
namespace P14348134{
	struct CardSorter{
		int *rank;
		CardSorter(int rank[5][15]):rank(&rank[0][0]){}
		bool operator()(const Card &card1, const Card &card2){
			int r1 = rank[card1.color * 15 + card1.number];
			int r2 = rank[card2.color * 15 + card2.number];
			return r1 < r2;
		}
	};
};

class Player14348134:public Player{
public:
	const string myName = "WuKan";
	//两个要实现的虚函数 
	int askMainCard(vector<Card> cards); //是否叫主牌 
	Card discard();	//出牌 
private:
	int getCardLevel(Card card);
	bool cmpCard2(Card card1, Card card2);
	void reset();
	void SetRank(vector<Card> &myCards, vector<vector<pair<Card,int> > > & historys);
	int GetRank(Card card, int rank[5][15]);
	bool IsScore(Card card);
private:
	bool hasColor[4][4]; // [id][colors]
	int pukeRank[5][15];
	int pukeColorRank[5][15];
};

/*叫主牌  
  裁判每发一张牌，询问当前接牌者是否叫主牌（调用以下函数） ，是否叫牌策略同学自行决定 
*/ 
int Player14348134::askMainCard(vector<Card> cards){
	//获取级牌数字 
	int level = judge->getCurrentLevel(this);
	
	/*玩家通过以上方式获取当前级牌，然后决定是否叫牌
	玩家叫牌必须符合规则，乱叫牌会被裁判否认，游戏结束后接受惩罚
	玩家进行决策后，返回0：不叫牌，1：黑桃，2：红桃，3：梅花，4：方块 
	*/
	bool can[5]; 
	int nums[5];
	for (int i = 0;i < 5;++i){
		can[i] = false;
		nums[i] = 0;
	}
	for (size_t i = 0;i < cards.size(); ++i){
		Card &card = cards[i];
		if (card.number == level)can[card.color] = true;
		++nums[card.color];
	}
	int strategy = 0; // 决策
	int maxNum = 0; // 牌数
	for (int i = 1;i < 5;++i){
		if (can[i]){
			int w = nums[i]; 
			if (w >= (13-w) / 3 && w > maxNum){
				strategy = i;
				maxNum = w;
			}
		}
	}
	return strategy;
}

/*出牌
  一开始玩家可能需要调用裁判的API来获取游戏的信息,再决定怎么出牌 
  调用API的时候用this来传递自己，如下例所示 
*/
Card Player14348134::discard(){
	//获取自己的id 
	int myId = judge->getId(this);
	//获取历史记录 
	vector<vector<pair<Card,int> > > historys = judge->getHistoryTurn(this);
	if (historys.empty())reset();
	//获取当前手牌 
	vector<Card> myCards = judge->getCurrentCards(this);
	//获取前面的人出的牌 
	vector<pair<Card,int> > curCards = judge->getCurrentTurn(this);
	Card mainCard = judge->getMainCard(this);
	//...(获取其他信息)
	
	/*玩家通过以上方式获取决策所需的信息（具体需要获取什么根据不同决策方式而定）
	玩家出牌必须符合规则，乱出牌时裁判会否认其选择并随机出一张其牌组中符合要求的牌，游戏结束后接受惩罚
	玩家进行决策后，出一张牌 
	*/ 		

	//对当前牌大小排序并进行评级
	SetRank(myCards, historys);
	P14348134::CardSorter sorter(pukeRank);
	sort(myCards.begin(),myCards.end(), sorter);
	/*
	cout << "MainCard: " << endl;
	cout << mainCard.number << " - " << mainCard.color << endl;
	cout << "==========" << endl;
	for (Card c:myCards){
		cout << c.number << " - " << c.color << endl;
	}
	cout << "==========" << endl;
	*/

	vector<Card> vc; // 可以出的牌
	//GetValidCard
	bool force = false;
	if (!curCards.empty()){
		int curColor = curCards[0].first.color;
		//级牌变色
		if (curCards[0].first.number == mainCard.number)curColor = mainCard.color;
		for (size_t i = 0;i < myCards.size();++i){
			int myColor = myCards[i].color;
			if (myCards[i].number == mainCard.number){
				myColor = mainCard.color;
			}
			if (myColor == curColor){
				force = true;
				vc.push_back(myCards[i]);
			}
		}
	}

	if (!force){
		for (size_t i = 0;i < myCards.size();++i){
			vc.push_back(myCards[i]);
		}
	}


	if (curCards.empty()){
		//自己先出
		//判断出分是否最大
		//不过出分最大, 之后也会最大
		//因此出最小
		Card card = myCards[myCards.size() - 1]; 

		if (IsScore(card)){
			for (int i = myCards.size() - 2; i >= 0; --i){
				if (!IsScore(myCards[i])){
					card = myCards[i];
					break;
				}
			}
		}
		return card;
	}else{

		bool hasScore = false;
		int rank = 99;
		for (size_t i = 0;i < curCards.size(); ++i){
			pair<Card,int> p = curCards[i];
			if (IsScore(p.first)){
				hasScore = true;
			}
			int r = GetRank(p.first, pukeRank); 
			if (r < rank)rank = r;
		}	
		//有分尽量出最大的
		if (hasScore){
			Card card = vc[0];
			for (size_t i = 1; i < vc.size(); ++i){
				if (GetRank(vc[i], pukeRank) < rank){
					card = vc[i];
				}
			}
			if (GetRank(card, pukeRank) < rank){
				return card;
			}
		}
		Card card = vc[vc.size() - 1]; 
		if (IsScore(card)){
			for (int i = vc.size() - 2; i >= 0; --i){
				if (!IsScore(vc[i])){
					card = vc[i];
					break;
				}
			}
		}
		return card;
	}
	// 以防万一
	return vc[rand() % vc.size()];
}


//获取牌的级别, 数字越大代表级别越大
int Player14348134::getCardLevel(Card card){
	Card mainCard = judge->getMainCard(this);
	if (card == mainCard)return 4;
	if (card.number == mainCard.number)return 3;
	if (card.color == mainCard.color)return 2;
	return 1;
}

//card1 比 card2 先出
//若card1 大于 card2, 则返回true
bool Player14348134::cmpCard2(Card card1, Card card2){
	//大小比较
	//所叫的牌>级牌>主牌>副牌
	//内部排序: A>K>Q>J>10>...>2
	int lv1 = getCardLevel(card1);
	int lv2 = getCardLevel(card2);
	if (lv1 > lv2)return true;
	if (lv1 < lv2)return false;
	// when lv1 == lv2
	// 同级同牌, 先出者胜
	return card1.number >= card2.number;
}

void Player14348134::reset(){
	memset(hasColor, 1, sizeof(hasColor));
}


void Player14348134::SetRank(vector<Card> &myCards, vector<vector<pair<Card,int> > > & historys){
	bool used[5][15];
	memset(used, 0, sizeof(used));
	memset(pukeRank, 0, sizeof(pukeRank));
	memset(pukeColorRank, 0, sizeof(pukeColorRank));
	for (size_t i = 0;i < historys.size();++i){
		for (size_t j = 0;j < historys[i].size();++j){
			Card &card = historys[i][j].first;
			used[card.color][card.number] = true;
		}
	}
	//所叫的牌>级牌>主牌>副牌
	int rank = 1;
	Card mainCard = judge->getMainCard(this); 
	if (!used[mainCard.color][mainCard.number]){
		pukeRank[mainCard.color][mainCard.number] = rank++;
	}
	int tempRank = rank;
	for (int color = 1; color <= 4;++color){
		if (!used[color][mainCard.number]){
			pukeRank[color][mainCard.number] = tempRank;
			rank++;
		}
	}
	//主牌
	for (int number = 14;number >= 2;--number){
		if (number != mainCard.number){
			if (!used[mainCard.color][number]){
				pukeRank[mainCard.color][number] = rank++;
			}
		}
	}
	//副牌
	for (int number = 14; number >= 2;--number){
		if (number != mainCard.number){
			int tempRank = rank;
			for (int color = 1; color <= 4; ++color){
				if (color != mainCard.color){
					pukeRank[color][number] = tempRank;	
					rank++;
				}
			}
		}
	}
	//cout << "Rank " << rank << endl;
}

int Player14348134::GetRank(Card card, int rank[5][15]){
	return rank[card.color][card.number];
}

bool Player14348134::IsScore(Card card){
	return (card.number == 5 || card.number == 10 || card.number == 13);
}
