#ifndef JUDGE_H
#define JUDGE_H

#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<iomanip>
#include<map>
#include<set> 
#include<vector>
#include<deque>
#include<stack> 
#include<queue> 
#include<cstdio>
#include<cstring>
#include<cmath>
#include<ctime>
#include<cstdlib>

using namespace std;

//卡牌 
struct Card;
//玩家 
class Player;
//裁判 
class Judge;


struct Card{
	//构造函数 
	Card(){}
	Card(int n,int c):number(n),color(c){}
	//判断两张牌是否相等 
	bool operator==(const Card& c) const { return number==c.number&&color==c.color; }
	//数字(数字是数字,J是11,Q是12,K是13,A是14) 
	int number;
	//花色(1:黑桃,2:红桃,3:梅花,4:方块) 
	int color;
};  

class Player{
public:
	//由于player需要调用裁判的接口来获取游戏信息,因此player需要保存裁判的句柄 
	//下面的两个函数不应该被修改 
	
	//构造函数
	Player(){}
	//设置裁判 
	void setJudge(Judge* j){judge=j;}	
public:
	//下面是需要实现的两个重要的虚函数,它们会在游戏过程中被裁判调用,来获取玩家的行动 
	
	/*发牌过程中,裁判每发一张牌给玩家,就会调用这个玩家的这个函数
	  参数cards是当前派给该玩家的所有牌,元素按时间派发的先后顺序.也就是说cards的最后一个元素是刚发的牌
	  当有玩家叫主牌后,裁判不会再调用该函数,而是直接发完牌,进入下一环节. 
	*/ 
	virtual int askMainCard(vector<Card> cards)=0;
	/*
	  出牌过程中,每当玩家需要出牌时,裁判会调用这个玩家的这个函数来获取玩家的出牌
	  这个函数不提供参数,玩家要获取信息时可调用裁判提供的接口.
	  返回值就是玩家要出的牌,请确保出的牌是合法的.如果不合法,裁判会随便替这个玩家出一张合法的牌. 
	*/
	virtual Card discard()=0;
protected:
	//裁判句柄 
	Judge* judge;	
};

//裁判类声明 
class Judge{
public:
	//构造函数
	//参数里的玩家应该恰好有4个 
	Judge(vector<Player*> va):players(va){}
	
	/*调用以下函数会开始游戏,游戏所有过程的代码都在这里(或加上它调用的过程函数) 
	  因为该函数可能调用多次,因此在开始游戏前,应reset所有游戏信息相关的数据成员
	  函数里可以输出每场游戏的胜者和级别,以及最后的胜者 
	*/
	void run();
public:
	//裁判提供给玩家的所有接口
	//玩家调用这些API都需要提供一个自身的指针作为参数,目的是让裁判根据不同玩家来返回不同结果(比如当前手牌) 
	
	//返回玩家ID(玩家ID分别是0,1,2,3) 
	int getId(Player* player);
	
	/*返回历史出牌 
	  返回值是一个(已经过的轮数*4)的二维向量,v[i][j]表示第i轮出的第j张牌,其中元素是pair类型,表示出的牌和出牌人的ID 
	  返回值不包括当前回合的出牌 
	*/
	vector<vector<pair<Card,int> > > getHistoryTurn(Player* player);
	
	/*返回当前回合的出牌
	  向量里,元素按照出牌先后顺序排列 
	*/ 
	vector<pair<Card,int> > getCurrentTurn(Player* player);
	
	/*返回当前手牌 
	  手牌没有某种固定的顺序,有必要时玩家可以自己实现排序和分类 
	*/
	vector<Card> getCurrentCards(Player* player);
	
	/*返回各玩家拥有的分数牌 
	  返回值是一个二维向量,v[i][j]表示ID为i的玩家得到的第j张分数牌 
	*/ 
	vector<vector<Card> > getCurrentScoreCards(Player* player);
	
	//返回当前级牌数字(庄家的级别) 
	int getCurrentLevel(Player* player);
	
	/*返回所有玩家的分数
	  v[i]表示ID为i的玩家的分数 
	*/ 
	vector<int> getScore(Player* player);
	
	/*返回主牌 
	*/
	Card getMainCard(Player* player);
	
	/*返回庄家 
	*/
	int getBanker(Player* player); 
	
	/*返回所有玩家的等级
	  v[i]表示ID为i的玩家的等级 
	*/
	vector<int> getLevel(Player* player);
private:	
	//玩家句柄 
	vector<Player*> players;
private:
	//每场游戏信息 
	
	//每轮胜者 
	vector<int> winners;
	//所有玩家的等级 
	vector<int> levels;
private:	
	//每轮游戏信息 

	//当前轮所处的牌，按出牌顺序递增	
	vector<pair<Card,int> > currentTurn;//
	//currentScoreCard[i][j] 表示第i位玩家第j张分数牌
	vector<vector<Card> > currentScoreCard;
	
	//历史出牌 
	vector<vector<pair<Card,int> > > historyTurn;
	//所有玩家的手牌 
	vector<vector<Card> > handCards;
	//所有玩家的分数 
	vector<int> scores;
	//级牌数字 
	int level;
	//主牌 
	Card mainCard;
	//庄家ID
	int banker;
private:
	//获得一个回合(每人出一张牌)的胜者
	//传入参数: 当前回合的出牌顺序(按出牌顺序排列), (Card, 出牌人ID)
	//返回该回合的胜者
	//假设传入的参数不违规
	int getTurnWinner(vector<pair<Card,int> > vp);
	//获取牌的级别, 数字越大代表级别越大
	int getCardLevel(Card card);
	//card1 比 card2 先出
	//若card1 大于 card2, 则返回true
	bool cmpCard2(Card card1, Card card2);
	//判断当前出牌是否合法
	bool DiscardValid(Player *player, Card card);
	//随机出一张合法的牌
	Card DisRightCard(Player *player);
};

#endif 
