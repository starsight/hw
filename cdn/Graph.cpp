#include "Graph.h"


Graph::Graph(int node_num,int edge_num,int consumer_num) :edgecount(0),node_num(node_num),edge_num(edge_num),consumer_num(consumer_num)
{
	memset(head, -1, sizeof(head));
	memset(next, -1, sizeof(next));
	memset(edge, 0, sizeof(edge));

}
void Graph::addEdge(int from, int to, int cap,int cost )
{
	edge[edgecount].from = from;
	edge[edgecount].to = to;
	edge[edgecount].cap = cap;
	edge[edgecount].cost = cost;
	next[edgecount] = head[from];
	head[from] = edgecount++;
	// head--由该点可以到达的边的序号 同时通过next数组，找到所有的这样的边;边的序号越低，添加时间越早，同时也越晚被搜索到

	edge[edgecount].from = to;
	edge[edgecount].to = from;
	edge[edgecount].cap = 0;
	edge[edgecount].cost = -cost;
	next[edgecount] = head[to];
	head[to] = edgecount++;

}
void Graph::deleteEdge(int node)
{
	int k=head[node];
	head[node]=-1;
	while(k!=-1)
	{
		edge[k].from=0;
		edge[k].to=0;
		edge[k].cap=0;
		edge[k].cost=0;
		int temp =next[k];
		k=next[k];
		next[temp]=-1;
		edgecount--;
	}

}
int Graph::getEdgecount()
{
	return edgecount;
}
void Graph::getNodeCap()
{
	for (int i=0; i < node_num; i++)
	{
		node_degree temp_cap;
		int k = head[i];
		
		temp_cap.nodeID = i;
		while (k!= -1)
		{
			temp_cap.degree+= edge[k].cap;
			k = next[k];
		}
		nodecap.push_back(temp_cap);
	}
	sort(nodecap.begin(), nodecap.end(), node_degree::decrease);
	
}
void Graph::getNodeDegree()
{
	for (int i=0; i < node_num; i++)
	{
		node_degree temp_degree;
		int k = head[i];
		
		temp_degree.nodeID = i;
		while (k!= -1)
		{
			temp_degree.degree+= 1;
			k = next[k];
		}
		nodedegree.push_back(temp_degree);
	}
	sort(nodedegree.begin(), nodedegree.end(), node_degree::decrease);
}
void Graph::getRank()
{
	vector<node_degree> ().swap(noderank);
	for (int i=0; i < node_num; i++)
		{
			node_degree temp_rank;
			int k = head[i];

			temp_rank.nodeID = i;
			while (k!= -1)
			{
				temp_rank.degree+= double(edge[k].cap)/double(edge[k].cost);
				k = next[k];
			}
			noderank.push_back(temp_rank);
		}
		sort(noderank.begin(), noderank.end(), node_degree::decrease);
}
void Graph::createG(vector<edge_information> net)
{
	for (auto it = net.begin(); it != net.end(); it++)
	{
		addEdge((*it).source, (*it).target, (*it).cap,(*it).price);
		addEdge((*it).target, (*it).source,  (*it).cap,(*it).price);
	}
}
void Graph::removeG()
{
	edgecount=0;
	memset(head, -1, sizeof(head));
	memset(next, -1, sizeof(next));
	memset(edge, 0, sizeof(edge));

}
void Graph::printGraph()
{
	for (int i = 0; i < node_num+2; i++)
	{
		int k = head[i];
		while (k != -1)
		{
			cout << edge[k].from << " " << edge[k].to << " " << edge[k].cap << " " << edge[k].cost << endl;
			k = next[k];
		}
	}
}
void Graph::addConsumer(vector<consumer_information> consumer)
{
		for (int i = 0; i < consumer_num; i++)
	{
		addEdge(consumer[i].connect_ID, node_num+1,consumer[i].need,0);
	}
}
void Graph::addServer(vector<int> server)
{
	for(auto it=server.begin();it!=server.end();it++)
	{
		addEdge(node_num, *it,INF,0);
	}
}

bool Graph::spfa(int s, int t)
{
	
	memset(inqueue, 0, sizeof(inqueue));
	memset(dis, 0x3f, sizeof(dis));
	memset(pre, -1, sizeof(pre));
	memset(in, 0, sizeof(in));

	dis[s] = 0; inqueue[s] = true;
	int front = 0, rear = 0;
	q[rear++] = s;//q--存放点的队列，队列为空，循环结束
	while (front < rear) {
		int u = q[front%MAXNODE]; //循环队列
		front++;
		inqueue[u] = false;
		for (int i = head[u]; i != -1; i = next[i]) {// head--由该点可以到达的边的序号 同时通过next数组，找到所有的这样的边
			int v= edge[i].to;//第i条边的终点
			if (edge[i].cap > 0 && dis[v] > dis[u] + edge[i].cost)
			{
				dis[v] = dis[u] + edge[i].cost;
				pre[v] = u;  //pre--路径中点v的前一个点u 
				path[v] = i; //path--对于点v，我应该沿着第i路径走
				if (!inqueue[v])
				{
					in[v]++;//破负环
					if (in[v] > edgecount) 
						return true;
					inqueue[v] = true;
					q[rear%MAXNODE] = v; 
					rear++;
				}
			}
		}
	}
	return dis[t] < INF;

}
min_max Graph::EK(int s,int t,vector<Path_Need> &path_save,int max_need)
{
	vector<Path_Need> ().swap(path_save);
	min_max ret={0,0};
	Path_Need temp;
	while (spfa(s,t)) {

		int mini = INF;
		for (int i = t; i != s; i = pre[i])
		{

			mini = min(mini, edge[path[i]].cap);

		}
	//	cout << t<<"<--";
		for (int i = t; i != s; i = pre[i]) {
			//cout << edge[path[i]].from << "<--";
				temp.path.push_back( edge[path[i]].from);
			edge[path[i]].cap -= mini;
			edge[path[i] ^ 1].cap += mini;
		}
		temp.path.pop_back();
		reverse(temp.path.begin(),temp.path.end());
		int consumer_id=find(consumer_related_Node.begin(),consumer_related_Node.end(),*(temp.path.end()-1))-consumer_related_Node.begin();
				temp.path.push_back(consumer_id);
		//cout << endl;
		ret.price += mini * dis[t];
		ret.flow += mini;
		temp.need=mini;
		path_save.push_back(temp);
		vector<int> ().swap(temp.path);//clear path_temp
		temp.need=0;
	}

	if(ret.flow<max_need)
			{
				ret.price=INF;
				vector<Path_Need>().swap(path_save);
			}

			return ret;

}
void Graph::getConsumerdata(vector<consumer_information> consumer)
{
	 for (auto iter=consumer.begin();iter!=consumer.end();iter++)
	{
		 consumer_related_Node.push_back((*iter).connect_ID);
	}
}

/*****组合****************/

 Search::Search( int m,int k):wait_for_choose(m),choose_num(k),max_need(0),min_cost(INF),find_flag(false)
 {
	temp_save.resize(k);
	a[0]=k;
	server0.resize(m);
	server.resize(k);
	min_server.resize(k);
}
int  Search::comb(int m,int k)//(C(m,k))
{
	static int count;

    int i,j;


   for (i=m;i>=k;i--)
    {
        a[k]=i;
        if (k>1)
        {
            comb(i-1,k-1);
        }
        else
        {
            count++;
            for (j=a[0];j>0;j--)
            {

               temp_save[j-1]=a[j];
            }

            save.push_back(temp_save);
         //cout<<temp_save[0]<<temp_save[1]<<temp_save[2]<<endl;

        }
    }

    return count;
}
void Search::start(vector<consumer_information> consumer,Graph graph,base_information base,vector<edge_information> edge)
{

	vector<Path_Need> path_need_temp;//temp
	min_max temp={0,0};



	graph.getNodeDegree();
	graph.getNodeCap();
	graph.getRank();
	for(int i=0;i<wait_for_choose;i++)
	{
		//server0[i]=graph.nodedegree[i].nodeID;
		server0[i]=graph.nodecap[i].nodeID;
		//cout<<graph.nodedegree[i].nodeID<<" "<<graph.nodecap[i].nodeID<<" "<< graph.noderank[i].nodeID<<endl;
	}


	int num=comb(wait_for_choose,choose_num);
	for(int i=0;i<num;i++)
	{
	//	cout<<"第"<<i<<"次"<<endl;

		//graph.createG(edge);
		graph.addConsumer(consumer);
	//	cout<<"新服务器编号: ";
		for(int j=0;j<choose_num;j++)
		{
			server[j]=server0[save[i][j]-1];
			//cout<<server[j]<<" ";
		}
		//cout<<endl;
		//server={0,3,22};
		graph.addServer(server);
	//	graph.printGraph();
		temp=graph.EK(graph.node_num , graph.node_num+1,path_need_temp,max_need);
		//cout<<temp.price<<" "<<temp.flow<<endl;
		if(temp.price<min_cost)
			{
				find_flag=true;
				vector<Path_Need> ().swap(path_need);
				path_need.swap(path_need_temp);

				min_cost=temp.price;
				for(int c=0;c<choose_num;c++)
					min_server[c]=server[c];

			}
		graph.removeG();
		graph.createG(edge);
	}
	if(find_flag==true)
	{
		min_cost+=choose_num*base.server_cost;

	for(auto it=path_need.begin();it!=path_need.end();it++)
			{
				cout<<"path: ";
				for(auto it2=(*it).path.begin();it2!=(*it).path.end();it2++)
				{
					cout<<*it2<<" ";
				}
				cout<<"need: "<<(*it).need<<endl;
			}
//	cout<<"min cost:"<<min_cost<<endl;
//	cout<<"server: ";
//	for(int n=0;n<choose_num;n++)
//		cout<<min_server[n]<<" ";
//	cout<<endl;
//	cout<<"max need:"<<max_need<<endl;
	}
	else
	{
//		cout<<"NA"<<endl;
	}
}

string outputData(vector<Path_Need> path_need,bool find_flag)
{
	string res;
	char a[20];

		sprintf(a, "%d\n\n",(int)path_need.size());

	 res = a;
	 for(auto it=path_need.begin();it!=path_need.end();it++)
	 {

	 	for(auto it2=(*it).path.begin();it2!=(*it).path.end();it2++)
	 	{
	 		 sprintf(a, "%d ",*it2);
	 		res += a;
	 	}
	 	 sprintf(a, "%d",(*it).need);
	 	res += a;
	 	res += "\n";
	 }


	 return res;
}
void Search::setServerInit(int node_num)
{
	vector<int> ().swap(server0);
	for(int i=0;i<node_num;i++)
	{
		server0.push_back(i);
	}
}
double Search::random(double start, double end)
{
    return start+(end-start)*rand()/(RAND_MAX + 1.0);
}

vector<int> Search::randSever(Graph graph)
{
	vector<int> server_rand;
	vector<int> id;
	int node_num=int(graph.consumer_num);
	int node_num_temp=node_num;
	int server_num;
	for(int i=0;i<node_num;i++)
	{
				id.push_back(graph.consumer_related_Node[i]);
	}

	server_num=int(random(2,int(graph.consumer_num+1)));
	vector <int>::iterator Iter;
	for(int i=0;i<server_num;i++)
		{
			        int in=int(random(0,node_num_temp));//生成0~node_num-1随机数
			        if(in>=50)cout<<in<<endl;
			        node_num_temp--;
			        server_rand.push_back(id[in]);


			        Iter=id.begin()+in;
			        id.erase(Iter);
			// cout<<id[in]<<endl;
		}
//	for(auto it=server_rand.begin();it!=server_rand.end();it++)
	//	cout<<*it<<" ";
	//	cout<<endl;
	return server_rand;

}


int Search::runEzSA(vector<int> &server,vector<int> server0,vector<consumer_information> consumer,Graph graph,base_information base,vector<edge_information> edge)// mo ni tui huo
{
	//cout<<"start"<<endl;
	vector<Path_Need> path_need_temp;//temp
	min_max res={0,0},pre_res={0,0},new_res={0,0};
	int co=server0.size();
        int temperature=1*co;
	float zero=1e-2;
	int iter=20e2;
	vector<int> tmp_server;
	int delta_e=0;
	graph.createG(edge);

	graph.addConsumer(consumer);
	graph.addServer(server0);
	for(auto it=consumer.begin();it!=consumer.end();it++)
	{
		max_need+=(*it).need;
	}
	pre_res=graph.EK(graph.node_num , graph.node_num+1,path_need_temp,max_need);
	if(pre_res.price<INF)
		pre_res.price+=server0.size()*base.server_cost;

	while(temperature>zero)
	{

		for(int i=0;i<iter;i++)
		{

			graph.removeG();
			graph.createG(edge);
			graph.addConsumer(consumer);
			tmp_server=randSever(graph);
			graph.addServer(tmp_server);



			new_res=graph.EK(graph.node_num , graph.node_num+1,path_need_temp,max_need);
			if(new_res.price<INF)
				new_res.price+=tmp_server.size()*base.server_cost;
			//cout<<new_res.price<<endl;

			delta_e=new_res.price-pre_res.price;

			if(delta_e<0)
			{
				find_flag=true;
				vector<int> ().swap(server);
				server.swap(tmp_server);
				vector<Path_Need> ().swap(path_need);
				path_need.swap(path_need_temp);
				res=new_res;
				//cout<<res.flow<<" ";
				//cout<<res.price<<endl;
				pre_res=new_res;
			}
			else
			{
				/*if(exp(-delta_e/temperature)>rand())
				{
					vector<int> ().swap(server);
					server.swap(tmp_server);
					vector<Path_Need> ().swap(path_need);
					path_need.swap(path_need_temp);
					res=new_res;
				//	cout<<res.price<<endl;
					pre_res=new_res;
				}*/
			}
		}
		temperature=temperature*0.8;

	}
	cout<<res.price<<endl;
	//for(int n=0;n<int(server.size());n++)
		//	cout<<server[n]<<" ";
	//cout<<"end"<<endl;
	return res.price;
}

