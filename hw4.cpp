//============================================================================
// Name        : hw4.cpp
// Author      : Ömer Yavuz
// Version     : ceng315
// Copyright   : Your copyright notice
// Description : Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
#include <limits.h>
#include <queue>
using namespace std;


class Edge{
public:
	int from,to,previous,id;
	double capacity,flow;
	bool visited;
	void print(){
		cout << "( " << id << " ) "; 
		cout << from << " " << to << " " << capacity << " " << flow  ;
	}
	bool is_avaliable(){
		if(capacity > flow){
			return true;
		}
		else{
			return false;
		}
	};
	
};
class Vertex{
public:
	vector< int > adjacents;
	int id,parent;
	bool visited;

	
};

vector<int> DFS(int start,int end,vector< Vertex >& vertex_list, vector< Edge > &edge_list){
	stack< Edge > s;
	Edge tmp ;
	vector<int > dfs_path;
	bool path = false;
	for(unsigned int i=0;i<edge_list.size();i++){
		edge_list[i].visited = false;
	}
	for(unsigned int i=0;i<vertex_list[start].adjacents.size();i++){
		int x = vertex_list[start].adjacents[i];
		if(edge_list[x].is_avaliable()){
			path = true;
			edge_list[x].previous = -1;
			s.push(edge_list[x]);
		}
	}
	if(path == false){
		return dfs_path;
	}
	path = false;
	int last_edge = -1;
	while(!s.empty()){
		tmp = s.top();
		s.pop();
		if(tmp.to == end){
			path = true;
			last_edge = tmp.id;
			break;
		}
		edge_list[tmp.id].visited = true;
		if(tmp.id %2 == 0){
			edge_list[tmp.id+1].visited = true;
		}
		else{
			edge_list[tmp.id-1].visited = true;
		}
		for(unsigned int i=0;i<vertex_list[tmp.to].adjacents.size();i++){
			int pos = vertex_list[tmp.to].adjacents[i];
			if(edge_list[pos].is_avaliable() && !edge_list[pos].visited){
				edge_list[pos].previous = tmp.id;					
				s.push(edge_list[pos]);
			}
		}
	}
	if(last_edge != -1){
		while(last_edge != -1) {
			dfs_path.push_back(last_edge);
			last_edge = edge_list[last_edge].previous;
		}
	}
	return dfs_path;
}
vector<int> BFS(int start,int end,vector< Vertex >& vertex_list, vector< Edge > &edge_list){
	queue< Edge > q;
	Edge tmp ;
	vector<int > dfs_path;
	bool path = false;
	for(unsigned int i=0;i<edge_list.size();i++){
		edge_list[i].visited = false;
	}
	for(unsigned int i=0;i<vertex_list[start].adjacents.size();i++){
		int x = vertex_list[start].adjacents[i];
		if(edge_list[x].is_avaliable()){
			path = true;
			edge_list[x].previous = -1;
			q.push(edge_list[x]);
			
		}
	}
	if(path == false){
		return dfs_path;
	}
	path = false;
	int last_edge = -1;
	while(!q.empty()){
		tmp = q.front();
		q.pop();
		if(tmp.to == end){
			path = true;
			last_edge = tmp.id;
			break;
		}
		edge_list[tmp.id].visited = true;
		if(tmp.id %2 == 0){
			edge_list[tmp.id+1].visited = true;
		}
		else{
			edge_list[tmp.id-1].visited = true;
		}
		for(unsigned int i=0;i<vertex_list[tmp.to].adjacents.size();i++){
			int pos = vertex_list[tmp.to].adjacents[i];
			if(edge_list[pos].is_avaliable() && !edge_list[pos].visited){
				edge_list[pos].previous = tmp.id;					
				q.push(edge_list[pos]);
			}
		}
	}
	if(last_edge != -1){
		while(last_edge != -1) {
			dfs_path.push_back(last_edge);
			last_edge = edge_list[last_edge].previous;
		}
	}
	return dfs_path;
}
double Ford_Fulkerson(int source,int destination,vector<Vertex>& vertex_list,vector<Edge>& edge_list){
	double max_flow = 0;
	for(unsigned int i=0;i<edge_list.size();i++){
		edge_list[i].flow = 0;
	}
	vector<int> path = BFS(source,destination,vertex_list,edge_list);
	double min ;
	while(	path.size() != 0){
		min = edge_list[path[0]].capacity - edge_list[path[0]].flow; 
		for(unsigned int i=1;i<path.size();i++){
			if(edge_list[path[i]].capacity - edge_list[path[i]].flow  < min){
				min = edge_list[path[i]].capacity - edge_list[path[i]].flow;
			}
		}
		for(unsigned int i=0;i<path.size();i++){
			if(path[i] % 2 == 0){
				edge_list[path[i]].flow += min;
				edge_list[path[i]+1].flow -= min;
			}
			else{
				edge_list[path[i]].flow += min;
				edge_list[path[i]-1].flow -= min;
			}
		}
		path = BFS(source,destination,vertex_list,edge_list);
	}

	for(unsigned int i=0;i<vertex_list[0].adjacents.size();i++){
		max_flow += edge_list[vertex_list[0].adjacents[i]].flow;
	}
	
	return max_flow;
}
void add_edge_vertex(int from,int to,int& edge_id,vector<Vertex> & vertex_list,vector<Edge>& edge_list){
	Edge edge;
	edge.from = from;
	edge.to = to;
	edge.capacity = INT_MAX;
	edge.flow = 0;
	edge.id = edge_id;
	edge_list.push_back(edge);
	vertex_list[edge.from].adjacents.push_back(edge.id);
	vertex_list[edge.from].id = edge.from;
	edge_id++;
	int d = edge.from;
	edge.from = edge.to;
	edge.to = d;
	edge.id = edge_id;
	edge_list.push_back(edge);
	vertex_list[edge.from].adjacents.push_back(edge.id);
	vertex_list[edge.from].id = edge.from;
	edge_id++;
	
}
double abs(double x){
	if(x < 0 ){
		return -x;
	}
	return x;
}
int main() {
	ifstream inputFile;
	inputFile.open(("hw4.inp"));	
	int N,M;
	vector< Vertex > vertex_list;
	vector< Edge > edge_list;
	int edge_id = 0;
	if(inputFile.is_open()){
		inputFile >> N;
		vertex_list.resize(N+4);
		inputFile >> M;
		Edge edge ;
		Vertex vertex;
		
		for(int i=0;i<M;i++){
			inputFile >> edge.from;
			inputFile >> edge.to;
			inputFile >> edge.capacity;
			edge.flow = 0;
			edge.id = edge_id;
			edge_list.push_back(edge);
			
			vertex_list[edge.from].adjacents.push_back(edge.id);
			vertex_list[edge.from].id = edge.from;
			edge_id++;
			
			int d = edge.from;
			edge.from = edge.to;
			edge.to = d;
			edge.id = edge_id;
			edge_list.push_back(edge);
			vertex_list[edge.from].adjacents.push_back(edge.id);
			vertex_list[edge.from].id = edge.from;
			edge_id++;
		}
	}
	/*
	for(unsigned int i=0;i<vertex_list.size();i++){
		cout << vertex_list[i].id << " => " ;
		for(unsigned int j=0;j<vertex_list[i].adjacents.size();j++){
			edge_list[vertex_list[i].adjacents[j]].print();
			cout << " ||| " ;
		}
		cout << endl;
	}
	*/
	
	double max_flow_1 = Ford_Fulkerson(0,1,vertex_list,edge_list);
	double max_flow_2 = Ford_Fulkerson(0,2,vertex_list,edge_list);


	add_edge_vertex(1,N+3,edge_id,vertex_list,edge_list);
	add_edge_vertex(2,N+3,edge_id,vertex_list,edge_list);
	double max_flow_3 = Ford_Fulkerson(0,N+3,vertex_list,edge_list);

	
	max_flow_3 = max_flow_3/2;
	if(max_flow_1 < max_flow_3){
		max_flow_3 = max_flow_1;
	}
	if(max_flow_2 < max_flow_3){
		max_flow_3 = max_flow_2;
	}
	edge_list[edge_list.size()-1].capacity = max_flow_3;
	edge_list[edge_list.size()-2].capacity = max_flow_3;
	edge_list[edge_list.size()-3].capacity = max_flow_3;
	edge_list[edge_list.size()-4].capacity = max_flow_3;
	
	double final_flow = Ford_Fulkerson(N+3,0,vertex_list,edge_list);
	
	ofstream outputFile;
	outputFile.open(("hw4.out"));
	outputFile << edge_list[0].from << " " << edge_list[0].to << " " << abs(edge_list[0].flow) ;
	for(unsigned int i=2;i<edge_list.size()-4;i=i+2){
		outputFile << endl;
		outputFile << edge_list[i].from << " " << edge_list[i].to << " " << abs(edge_list[i].flow) ;
	}
	

	
	return 0 ;
}

