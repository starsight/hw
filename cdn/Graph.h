#pragma once
#include <vector>
#include <map>
#include<algorithm>
#include <iostream>
#include <string.h>
#include"deploy.h"
#include<memory>
#include <ctime>
#include <cstdlib>
//const int  MAXEDGE = 10000;
const int MAXNODE = 1000;
const int  INF= 0x3f3f3f3f;
#include <math.h>
struct Edge//邻接列表的信息
{
	int from;
	int to;
	int cost;
	int cap;
};
class node_degree
{
public:
	int nodeID;
	int degree;
	node_degree() :nodeID(0), degree(0) {}
	~node_degree() {}
	node_degree(int a, int b)
	{
		nodeID = a;
		degree = b;
	}
	/*******排序方式*******/
	bool operator<(const node_degree &bar)
	{
		if (this->nodeID < bar.nodeID)
		{
			return true;
		}
		else if (this->nodeID == bar.nodeID)
		{
			return this->degree < bar.degree;
		}
		return false;
	}


	bool static decrease(const node_degree &foo1, const node_degree &foo2)
	{
		if (foo1.degree > foo2.degree)
		{
			return true;
		}
		else if (foo1.degree == foo2.degree)
		{
			return foo1.nodeID > foo2.nodeID;
		}
		return false;
	}



};
struct min_max
{
	int price;
	int flow;
};

struct Path_Need
{
	vector<int> path;
	int need;
};
/*
 * 主要功能类
 */
class Graph
{
	private:
		int  edgecount;//记录边的个数
	public:
		vector<int> consumer_related_Node;//记录与消费点相连的网络节点编号
	
		int node_num, edge_num, consumer_num;//节点个数，读入文件的边数，消费节点个数
		int head[MAXNODE],next[MAX_EDGE_NUM];//邻接列表的head next
		int pre[MAXNODE], path[MAXNODE], dis[MAXNODE], q[MAXNODE], in[MAXNODE];
		bool inqueue[MAXNODE];//点是否在队列中

		Edge edge[MAX_EDGE_NUM];//记录生成边的信息
		std::vector<node_degree> nodecap;
		std::vector<node_degree> nodedegree;
		std::vector<node_degree> noderank;
		
		Graph() {}
		Graph(int node_num, int edge_num,int consumer_num);//初始化
		~Graph(){}
		void addEdge(int from,int to,int cost,int cap);//添加边
		void deleteEdge(int node);
		void createG(vector<edge_information> net);//创建基本图
		void addConsumer(vector<consumer_information> consumer);//添加消费节点的关联
		void addServer(vector<int> server);//根据服务器位置添加图的原点连接方式
		void printGraph();//打印图
		int getEdgecount();//获取生成边的个数
		void getNodeCap();//获取各个节点的容量
		void getNodeDegree();//获取各个节点的度（出度+入度）
		bool spfa(int s,int t);
		min_max EK(int s, int t,vector<Path_Need> &path_save,int max_need);//计算最小费最大流
		void removeG();//清空图
		void getConsumerdata(vector<consumer_information> consumer);//获得消费节点的有关信息
		/*****有待添加*****/
		void getRank();

};

class Search
{
	private:
	int wait_for_choose;//待选择的节点数量
	int choose_num;//服务器数量

	int a[1000];//用于组合数计算的

	vector<int> temp_save;//临时保存当前计算的组合情况
	vector<vector<int>> save;//存储所有组合情况
	int max_need;//所有消费节点的最大需求
	int min_cost;//最小费用


	public:
	vector<int>  server0,server,min_server;//服务器编号
	bool find_flag;//是否找到最优解的标志
	vector<Path_Need> path_need;//存储路径和提供的带宽
	Search(){}
	Search(int wait_for_choose,int choose_num);//初始化
	~Search(){}
	int  comb(int m,int k);//(C(m,k));
	void start(vector<consumer_information> consumer,Graph graph,base_information base,vector<edge_information> edge);
	void setServerInit(int node_num);
	int runEzSA(vector<int> &server,vector<int> server0,vector<consumer_information> consumer,Graph graph,base_information base,vector<edge_information> edge);// mo ni tui huo
	vector<int> randSever(Graph graph,vector<node_degree>& nodecap);
	double random(double start, double end);

	int weight[MAXNODE];

};
string outputData(vector<Path_Need> path_need,bool flag);//生成输出的格式

