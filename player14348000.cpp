#include"judge.h"
#include <cstdlib>
using namespace std;

//����һ�������������� 
 
class Player14348000:public Player{
public:
	//����Ҫʵ�ֵ��麯�� 
	int askMainCard(vector<Card> cards); //�Ƿ������ 
	Card discard();	//���� 
private:
};

/*������  
  ����ÿ��һ���ƣ�ѯ�ʵ�ǰ�������Ƿ�����ƣ��������º����� ���Ƿ���Ʋ���ͬѧ���о��� 
*/ 
int Player14348000::askMainCard(vector<Card> cards){
	//��ȡ�������� 
	int level = judge->getCurrentLevel(this);

	return 0;
	/*���ͨ�����Ϸ�ʽ��ȡ��ǰ���ƣ�Ȼ������Ƿ����
	��ҽ��Ʊ�����Ϲ����ҽ��ƻᱻ���з��ϣ���Ϸ��������ܳͷ�
	��ҽ��о��ߺ󣬷���0�������ƣ�1�����ң�2�����ң�3��÷����4������ 
	*/
}

/*����
  һ��ʼ��ҿ�����Ҫ���ò��е�API����ȡ��Ϸ����Ϣ,�پ�����ô���� 
  ����API��ʱ����this�������Լ�����������ʾ 
*/
Card Player14348000::discard(){
	//��ȡ�Լ���id 
	int myId=judge->getId(this);
	//��ȡ��ʷ��¼ 
	vector<vector<pair<Card,int> > > historys=judge->getHistoryTurn(this);
	//��ȡ��ǰ���� 
	vector<Card> myCards=judge->getCurrentCards(this);
	//��ȡǰ����˳����� 
	vector<pair<Card,int> > curCards=judge->getCurrentTurn(this);
	//...(��ȡ������Ϣ)
	
	return myCards[rand() % myCards.size()];
	/*���ͨ�����Ϸ�ʽ��ȡ�����������Ϣ��������Ҫ��ȡʲô���ݲ�ͬ���߷�ʽ������
	��ҳ��Ʊ�����Ϲ����ҳ���ʱ���л������ѡ�������һ���������з���Ҫ����ƣ���Ϸ��������ܳͷ�
	��ҽ��о��ߺ󣬳�һ���� 
	*/ 		
}

