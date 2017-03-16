#pragma once
#include <vector>
#include<algorithm>
#include <iostream>
#include <string.h>
#include"deploy.h"
//const int  MAXEDGE = 10000;
const int MAXNODE = 1000;
const int  INF= 0x3f3f3f3f;

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
/*
 * 主要功能类
 */
class Graph
{
	private:
		int  edgecount;//记录边的个数

	public:
		int node_num, edge_num, consumer_num;//节点个数，读入文件的边数，消费节点个数
		int head[MAXNODE],next[MAX_EDGE_NUM];//邻接列表的head next
		int pre[MAXNODE], path[MAXNODE], dis[MAXNODE], q[MAXNODE], in[MAXNODE];
		bool inq[MAXNODE];

		Edge edge[MAX_EDGE_NUM];//记录生成边的信息
		std::vector<node_degree> nodecap;
		std::vector<node_degree> nodedegree;
		
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
		min_max EK(int s, int t);//计算最小费最大流
		void removeG();
		/*****有待添加*****/


};
void start(vector<consumer_information> consumer,Graph graph,base_information base,vector<edge_information> edge);

