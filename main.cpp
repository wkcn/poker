#include"judge.cpp"
#include"player14348134.cpp"
#include"player14348000.cpp"

using namespace std;

//该代码至少需要实现裁判的函数后,才能编译 
//以下是一个简略的例子 
int main(){
	//freopen("test,txt", "w", stdout);
	Player14348134 player1; //四位玩家 
	Player14348134 player2; //四位玩家 
	Player14348134 player3; //四位玩家 
	Player14348000 player4; //四位玩家 
	vector<Player*> va;
	int i;
	
	va.push_back(&player1);
	va.push_back(&player2);
	va.push_back(&player3);
	va.push_back(&player4);
		
	Judge judge(va);  //裁判 
	
	for(i=0;i<4;++i)
		va[i]->setJudge(&judge); //每位玩家要设置同一个裁判 
	
	judge.run(); //开始游戏 
}
