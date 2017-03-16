#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "lib_io.h"
#include<iostream>
#include<sstream>

#include <stdio.h>
#include<vector>


using namespace std;
struct edge_information//边的信息
{
	int source;//起始节点ID
	int target;//结束节点ID
	int cap;//总宽带大小
	int price;//网络租用费
};
struct consumer_information//消费节点的信息
{
	int consumer_ID;//消费节点ID
	int connect_ID;//与消费节点相连节点ID
	int need;//消费节点需求量
};
struct base_information//数据总体信息
{
	int node_num;//网络节点数量
	int edge_num;//网络链路数量
	int consumer_num;//消费节点数量
	int server_cost;//服务器部署成本
};
	
void deploy_server(char * graph[MAX_EDGE_NUM], int edge_num, char * filename);
/**************读取信息******************/
void readdata(char *topo[MAX_EDGE_NUM],std::vector<edge_information> &edge,std::vector<consumer_information> &cosumer,base_information *base);
#endif
