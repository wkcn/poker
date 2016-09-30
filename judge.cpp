#include"judge.h"

using namespace std; 

//���еĺ���������ʵ��

bool theGameNotEnd(const vector<int> &levels) {
	for (int i = 0; i < levels.size(); ++i) 
		if (levels[i] == 14) return false;
	return true;
}

void Judge::run() {
	#define PB push_back
	#define INF 0x3f3f3f3f
	#define DECK_LIM 52
	#define MP make_pair

	//��ʼ��һ����
	Card deck[DECK_LIM];
	for (int i = 0; i < DECK_LIM; ++i) {
		deck[i].number = i % 13 + 2;
		deck[i].color = i / 13 + 1;
	}

	//��ʼ����(n)λ���������Ϣ
	for (int i = 0; i < players.size(); ++i) {
		levels.PB(2);
		historyTurn.PB(vector<pair<Card, int> > () );	
		handCards.PB(vector<Card> () );
		scores.PB(0);
	}

	//��ʼ����������
	level = 2; banker = -1; mainCard = Card(-1, -1);

	int cntGame = 0, startPlayer = -1;

	while (theGameNotEnd(levels)) {
		++cntGame;
	//������ʼ��
		historyTurn.clear();
		for (int i = 0; i < players.size(); ++i) {
			scores[i] = 0;
			handCards[i].clear();
		}

	//���� + ��ׯ�׶�
		//ȷ������
		level = levels[banker];
		random_shuffle(deck, deck + DECK_LIM);	

		bool askingMainCard = true;

		for (int t = 0, p = 0; t < DECK_LIM; ++t, p = (p+1)%players.size()) {
			handCards[p].PB(deck[t]);
			if (askingMainCard) {
				int s = players[p]->askMainCard(handCards[p]);
				if (s) {
					//��ɫ(1:����,2:����,3:÷��,4:����) 
					Card needCard(level, s);
					bool haveCard = false;
					for (int i = 0; i < handCards[p].size(); ++i) 
						if (needCard == handCards[p][i]) {
							haveCard = true; break;
						}
					if (!haveCard) {
						//��¼����
						cout << " player " << p << " foul in askMainCard, (S)He call (" << level << ", " << s << ") but (S)He hasn't!!" << endl;
						scores[p] = -INF;
					}
					else {
						askingMainCard = false;
						//�����Ƴɹ���ȷ������
						mainCard = needCard;
						if (banker == -1) {
							banker = p;
						}
					}
				}
			}
		}
		cout << "fapai finish, the mainCard is " << mainCard.number << " " << mainCard.color << endl;
	//��Ϸ���н׶�
		cout << "======================================" << endl;
		cout << "The game start" << endl;
		//��һ��ׯ���ȿ�ʼ���Ժ�������Ӯ�ҿ�ʼ����ʵҲ���ǵ���ׯ�ң�
		if (startPlayer == -1) startPlayer = banker;
		int turn = 0;
		for (int p = startPlayer; handCards[p].size();) {
			historyTurn.PB(vector<pair<Card, int> > () );
			for (int tim = 0; tim < players.size(); ++tim, p = (p+1)%players.size()) {
				
			}
		}

	//����׶�
		cout << "=====================================" << endl;
		cout << "The " << cntGame << "th game finish, the result is:  " << endl;

		int maxx = -1, whoWin = -1;
		for (int i = 0; i < players.size(); ++i) {
			if (scores[i] < 0) {
				cout << "The player " << i << " was fouled, (S)he's rank will decrease. " << endl; 
				if (levels[i] > 2) --levels[i];
			}
			if (scores[i] > maxx) {
				maxx = scores[i];
				whoWin = i;
			}
			else if (scores[i] == maxx) {
				if (whoWin == banker) ;
				else if (i == banker) whoWin = i;
				else whoWin = -1;
			}
		}
		if (whoWin != -1) {
			if (whoWin == banker) levels[whoWin] ++;
			else banker = whoWin;
			startPlayer = whoWin;
			winners.PB(whoWin);
		}
		else {
			winners.PB(-1);
		//	continue;
		}
		system("pause");
	}

	for (int i = 0; i < players.size(); ++i) {
		if (levels[i] == 14) {
			cout << "The final winner is players " << i << endl;
			//��ҪΪplayer��һ�������ַ���
//			cout << "(S)He's name is " << "" << endl;
		}
	}
	
	#undef PB
	#undef INF
	#undef MP
	#undef DECK_LIM
}


//��������һ�ȡ�Լ��ı�ţ�ID�� 
int Judge::getId(Player* player){
	for(int i=0;i<4;++i)
		if(players[i]==player)
			return i;
	//error		
	return -1;
} 

//��ȡ�Ƶļ���, ����Խ���������Խ��
int Judge::getCardLevel(Card card){
	if (card == mainCard)return 4;
	if (card.number == mainCard.number)return 3;
	if (card.color == mainCard.color)return 2;
	return 1;
}

//card1 �� card2 �ȳ�
//��card1 ���� card2, �򷵻�true
bool Judge::cmpCard2(Card card1, Card card2){
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

//���һ���غ�(ÿ�˳�һ����)��ʤ��
//�������: ��ǰ�غϵĳ���˳��(������˳������), (Card, ������ID)
//���ظûغϵ�ʤ��
//���贫��Ĳ�����Υ��
int Judge::getTurnWinner(vector<pair<Card,int> > vp){
	pair<Card,int> p = vp[0];
	Card card1 = p.first;
	int win = p.second;
	//ע��, i���ǳ����˵�id
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

//���������
int main() {}