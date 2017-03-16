#include "deploy.h"
#include "Graph.h"

/*
***作用：读入数据，并将字符串转换为数字存在相应的结构中
*/
void readdata(char *topo[MAX_EDGE_NUM],std::vector<edge_information> &edge,std::vector<consumer_information> &consumer,base_information *base)
{
	std::string temp;
	std::istringstream record(*topo);
	record>>temp;
	base->node_num=stoi(temp);
	record>>temp;
	base->edge_num=stoi(temp);
	record>>temp;
	base->consumer_num=stoi(temp);
	
	std::istringstream record2(*(topo+2));
	record2>>temp;
	base->server_cost=stoi(temp);
	
		for(int i=0;i<base->edge_num;i++)
	{
		edge_information temp_edge;
		std::istringstream record3(*(topo+i+4));
		
		record3>>temp;
		temp_edge.source=stoi(temp);
		record3>>temp;
		temp_edge.target=stoi(temp);
		record3>>temp;
		temp_edge.cap=stoi(temp);
		record3>>temp;
		temp_edge.price=stoi(temp);
		edge.push_back(temp_edge);
	}
	for(int i=0;i<base->consumer_num;i++)
	{
		consumer_information temp_consumer;
		std::istringstream record4(*(topo+i+base->edge_num+5));
		record4>>temp;
		temp_consumer.consumer_ID=stoi(temp);
		record4>>temp;
		temp_consumer.connect_ID=stoi(temp);
		record4>>temp;
		temp_consumer.need=stoi(temp);
		consumer.push_back(temp_consumer);
	}
	
}
//你要完成的功能总入口
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
	base_information base;//网络基本信息
	std::vector<edge_information> edge;//边的信息
	std::vector<consumer_information> consumer;//消费节点的信息
	vector<int> server;//选定的服务器设置的节点编号
    readdata(topo,edge,consumer,&base);//读取数据
	
	/*
	 ***************创建基本的无向图**************
	定义：虚拟原点编号为节点数，虚拟汇点编号为节点数+1
	*/
    Graph graph(base.node_num, base.edge_num, base.consumer_num);
	graph.createG(edge);

	/*********选定服务器所在网络节点*************/
		//server.push_back(0);
		//server.push_back(1);
		//server.push_back(24);

	//graph.getNodeCap();
	//cout<<"cap rank:"<<endl;
	//for(auto it=graph.nodecap.begin();it!=graph.nodecap.end();it++)
	//	std::cout << (*it).nodeID << " " << (*it).degree << std::endl;
	
	//graph.getNodeDegree();
	//cout<<"degree rank:"<<endl;
	//for(auto it=graph.nodedegree.begin();it!=graph.nodedegree.end();it++)
		//std::cout << (*it).nodeID << " " << (*it).degree << std::endl;


//	graph.addServer(server);//根据选定的服务器位置添加虚拟原点
	/*********进行最大流最消费计算*************/
	/*graph.addServer(server);
	graph.printGraph();
	graph.deleteEdge(base.node_num+1);
	graph.printGraph();
	graph.addServer(server);
	graph.printGraph();*/
	start( consumer,graph, base,edge);




	/*std::cout<<"node num: "<<base.node_num<<" edge_num: "<<base.edge_num<<" consumer_num: "<<base.consumer_num<<" server_cost: "<<base.server_cost<<std::endl;
	std::cout<<"edge size is: "<<edge.size()<<std::endl;
	for(auto it = edge.begin(); it != edge.end(); it ++)
	{
		std::cout<<(*it).source<<" "<<(*it).target<<" "<<(*it).cap<<" "<<(*it).price<<std::endl;
	}
	std::cout<<"consumer size is: "<<consumer.size()<<std::endl;
	for(auto it = consumer.begin(); it != consumer.end(); it ++)
	{
		std::cout<<(*it).consumer_ID<<" "<<(*it).connect_ID<<" "<<(*it).need<<std::endl;
	}*/
	
	
	// 需要输出的内容
	char * topo_file = (char *)"17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(topo_file, filename);

}
