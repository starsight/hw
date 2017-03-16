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
	memset(inq, 0, sizeof(inq));
	memset(dis, 0x3f, sizeof(dis));
	memset(pre, -1, sizeof(pre));
	memset(in, 0, sizeof(in));
	dis[s] = 0; inq[s] = true;
	int fron = 0, rear = 0;
	q[rear++] = s;
	while (fron < rear) {
		int u = q[fron%MAXNODE]; fron++;
		inq[u] = false;
		for (int i = head[u]; i != -1; i = next[i]) {
			int v= edge[i].to;
			if (edge[i].cap > 0 && dis[v] > dis[u] + edge[i].cost)
			{
				dis[v] = dis[u] + edge[i].cost;
				pre[v] = u; path[v] = i;
				if (!inq[v])
				{
					in[v]++;
				//	if (in[v] > edgecount) return true;
					inq[v] = true;
					q[rear%MAXNODE] = v; rear++;

				}
			}
		}
	}
	return dis[t] < INF;

}
min_max Graph::EK(int s,int t)
{
	min_max ret={0,0};

	while (spfa(s,t)) {

		int mini = INF;
		for (int i = t; i != s; i = pre[i])
		{

			mini = min(mini, edge[path[i]].cap);

		}
	//	cout << t<<"<--";
		for (int i = t; i != s; i = pre[i]) {
		//	cout << edge[path[i]].from << "<--";
			edge[path[i]].cap -= mini;
			edge[path[i] ^ 1].cap += mini;
		}
		
	//	cout << endl;
		ret.price += mini * dis[t];
		ret.flow += mini;
	//	cout <<"MINI:"<< mini << endl;

	}
//	cout << "flow:" << ret.flow << endl;
//	cout << ret << endl;
	return ret;
}

/*****组合****************/

 Search::Search( int m,int k):wait_for_choose(m),choose_num(k),max_need(0),min_cost(INF)
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

	min_max temp={0,0};

	for(auto it=consumer.begin();it!=consumer.end();it++)
	{
		max_need+=(*it).need;
	}

	graph.getNodeDegree();
	graph.getNodeCap();

	for(int i=0;i<wait_for_choose;i++)
	{
		//server0[i]=graph.nodedegree[i].nodeID;
		server0[i]=graph.nodecap[i].nodeID;
		cout<<graph.nodedegree[i].nodeID<<" "<<graph.nodecap[i].nodeID<<endl;
	}


	int num=comb(wait_for_choose,choose_num);
	for(int i=0;i<num;i++)
	{
		cout<<"第"<<i<<"次"<<endl;

		//graph.createG(edge);
		graph.addConsumer(consumer);
		cout<<"新服务器编号: ";
		for(int j=0;j<choose_num;j++)
		{
			server[j]=server0[save[i][j]-1];
			cout<<server[j]<<" ";
		}
		cout<<endl;
		graph.addServer(server);
	//	graph.printGraph();
		temp=graph.EK(graph.node_num , graph.node_num+1);

		cout<<temp.price<<" "<<temp.flow<<endl;
		if(temp.price<min_cost&&temp.flow>=max_need)
			{
				min_cost=temp.price;
				for(int c=0;c<choose_num;c++)
					min_server[c]=server[c];

			}
		graph.removeG();
		graph.createG(edge);
	}
	min_cost+=choose_num*base.server_cost;
	cout<<"min cost:"<<min_cost<<endl;
	cout<<"server: ";
	for(int n=0;n<choose_num;n++)
		cout<<min_server[n]<<endl;
	cout<<endl;
	cout<<max_need<<endl;

}

