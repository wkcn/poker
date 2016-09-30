//#include "judge.h"
#include <cstring>
#include <string>
#include <algorithm>
#include <cstdlib>
using namespace std;

//����һ�������������� 
 
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
	//����Ҫʵ�ֵ��麯�� 
	int askMainCard(vector<Card> cards); //�Ƿ������ 
	Card discard();	//���� 
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

/*������  
  ����ÿ��һ���ƣ�ѯ�ʵ�ǰ�������Ƿ�����ƣ��������º����� ���Ƿ���Ʋ���ͬѧ���о��� 
*/ 
int Player14348134::askMainCard(vector<Card> cards){
	//��ȡ�������� 
	int level = judge->getCurrentLevel(this);
	
	/*���ͨ�����Ϸ�ʽ��ȡ��ǰ���ƣ�Ȼ������Ƿ����
	��ҽ��Ʊ�����Ϲ����ҽ��ƻᱻ���з��ϣ���Ϸ��������ܳͷ�
	��ҽ��о��ߺ󣬷���0�������ƣ�1�����ң�2�����ң�3��÷����4������ 
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
	int strategy = 0; // ����
	int maxNum = 0; // ����
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

/*����
  һ��ʼ��ҿ�����Ҫ���ò��е�API����ȡ��Ϸ����Ϣ,�پ�����ô���� 
  ����API��ʱ����this�������Լ�����������ʾ 
*/
Card Player14348134::discard(){
	//��ȡ�Լ���id 
	int myId = judge->getId(this);
	//��ȡ��ʷ��¼ 
	vector<vector<pair<Card,int> > > historys = judge->getHistoryTurn(this);
	if (historys.empty())reset();
	//��ȡ��ǰ���� 
	vector<Card> myCards = judge->getCurrentCards(this);
	//��ȡǰ����˳����� 
	vector<pair<Card,int> > curCards = judge->getCurrentTurn(this);
	Card mainCard = judge->getMainCard(this);
	//...(��ȡ������Ϣ)
	
	/*���ͨ�����Ϸ�ʽ��ȡ�����������Ϣ��������Ҫ��ȡʲô���ݲ�ͬ���߷�ʽ������
	��ҳ��Ʊ�����Ϲ����ҳ���ʱ���л������ѡ�������һ���������з���Ҫ����ƣ���Ϸ��������ܳͷ�
	��ҽ��о��ߺ󣬳�һ���� 
	*/ 		

	//�Ե�ǰ�ƴ�С���򲢽�������
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

	vector<Card> vc; // ���Գ�����
	//GetValidCard
	bool force = false;
	if (!curCards.empty()){
		int curColor = curCards[0].first.color;
		//���Ʊ�ɫ
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
		//�Լ��ȳ�
		//�жϳ����Ƿ����
		//�����������, ֮��Ҳ�����
		//��˳���С
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
		//�з־���������
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
	// �Է���һ
	return vc[rand() % vc.size()];
}


//��ȡ�Ƶļ���, ����Խ�������Խ��
int Player14348134::getCardLevel(Card card){
	Card mainCard = judge->getMainCard(this);
	if (card == mainCard)return 4;
	if (card.number == mainCard.number)return 3;
	if (card.color == mainCard.color)return 2;
	return 1;
}

//card1 �� card2 �ȳ�
//��card1 ���� card2, �򷵻�true
bool Player14348134::cmpCard2(Card card1, Card card2){
	//��С�Ƚ�
	//���е���>����>����>����
	//�ڲ�����: A>K>Q>J>10>...>2
	int lv1 = getCardLevel(card1);
	int lv2 = getCardLevel(card2);
	if (lv1 > lv2)return true;
	if (lv1 < lv2)return false;
	// when lv1 == lv2
	// ͬ��ͬ��, �ȳ���ʤ
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
	//���е���>����>����>����
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
	//����
	for (int number = 14;number >= 2;--number){
		if (number != mainCard.number){
			if (!used[mainCard.color][number]){
				pukeRank[mainCard.color][number] = rank++;
			}
		}
	}
	//����
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
