//PRL projekt: PREORDER TREE TRAVERSAL
// Autor: Monika Rosinská, xrosin00
//Datum: 24. 4. 2022

#include <fstream>
#include <vector>
#include <mpi.h>
#include <thread>
#include <cmath>

int input_length;
int count_of_processes;

using namespace std;

///////////////// TYPES AND STRUCTURES ////////////////////

// class for saving edges
// contains:
    // id
    // from - id of node, from which edge starts
    // to - id of node, to which edge ends
    // type - type of edge (right, left, back)
// functions:
    // get_id - returns id of edge
    // get_from - returns id of node, from which edge starts
    // get_to - returns id of node, to which edge ends
class Edge{
public:
    Edge(int id, int from, int to) {
        this->id = id;
        this->from = from;
        this->to = to;
    };
    int get_id() const{
        return id;
    };
    int get_to() const{
        return to;
    };
private:
    int id;
    int from;
    int to;
};

// class for saving nodes
// contains:
    // id
    // name - name of node given in input (doesn't have to be unique)
    // edges - vector of edges, which starts from this node
// functions:
    // get_id - returns id of node
    // get_name - returns name of node
    // get_edges - returns vector of edges, which starts from this node
    // add_edge - adds given edge to vector of edges
class Node{
public:
    Node(int id, char name){
        this->id = id;
        this->name = name;
    };
    Node(){
        id = -1;
        name = 0;
    };
    int get_id() const{
        return id;
    };
    char get_name() const{
        return name;
    };
    vector<Edge> get_edges(){
        return edges;
    };
    void add_edge(Edge edge){
        edges.push_back(edge);
    };
private:
    int id;
    char name;
    vector<Edge> edges = {};
};

// struct for saving parts of adjacency list
// contains:
    // edge - id of starting edge
    // reversed_edge - id of edge reversed to edge
    // visited - id of node, which is between this to edges
struct Way{
    int edge;
    int reversed_edge;
    int visited_node;
    int reversed_visited_node;
};

// struct for saving lines of adjacency list
// contains:
    // ways - vector of ways (struct way)
// functions:
    // last - get last way (last in vector ways)
    // first - get first way (first in vector ways)
struct adjacency_list_line {
    vector<Way> ways;
    int id = 0;
    Way last() {
        return ways.back();
    }
    Way first() {
        return ways[0];
    }
};


///////////////// ADDITIONAL FUNCTIONS ////////////////////

// get reverse edge
// edge_id - edge for which we want to get reversed edge
// because the algorithm of saving edges is designed to save edges the way,
//      that edges going down has id i, and edges going up have id i+1 (starting at id 1),
//      so we find out if it's edge down (even number) or up (odd number)
//      if it's odd number, we get reversed edge by edge_id - 1
//      if it's even number, we get reversed edge by edge_id + 1
int get_reverse_edge_id(int edge_id){
    if(edge_id % 2 == 0){
        return edge_id - 1;
    }
    else{
        return edge_id + 1;
    }
}

void print_preorder(vector<int> suffix_sum, map<int, Node> nodes, vector<Edge> edges){
    map<int, int> preorder;
    cout<<nodes[1].get_name();
    for(auto edge: edges){
        if(edge.get_id() % 2 != 0){
            int node_id = edge.get_to();
            int value = input_length - suffix_sum[edge.get_id()] + 1;
            preorder[value] = node_id;
        }
    }

    for (auto pair: preorder){
        cout<<nodes[get<1>(pair)].get_name();
    }
    cout<<endl;
}


///////////////// PROCESS INPUT ////////////////////

// works on logic: node is at index i, its left child is at index i*2 and its right child is at index i*2+1
// returns tuple of map of nodes and vector of edges
tuple<map<int,Node>, vector<Edge>> process_input(string input){
    map<int, Node> nodes;
    vector<Edge> edges;

    // get first node and edges to its successors
    Node node(1, input[0]);
    Edge edge_left(1, 1, 2);
    Edge edge_right(1 + 2, 1, 2 * 1 + 1);
    node.add_edge(edge_left);
    node.add_edge(edge_right);

    // save first node and its successors
    nodes[1] = node;
    edges.push_back(edge_left);
    edges.push_back(edge_right);

    int calibration = 3;
    int edge_id = 5;
    // for each node: save node, save edges to its successors and edge to its parent
    for (int i = 2; i <= input_length; i++){
        // save current current_node
        Node current_node(i, input[i - 1]);

        // save edge to its parent
        Edge edge_back(edge_id - calibration, i, i/2);
        edges.push_back(edge_back);
        current_node.add_edge(edge_back);

        // save edges to its successors
        if(2*i <= input_length){
            Edge edge_left(edge_id, i, 2*i);
            edges.push_back(edge_left);
            current_node.add_edge(edge_left);
        }
        if(2*i + 1 <= input_length){
            Edge edge_right(edge_id + 2, i, 2*i + 1);
            edges.push_back(edge_right);
            current_node.add_edge(edge_right);
        }
        calibration += 2;
        edge_id += 4;
        nodes[i] = current_node; // add current_node to list
    }
    return {nodes, edges};
}


///////////////// ADJACENCY LIST ////////////////////

// create one line of adjacency list
// node - node for which we make adjacency list
// nodes - list (actually map) of all nodes
adjacency_list_line make_adjacency_list(Node node){
    adjacency_list_line item;
    for(Edge edge : node.get_edges()){
        int next_node_id = edge.get_to();
        int reversed_edge = get_reverse_edge_id(edge.get_id());
        item.ways.push_back({edge.get_id(), reversed_edge, next_node_id, node.get_id()});
    }
    item.id = node.get_id()-1;
    return item;
}

// for main process
// send adjacency list to other processes
// as root process is considered process with id 0
void send_adj_list_to_others(const vector<adjacency_list_line>& adjacency_list){
    int next_item = -1; // divides lines of adjacency list
    int end = -2; // indicate end of adjacency list
    for (const auto& line: adjacency_list){
        for (auto way: line.ways){
            MPI_Bcast(&(way.edge), 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(&(way.reversed_edge), 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(&(way.visited_node), 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(&(way.reversed_visited_node), 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
        MPI_Bcast(&next_item, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    MPI_Bcast(&end, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

// for other processes
// receive adjacency lost from root (process with id 0)
vector<adjacency_list_line> receive_adj_list(){
    vector<adjacency_list_line> adjacency_list;
    int line_id = 0; // used for indexing lines of adjacency list

    int edge_value = 0;
    int reversed_edge_value;
    int visited_node_value;
    int reversed_visited_node_value;

    // -1 indicates end of line of adjacency list (push item to adjacency list and create new)
    // -2 indicates end of adjacency list
    while(edge_value != -2){
        adjacency_list_line line;
        // get line until -1 is received
        MPI_Bcast(&edge_value, 1, MPI_INT, 0, MPI_COMM_WORLD); // get edge value
        while (edge_value != -1 && edge_value != -2){
            MPI_Bcast(&reversed_edge_value, 1, MPI_INT, 0, MPI_COMM_WORLD); // get reversed edge value
            MPI_Bcast(&visited_node_value, 1, MPI_INT, 0, MPI_COMM_WORLD); // get visited node value
            MPI_Bcast(&reversed_visited_node_value, 1, MPI_INT, 0, MPI_COMM_WORLD); // get visited node value
            (line.ways).push_back({edge_value, reversed_edge_value, visited_node_value, reversed_visited_node_value}); // add way to current line
            MPI_Bcast(&edge_value, 1, MPI_INT, 0, MPI_COMM_WORLD); // get reversed edge value of next way
        }
        // if while was corrupted, and it's not the end of adjacency list, then add line to adjacency list
        if(edge_value != -2){
            line.id = line_id;
            adjacency_list.push_back(line);
        }
        // we are going to process next line
        line_id++;
    }
    return adjacency_list;
}


///////////////// EULER TOUR ////////////////////

// for main process
// instruct other processes to compute euler tour for one edge
map<int, int> create_euler_tour(const vector<adjacency_list_line>& adjacency_list){
    int process_id = 1; // which process we send messages to

    // give work to other processes
    for (adjacency_list_line line: adjacency_list){
        // each way (resp. id of edge and line, where it is) send to one process
        for (Way way: line.ways){
            MPI_Send(&(line.id), 1, MPI_INT, process_id, 5, MPI_COMM_WORLD);
            MPI_Send(&(way.reversed_edge), 1, MPI_INT, process_id, 6, MPI_COMM_WORLD);
            process_id++; // increase process id, so we send next messages to next process
        }
    }

    // get results
    map<int, int> euler_tour; // to save euler tour (edge_id, result)
    int edge_id; // id of edge for which process computed euler tour
    int result;
    // get results from each processes
    for(int i = 1; i < count_of_processes; i++){
        MPI_Recv(&edge_id, 1, MPI_INT, i, 7, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&result, 1, MPI_INT, i, 7, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        euler_tour.insert({edge_id, result}); // save result
    }

    // cutting euler tour, so it is not cyclic
    // made by cutting edge going to root, it is always at index 4
    euler_tour[4] = 4;

    return euler_tour;
}

// for main process
// send euler euler_tour to others
// euler_tour - euler euler_tour to be send
void send_euler_tour_to_others(const map<int, int>& euler_tour){
    int end = -1; // indicates end of euler euler_tour
    for (auto pair: euler_tour){
        MPI_Bcast((void *) &(get<0>(pair)), 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&(get<1>(pair)), 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    MPI_Bcast(&end, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

// for not-main processes
// get part of euler tour for one edge
// adjacency list - adjacency list for all nodes
// line_id - node id, where we should start
// edge_id - edge id, for which we make euler tour
void euler_tour(vector<adjacency_list_line> adjacency_list, int line_id, int edge_id){
    adjacency_list_line adj_list_line = adjacency_list[line_id];
    int result;
    // check each way in our adj_list_line, and if first edge of the way matches our edge_id, count euler tour
    for(int i = 0; i < (adj_list_line.ways).size(); i++){
        Way way = adj_list_line.ways[i];
        if(way.reversed_edge == edge_id){
            // if the way is the last element of adj_list_line in adjacency list, next edge is first edge of node we visited
            if (way.edge == adj_list_line.last().edge){
                result = adjacency_list[way.reversed_visited_node - 1].ways[0].edge;
                break;
            }
            // else it's not last adj_list_line and next edge is first edge of next way in adj_list_line
            else{
                result = (adj_list_line.ways[i+1]).edge;
                break;
            }
        }
    }
    MPI_Send(&edge_id, 1, MPI_INT, 0, 7, MPI_COMM_WORLD);
    MPI_Send(&result, 1, MPI_INT, 0, 7, MPI_COMM_WORLD);
}

// for not-main processes
// receive euler tour from main process
map<int, int> receive_euler_tour(){
    map<int, int> euler_tour;
    int edge_id;
    int value;

    // -1 indicates end of euler tour
    MPI_Bcast(&(edge_id), 1, MPI_INT, 0, MPI_COMM_WORLD);
    while(edge_id != -1){
        MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);
        euler_tour.insert({edge_id, value});
        MPI_Bcast(&(edge_id), 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    return euler_tour;
}


///////////////// SUFFIX SUM ////////////////////

// for main process
// instruct others to count weights of edges
void get_weights(){
    vector<int> weights;
    int weight;

    // send processes edges, which they should count
    for (int i = 1; i < count_of_processes; i++){
        MPI_Send(&i, 1, MPI_INT, i, 8, MPI_COMM_WORLD);
    }

    // receive results and save them
    for (int i = 1; i < count_of_processes; i++){
        MPI_Recv(&weight, 1, MPI_INT, i, 8, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        weights.push_back(weight);
    }

    // send results to others
    for (int i = 0; i < count_of_processes - 1; i++){
        MPI_Bcast(&weights[i], 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
}

// for other processes
// count weight of edge, sends it to main process and saves weights of edges from other processes
vector<int> get_weight(){
    int edge_id;
    vector<int> weights;
    int weight;

    // get id of edge which we count weight for
    MPI_Recv(&edge_id, 1, MPI_INT, 0, 8, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // if edge is forward, it has even id, otherwise it has odd id
    // even edges should have weight 1, odd should have weight 0 (implicit)
    weight = edge_id % 2;

    // send our result to main process
    MPI_Send(&weight, 1, MPI_INT, 0, 8, MPI_COMM_WORLD);

    // save weights of all edges
    for(int i = 0; i < count_of_processes - 1; i++){
        MPI_Bcast(&weight, 1,   MPI_INT, 0, MPI_COMM_WORLD);
        weights.push_back(weight);
    }
    return weights;
}

// for main process
// handles data passing between other processes
vector<int> get_suffix_sum(){
    // arrays for saving received data
    int weights[count_of_processes];
    int euler_tour[count_of_processes];
    int discard = 1;

    // repeat log(n) times, where n is count of nodes, i.e. input length
    // because it takes log(n) steps to most distanced node from end reaches the end
    for (int i = 0; i <= ceil(log2(input_length));   i++) {
        // get data from others processes
        MPI_Gather(&discard, 1, MPI_INT, weights, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Gather(&discard, 1, MPI_INT, euler_tour, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // send data to other processes
        for (int j = 1; j < count_of_processes; j++){
            MPI_Bcast(&(weights[j]), 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(&(euler_tour[j]), 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
    }
    // get final values of suffix sums
    MPI_Gather(&discard, 1, MPI_INT, weights, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // save result to vector for better return type
    vector<int> return_weights;
    for(int i = 0; i < count_of_processes; i++){
        return_weights.push_back(weights[i]);
    }
    return return_weights;
}

// for other processes
// create suffix_sum of given array
// euler_tour - calculated euler tour
// edge_id - id of edge for which is calculated suffix sum and id of process
// suffix_sum - array containing starting values, here weights of edges
// suffix_sum indexed from 0, euler tour from 1 - that's why the weird indexing
void suffix_sum(map<int, int> euler_tour, int edge_id, vector<int> suffix_sum) {
    int successor_id;
    int successor_value;
    int successor_of_successor;
    int current_value;
    int new_euler_tour;

    // repeat log(n) times, where n is count of nodes, i.e. input length
    // because it takes log(n) steps to most distanced node from end reaches the end
    for (int i = 0; i <= ceil(log2(input_length)); i++) {
        successor_id = euler_tour[edge_id];
        successor_value = suffix_sum[successor_id-1];
        suffix_sum[edge_id-1] = suffix_sum[edge_id-1] + successor_value; // add value of succcessor to my value
        successor_of_successor = euler_tour[successor_id];
        euler_tour[edge_id] = successor_of_successor; // set successor of my successor as my successor (skip node/nodes)

        // for sending info to main process
        new_euler_tour = euler_tour[edge_id];
        current_value = suffix_sum[edge_id-1];

        // send new suffix sum and euler tour to main process (our part)
        MPI_Gather(&current_value, 1, MPI_INT, nullptr, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Gather(&new_euler_tour, 1, MPI_INT, nullptr, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // save new suffix sum and euler tour (all parts)
        for (int j = 0; j < count_of_processes - 1; j++) {
            MPI_Bcast(&suffix_sum[j], 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(&euler_tour[j+1], 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
    }
    MPI_Gather(&current_value, 1, MPI_INT, nullptr, 1, MPI_INT, 0, MPI_COMM_WORLD);
}


///////////////// PROCESS FUNCTIONS ////////////////////

// work of main process
void main_process(string &input) {
    vector<Edge> edges;
    map<int, Node> nodes;
    vector<adjacency_list_line> adjacency_list;

    tie(nodes, edges) = process_input(input);

    // create adjacency list
    for (auto pair: nodes){
        adjacency_list_line line = make_adjacency_list(get<1>(pair));
        adjacency_list.push_back(line);
    }
    send_adj_list_to_others(adjacency_list);

    map<int, int> euler_tour = create_euler_tour(adjacency_list);
    send_euler_tour_to_others(euler_tour);

    get_weights();
    vector<int> suffix_sum = get_suffix_sum();

    print_preorder(suffix_sum, nodes, edges);
}

// work of other processes
void run_processes(int id) {
    vector<adjacency_list_line> adjacency_list;
    adjacency_list = receive_adj_list();

    int line_id;
    int edge_id;
    MPI_Recv(&line_id, 1, MPI_INT, 0, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&edge_id, 1, MPI_INT, 0, 6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    euler_tour(adjacency_list, line_id, edge_id);
    map<int, int> euler_tour = receive_euler_tour();

    vector<int> weights = get_weight();
    suffix_sum(euler_tour, id, weights);
}


///////////////// MAIN ////////////////////

int main(int argc, char *argv[]) {

    string input = argv[1];
    input_length = int(input.length());
    count_of_processes = input_length * 2 - 1;

    // initialize processes and get their ids
    int id;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    if(input_length <= 3){
        if(id == 0){
            cout<<input<<endl;
        }
        MPI_Finalize();
        return 0;
    }


    // work for main process
    if(id == 0){
        main_process(input);
    }
    // work for other processes
    else{
        run_processes(id);
    }

    MPI_Finalize();
    return 0;
}
