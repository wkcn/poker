#include"judge.h"

using namespace std; 

//裁判的函数在这里实现

//检查游戏是否结束
bool theGameNotEnd(const vector<int> &levels) {
	for (int i = 0; i < levels.size(); ++i) 
		if (levels[i] == 14) return false;
	return true;
}

//检查牌堆v中是否有卡牌c
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

	//变量申请
	vector<bool > foul;

	//初始化一堆牌
	Card deck[DECK_LIM];
	for (int i = 0; i < DECK_LIM; ++i) {
		deck[i].number = i % 13 + 2;
		deck[i].color = i / 13 + 1;
	}

	//初始化四(n)位玩家所有信息
	for (int i = 0; i < players.size(); ++i) {
		foul.PB(false);
		levels.PB(2);
		historyTurn.PB(vector<pair<Card, int> > () );	
		handCards.PB(vector<Card> () );
		scores.PB(0);
	}

	//初始化其他变量
	level = 2; banker = -1; mainCard = Card(-1, -1);

	int cntGame = 0, startPlayer = -1;

	while (theGameNotEnd(levels)) {
		++cntGame;
	//变量初始化
		historyTurn.clear();
		for (int i = 0; i < players.size(); ++i) {
			foul[i] = false;
			scores[i] = 0;
			handCards[i].clear();
		}

	//发牌 + 叫庄阶段
		//确定级牌
		level = levels[banker];
		random_shuffle(deck, deck + DECK_LIM);	

		bool askingMainCard = true;

		for (int t = 0, p = 0; t < DECK_LIM; ++t, p = (p+1)%players.size()) {
			handCards[p].PB(deck[t]);
			if (askingMainCard) {
				int s = players[p]->askMainCard(handCards[p]);
				if (s) {
					//花色(1:黑桃,2:红桃,3:梅花,4:方块) 
					Card needCard(level, s);
					if (!haveCard(handCards[p], needCard)) {
						//记录犯规
						cout << " player " << p << " foul in askMainCard, (S)He call (" << level << ", " << s << ") but (S)He hasn't!!" << endl;
						foul[p] = true;
					}
					else {
						askingMainCard = false;
						//叫主牌成功，确定主牌
						mainCard = needCard;
						if (banker == -1) {
							banker = p;
						}
						//以叫主牌方开始
						startPlayer = p;
					}
				}
			}
		}
		cout << "fapai finish, the mainCard is " << mainCard.number << " " << mainCard.color << endl;
	//游戏运行阶段
		cout << "======================================" << endl;
		cout << "The game start" << endl;

		int turn = 0;
		for (int p = startPlayer; handCards[p].size();) {
			//第turn轮开始，创建记录
			historyTurn.PB(vector<pair<Card, int> > () );

			//轮流出牌
			for (int tim = 0; tim < players.size(); ++tim, p = (p+1)%players.size()) {
				Card nowCard = players[p]->discard();
				if (!haveCard(handCards[p], nowCard)) {
					//如果没有该手牌，犯规！
					foul[p] = true;
					//随机一张牌
					nowCard = handCards[p][0];
				}
				else {
					//没有犯规，出牌
					//nowCard = nowCard;
				}
				historyTurn[turn].PB(MP(nowCard, p));
				eraseCard(handCards[p], nowCard);
			}

			//结算该轮,计算胜者
			int winner = p = getTurnWinner(historyTurn[turn]);
			//统计出现分数和
			int sumScore = 0;
			for (int i = 0; i < historyTurn[turn].size(); ++i) {
				if (historyTurn[turn][i].first.number == 5) sumScore += 5; 
				if (historyTurn[turn][i].first.number == 10) sumScore += 10;
				if (historyTurn[turn][i].first.number == 13) sumScore += 10;
			}
			//累加得分
			scores[winner] += sumScore;

			++turn;
		}

	//结算阶段
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
			//需要为player加一个常量字符串
//			cout << "(S)He's name is " << "" << endl;
		}
	}
	
	#undef PB
	#undef INF
	#undef MP
	#undef DECK_LIM
}


//举例：玩家获取自己的编号（ID） 
int Judge::getId(Player* player){
	for(int i=0;i<4;++i)
		if(players[i]==player)
			return i;
	//error		
	return -1;
} 


/////////////////////////////////////////////////////////////////////XY part begin


/*返回历史出牌 
  返回值是一个(已经过的轮数*4)的二维向量,v[i][j]表示第i轮出的第j张牌,其中元素是pair类型,表示出的牌和出牌人的ID 
  返回值不包括当前回合的出牌 
 */
vector<vector<pair<Card,int> > > Judge::getHistoryTurn(Player* player)
{
	return historyTurn;	
}

/*返回当前回合的出牌
  向量里,元素按照出牌先后顺序排列 
 */ 
vector<pair<Card,int> > Judge::getCurrentTurn(Player* player)
{
	return currentTurn;	
}

/*返回当前手牌 
  手牌没有某种固定的顺序,有必要时玩家可以自己实现排序和分类 
 */
vector<Card> Judge::getCurrentCards(Player* player)
{
	for (int i=0;i<4;i++)
		if (players[i]==player)
			return handCards[i];	
}

/*返回各玩家拥有的分数牌 
  返回值是一个二维向量,v[i][j]表示ID为i的玩家得到的第j张分数牌 
 */ 
vector<vector<Card> > Judge::getCurrentScoreCards(Player* player)
{
	return currentScoreCard;	
}

//返回当前级牌数字(庄家的级别) 
int Judge::getCurrentLevel(Player* player)
{
	//for (int i=0;i<4;i++)
	//	if (players[i]==player)
	return level;	
}

/*返回所有玩家的分数
  v[i]表示ID为i的玩家的分数 
 */ 
vector<int> Judge::getScore(Player* player)
{
	return scores;	
}

/*返回主牌 
 */
Card Judge::getMainCard(Player* player)
{
	return mainCard;	
}
/*返回庄家 
 */
int Judge::getBanker(Player* player)
{
	return banker;	
}
/*返回所有玩家的等级
  v[i]表示ID为i的玩家的等级 
 */
vector<int> Judge::getLevel(Player* player)
{
	return levels;	
}

///////////////////////////////////////////////////XY part end


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

//编译测试用
//int main() {}
