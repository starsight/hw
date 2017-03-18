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
    Graph graph(base.node_num, base.edge_num, base.consumer_num);//图初始化
	//graph.createG(edge);//建立基本的网络

	graph.getConsumerdata(consumer);//获取消费节点的信息

	Search search(base.node_num,6);//搜索初始化
	search.setServerInit(base.node_num);
	search.runEzSA(search.server,search.server0,consumer, graph,base,edge);

//search.start( consumer,graph, base,edge);//开始搜索


	
	string res=outputData(search. path_need,search.find_flag);//形成输出的格式
//	cout<<res.c_str()<<endl;
	// 需要输出的内容
	char * topo_file = (char *)res.c_str();

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(topo_file, filename);

}
