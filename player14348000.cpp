#include"judge.h"
using namespace std;

//这是一个玩家子类的样例 
 
class Player14348000:public Player{
public:
	//两个要实现的虚函数 
	int askMainCard(vector<Card> cards); //是否叫主牌 
	Card discard();	//出牌 
private:
};

/*叫主牌  
  裁判每发一张牌，询问当前接牌者是否叫主牌（调用以下函数） ，是否叫牌策略同学自行决定 
*/ 
int Player14348000::askMainCard(vector<Card> cards){
	//获取级牌数字 
	int level = judge->getCurrentLevel(this);

	return 0;
	/*玩家通过以上方式获取当前级牌，然后决定是否叫牌
	玩家叫牌必须符合规则，乱叫牌会被裁判否认，游戏结束后接受惩罚
	玩家进行决策后，返回0：不叫牌，1：黑桃，2：红桃，3：梅花，4：方块 
	*/
}

/*出牌
  一开始玩家可能需要调用裁判的API来获取游戏的信息,再决定怎么出牌 
  调用API的时候用this来传递自己，如下例所示 
*/
Card Player14348000::discard(){
	//获取自己的id 
	int myId=judge->getId(this);
	//获取历史记录 
	vector<vector<pair<Card,int> > > historys=judge->getHistoryTurn(this);
	//获取当前手牌 
	vector<Card> myCards=judge->getCurrentCards(this);
	//获取前面的人出的牌 
	vector<pair<Card,int> > curCards=judge->getCurrentTurn(this);
	//...(获取其他信息)
	
	return *myCards.begin();
	/*玩家通过以上方式获取决策所需的信息（具体需要获取什么根据不同决策方式而定）
	玩家出牌必须符合规则，乱出牌时裁判会否认其选择并随机出一张其牌组中符合要求的牌，游戏结束后接受惩罚
	玩家进行决策后，出一张牌 
	*/ 		
}

