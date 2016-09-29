#include"judge.cpp"
#include"player14348000.cpp"

using namespace std;

//该代码至少需要实现裁判的函数后,才能编译 
//以下是一个简略的例子 
int main(){
	Player14348000 players[4]; //四位玩家 
	vector<Player*> va;
	int i;
	
	for(i=0;i<4;++i) 
		va.push_back(players+i);
		
	Judge judge(va);  //裁判 
	
	for(i=0;i<4;++i)
		va[i]->setJudge(&judge); //每位玩家要设置同一个裁判 
	
	judge.run(); //开始游戏 
}
