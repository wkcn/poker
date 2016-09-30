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

//���� 
struct Card;
//��� 
class Player;
//���� 
class Judge;


struct Card{
	//���캯�� 
	Card(){}
	Card(int n,int c):number(n),color(c){}
	//�ж��������Ƿ���� 
	bool operator==(const Card& c) const { return number==c.number&&color==c.color; }
	//����(����������,J��11,Q��12,K��13,A��14) 
	int number;
	//��ɫ(1:����,2:����,3:÷��,4:����) 
	int color;
};  

class Player{
public:
	//����player��Ҫ���ò��еĽӿ�����ȡ��Ϸ��Ϣ,���player��Ҫ������еľ�� 
	//���������������Ӧ�ñ��޸� 
	
	//���캯��
	Player(){}
	//���ò��� 
	void setJudge(Judge* j){judge=j;}	
public:
	//��������Ҫʵ�ֵ�������Ҫ���麯��,���ǻ�����Ϸ�����б����е���,����ȡ��ҵ��ж� 
	
	/*���ƹ�����,����ÿ��һ���Ƹ����,�ͻ���������ҵ��������
	  ����cards�ǵ�ǰ�ɸ�����ҵ�������,Ԫ�ذ�ʱ���ɷ����Ⱥ�˳��.Ҳ����˵cards�����һ��Ԫ���Ǹշ�����
	  ������ҽ����ƺ�,���в����ٵ��øú���,����ֱ�ӷ�����,������һ����. 
	*/ 
	virtual int askMainCard(vector<Card> cards)=0;
	/*
	  ���ƹ�����,ÿ�������Ҫ����ʱ,���л���������ҵ������������ȡ��ҵĳ���
	  ����������ṩ����,���Ҫ��ȡ��Ϣʱ�ɵ��ò����ṩ�Ľӿ�.
	  ����ֵ�������Ҫ������,��ȷ���������ǺϷ���.������Ϸ�,���л�����������ҳ�һ�źϷ�����. 
	*/
	virtual Card discard()=0;
protected:
	//���о�� 
	Judge* judge;	
};

//���������� 
class Judge{
public:
	//���캯��
	//����������Ӧ��ǡ����4�� 
	Judge(vector<Player*> va):players(va){}
	
	/*�������º����Ὺʼ��Ϸ,��Ϸ���й��̵Ĵ��붼������(����������õĹ��̺���) 
	  ��Ϊ�ú������ܵ��ö��,����ڿ�ʼ��Ϸǰ,Ӧreset������Ϸ��Ϣ��ص����ݳ�Ա
	  ������������ÿ����Ϸ��ʤ�ߺͼ���,�Լ�����ʤ�� 
	*/
	void run();
public:
	//�����ṩ����ҵ����нӿ�
	//��ҵ�����ЩAPI����Ҫ�ṩһ�������ָ����Ϊ����,Ŀ�����ò��и��ݲ�ͬ��������ز�ͬ���(���統ǰ����) 
	
	//�������ID(���ID�ֱ���0,1,2,3) 
	int getId(Player* player);
	
	/*������ʷ���� 
	  ����ֵ��һ��(�Ѿ���������*4)�Ķ�ά����,v[i][j]��ʾ��i�ֳ��ĵ�j����,����Ԫ����pair����,��ʾ�����ƺͳ����˵�ID 
	  ����ֵ��������ǰ�غϵĳ��� 
	*/
	vector<vector<pair<Card,int> > > getHistoryTurn(Player* player);
	
	/*���ص�ǰ�غϵĳ���
	  ������,Ԫ�ذ��ճ����Ⱥ�˳������ 
	*/ 
	vector<pair<Card,int> > getCurrentTurn(Player* player);
	
	/*���ص�ǰ���� 
	  ����û��ĳ�̶ֹ���˳��,�б�Ҫʱ��ҿ����Լ�ʵ������ͷ��� 
	*/
	vector<Card> getCurrentCards(Player* player);
	
	/*���ظ����ӵ�еķ����� 
	  ����ֵ��һ����ά����,v[i][j]��ʾIDΪi����ҵõ��ĵ�j�ŷ����� 
	*/ 
	vector<vector<Card> > getCurrentScoreCards(Player* player);
	
	//���ص�ǰ��������(ׯ�ҵļ���) 
	int getCurrentLevel(Player* player);
	
	/*����������ҵķ���
	  v[i]��ʾIDΪi����ҵķ��� 
	*/ 
	vector<int> getScore(Player* player);
	
	/*�������� 
	*/
	Card getMainCard(Player* player);
	
	/*����ׯ�� 
	*/
	int getBanker(Player* player); 
	
	/*����������ҵĵȼ�
	  v[i]��ʾIDΪi����ҵĵȼ� 
	*/
	vector<int> getLevel(Player* player);
private:	
	//��Ҿ�� 
	vector<Player*> players;
private:
	//ÿ����Ϸ��Ϣ 
	
	//ÿ��ʤ�� 
	vector<int> winners;
	//������ҵĵȼ� 
	vector<int> levels;
private:	
	//ÿ����Ϸ��Ϣ 

	//��ǰ���������ƣ�������˳�����	
	vector<pair<Card,int> > currentTurn;//
	//currentScoreCard[i][j] ��ʾ��iλ��ҵ�j�ŷ�����
	vector<vector<Card> > currentScoreCard;
	
	//��ʷ���� 
	vector<vector<pair<Card,int> > > historyTurn;
	//������ҵ����� 
	vector<vector<Card> > handCards;
	//������ҵķ��� 
	vector<int> scores;
	//�������� 
	int level;
	//���� 
	Card mainCard;
	//ׯ��ID
	int banker;
private:
	//���һ���غ�(ÿ�˳�һ����)��ʤ��
	//�������: ��ǰ�غϵĳ���˳��(������˳������), (Card, ������ID)
	//���ظûغϵ�ʤ��
	//���贫��Ĳ�����Υ��
	int getTurnWinner(vector<pair<Card,int> > vp);
	//��ȡ�Ƶļ���, ����Խ�������Խ��
	int getCardLevel(Card card);
	//card1 �� card2 �ȳ�
	//��card1 ���� card2, �򷵻�true
	bool cmpCard2(Card card1, Card card2);
	//�жϵ�ǰ�����Ƿ�Ϸ�
	bool DiscardValid(Player *player, Card card);
	//�����һ�źϷ�����
	Card DisRightCard(Player *player);
};

#endif 
