#include"judge.cpp"
#include"player14348134.cpp"
#include"player14348000.cpp"

using namespace std;

//�ô���������Ҫʵ�ֲ��еĺ�����,���ܱ��� 
//������һ�����Ե����� 
int main(){
	//freopen("test,txt", "w", stdout);
	Player14348134 player1; //��λ��� 
	Player14348134 player2; //��λ��� 
	Player14348134 player3; //��λ��� 
	Player14348000 player4; //��λ��� 
	vector<Player*> va;
	int i;
	
	va.push_back(&player1);
	va.push_back(&player2);
	va.push_back(&player3);
	va.push_back(&player4);
		
	Judge judge(va);  //���� 
	
	for(i=0;i<4;++i)
		va[i]->setJudge(&judge); //ÿλ���Ҫ����ͬһ������ 
	
	judge.run(); //��ʼ��Ϸ 
}
