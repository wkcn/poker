#include"judge.h"
#include <cstdlib>

using namespace std; 

//���еĺ���������ʵ��

//�����Ϸ�Ƿ����
bool theGameNotEnd(const vector<int> &levels) {
	for (int i = 0; i < levels.size(); ++i) 
		if (levels[i] == 14) return false;
	return true;
}

//����ƶ�v���Ƿ��п���c
bool haveCard(const vector<Card> &v, const Card &c) {
	for (int i = 0; i < v.size(); ++i)
		if (v[i] == c) return true;
	return false;
}

int eraseCard(vector<Card> &v, const Card &c) {
	for (int i = 0; i < v.size(); ++i) {
		if (c == v[i]) {
			v[i] = v[(int)v.size()-1];
			v.erase(v.end()-1, v.end());		
			return 1;
		}
	}	
	return 0;
}

void Judge::run() {
	#define PB push_back
	#define INF 0x3f3f3f3f
	#define DECK_LIM 52
	#define MP make_pair

	//��������
	vector<bool > foul;

	//��ʼ��һ����
	Card deck[DECK_LIM];
	for (int i = 0; i < DECK_LIM; ++i) {
		deck[i].number = i % 13 + 2;
		deck[i].color = i / 13 + 1;
	}

	//��ʼ����(n)λ���������Ϣ
	for (int i = 0; i < players.size(); ++i) {
		foul.PB(false);
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
			foul[i] = false;
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
					if (!haveCard(handCards[p], needCard)) {
						//��¼����
						cout << " player " << p << " foul in askMainCard, (S)He call (" << level << ", " << s << ") but (S)He hasn't!!" << endl;
						foul[p] = true;
					}
					else {
						askingMainCard = false;
						//�����Ƴɹ���ȷ������
						mainCard = needCard;
						if (banker == -1) {
							banker = p;
						}
						//�Խ����Ʒ���ʼ
						startPlayer = p;
					}
				}
			}
		}
		cout << "fapai finish, the mainCard is " << mainCard.number << " " << mainCard.color << endl;
	//��Ϸ���н׶�
		cout << "======================================" << endl;
		cout << "The game start" << endl;

		int turn = 0;
		for (int p = startPlayer; handCards[p].size();) {
			//��turn�ֿ�ʼ��������¼
			historyTurn.PB(vector<pair<Card, int> > () );

			//��������
			for (int tim = 0; tim < players.size(); ++tim, p = (p+1)%players.size()) {
				Card nowCard = players[p]->discard();
				if (!haveCard(handCards[p], nowCard)) {
					//���û�и����ƣ����棡
					foul[p] = true;
					//���һ����
					nowCard = handCards[p][0];
				}
				else {
					//û�з��棬����
					//nowCard = nowCard;
				}
				historyTurn[turn].PB(MP(nowCard, p));
				eraseCard(handCards[p], nowCard);
			}

			//�������,����ʤ��
			int winner = p = getTurnWinner(historyTurn[turn]);
			//ͳ�Ƴ��ַ�����
			int sumScore = 0;
			for (int i = 0; i < historyTurn[turn].size(); ++i) {
				if (historyTurn[turn][i].first.number == 5) sumScore += 5; 
				if (historyTurn[turn][i].first.number == 10) sumScore += 10;
				if (historyTurn[turn][i].first.number == 13) sumScore += 10;
			}
			//�ۼӵ÷�
			scores[winner] += sumScore;

			++turn;
		}

	//����׶�
		cout << "=====================================" << endl;
		cout << "The " << cntGame << "th game finish, the result is:  " << endl;

		int maxx = -1, whoWin = -1;
		for (int i = 0; i < players.size(); ++i) {
			if (foul[i]) {
				cout << "The player " << i << " was fouled, (S)he's rank will decrease. " << endl; 
				if (levels[i] > 2) --levels[i];
			}
			else {
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


/////////////////////////////////////////////////////////////////////XY part begin


/*������ʷ���� 
  ����ֵ��һ��(�Ѿ���������*4)�Ķ�ά����,v[i][j]��ʾ��i�ֳ��ĵ�j����,����Ԫ����pair����,��ʾ�����ƺͳ����˵�ID 
  ����ֵ��������ǰ�غϵĳ��� 
 */
vector<vector<pair<Card,int> > > Judge::getHistoryTurn(Player* player)
{
	return historyTurn;	
}

/*���ص�ǰ�غϵĳ���
  ������,Ԫ�ذ��ճ����Ⱥ�˳������ 
 */ 
vector<pair<Card,int> > Judge::getCurrentTurn(Player* player)
{
	return currentTurn;	
}

/*���ص�ǰ���� 
  ����û��ĳ�̶ֹ���˳��,�б�Ҫʱ��ҿ����Լ�ʵ������ͷ��� 
 */
vector<Card> Judge::getCurrentCards(Player* player)
{
	for (int i=0;i<4;i++)
		if (players[i]==player)
			return handCards[i];	
}

/*���ظ����ӵ�еķ����� 
  ����ֵ��һ����ά����,v[i][j]��ʾIDΪi����ҵõ��ĵ�j�ŷ����� 
 */ 
vector<vector<Card> > Judge::getCurrentScoreCards(Player* player)
{
	return currentScoreCard;	
}

//���ص�ǰ��������(ׯ�ҵļ���) 
int Judge::getCurrentLevel(Player* player)
{
	//for (int i=0;i<4;i++)
	//	if (players[i]==player)
	return level;	
}

/*����������ҵķ���
  v[i]��ʾIDΪi����ҵķ��� 
 */ 
vector<int> Judge::getScore(Player* player)
{
	return scores;	
}

/*�������� 
 */
Card Judge::getMainCard(Player* player)
{
	return mainCard;	
}
/*����ׯ�� 
 */
int Judge::getBanker(Player* player)
{
	return banker;	
}
/*����������ҵĵȼ�
  v[i]��ʾIDΪi����ҵĵȼ� 
 */
vector<int> Judge::getLevel(Player* player)
{
	return levels;	
}

///////////////////////////////////////////////////XY part end


//��ȡ�Ƶļ���, ����Խ�������Խ��
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
		//��������ɫ, ���ǲ���!
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
	//���Ʊ�ɫ
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
//���������
//int main() {}
