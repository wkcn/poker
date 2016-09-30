#include"judge.h"
#include <cstdlib>
using namespace std; 

//裁判的函数在这里实现

//举例：玩家获取自己的编号（ID） 
int Judge::getId(Player* player){
	for(int i=0;i<4;++i)
		if(players[i]==player)
			return i;
	//error		
	return -1;
} 

//获取牌的级别, 数字越大代表级别越大
int Judge::getCardLevel(Card card){
	if (card == mainCard)return 4;
	if (card.number == mainCard.number)return 3;
	if (card.color == mainCard.color)return 2;
	return 1;
}

//card1 比 card2 先出
//若card1 大于 card2, 则返回true
bool Judge::cmpCard2(Card card1, Card card2){
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

//获得一个回合(每人出一张牌)的胜者
//传入参数: 当前回合的出牌顺序(按出牌顺序排列), (Card, 出牌人ID)
//返回该回合的胜者
//假设传入的参数不违规
int Judge::getTurnWinner(vector<pair<Card,int> > vp){
	pair<Card,int> p = vp[0];
	Card card1 = p.first;
	int win = p.second;
	//注意, i不是出牌人的id
	for (int i = 1;i < 4;++i){
		pair<Card,int> w = vp[i];
		Card card2 = w.first;
		if (!cmpCard2(card1, card2)){
			card1 = card2;
			win = w.second;
		}
	}
	return win;
}

bool Judge::DiscardValid(Player *player, Card card){
	vector<Card> handCards = getCurrentCards(player);
	bool has = false;
	for (size_t i = 0; i < handCards.size();++i){
		if (handCards[i] == card){
			has = true;
			break;
		}
	}
	if (!has)return false;
	vector<pair<Card,int> > turnCards = getCurrentTurn(player);
	if (turnCards.empty())return true;
	Card firstCard = turnCards[0].first;
	Card mainCard = getMainCard(player);
	int curColor = firstCard.color; 
	if (firstCard.number == mainCard.number)curColor = mainCard.color;
	int myColor = card.color;
	if (card.number == mainCard.number)myColor = mainCard.color;
	if (curColor == myColor)return true;

	for (size_t i = 0; i < handCards.size();++i){
		//有这种颜色, 但是不出!
		if (handCards[i].color == curColor)return false;
	}
	return true;
}

Card Judge::DisRightCard(Player *player){
	vector<Card> handCards = getCurrentCards(player);
	vector<pair<Card,int> > turnCards = getCurrentTurn(player);
	if (turnCards.empty()){
		return handCards[rand() % handCards.size()];
	}
	vector<Card> vc;
	int curColor = turnCards[0].first.color;
	Card mainCard = getMainCard(player);
	bool force = false;
	//级牌变色
	if (turnCards[0].first.number == mainCard.number)curColor = mainCard.color;
	for (size_t i = 0;i < handCards.size();++i){
		int myColor = handCards[i].color;
		if (handCards[i].number == mainCard.number){
			myColor = mainCard.color;
		}
		if (myColor == curColor){
			force = true;
			vc.push_back(handCards[i]);
		}
	}
	if (!force){
		for (size_t i = 0;i < handCards.size();++i){
			vc.push_back(handCards[i]);
		}
	}
	return vc[rand() % vc.size()];

}
