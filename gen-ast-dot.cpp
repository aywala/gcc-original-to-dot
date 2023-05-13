#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

class Node{
public:
    string seq;
    string name;
    vector<string> fields;
    Node(string s) : seq(s) {};
    Node(string s, string n) : seq(s), name (n) {};
};

class Edge{
public:
    string start;
    string end;
    Edge(string s, string e) : start(s), end(e){};
};

bool read_and_analyze(string original_file_name, vector<Node>& nodes, vector<Edge>& edges){
    ifstream infile(original_file_name);
    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << original_file_name << std::endl;
        return false;
    }
    string line;
    Node *current_node = nullptr;
    string seq;
    smatch match;
    while (getline(infile, line))
    {
        if (line.empty() || line.substr(0, 2) == ";;") {
            continue;
        }
        if (line[0] == '@') {
            if(current_node!=nullptr){
                nodes.push_back(*current_node);
                Node *p = current_node;
                current_node = nullptr;
                delete p;
            }

            regex pattern_1(R"(^@\d+)");
            regex_search(line, match, pattern_1);
            string temp = match[0];
            seq = temp.substr(1);
            regex pattern_2("\\s+(\\w+)");
            regex_search(line, match, pattern_2);
            string name = match[1];
            current_node = new Node(seq, name);
        }

        regex pattern_3("(op )?\\w+ *: @\\d+");
        sregex_iterator it(line.begin(), line.end(), pattern_3);
        sregex_iterator end;
        while(it!=end){
            string temp = (*it)[0];

            regex pattern_4("(\\w* *\\w+) *:");
            regex_search(temp, match, pattern_4);
            string tempp = match[1];
            current_node->fields.push_back(tempp);
            string edge_start = seq + ":" + "\""+ tempp+ "\"";

            regex pattern_5("@(\\d+)");
            regex_search(temp, match, pattern_5);
            string edge_end = match[1];
            Edge edge(edge_start, edge_end);
            edges.push_back(edge);

            it++;
        }

        regex pattern_6("\\w+ *: \\w+(\\.\\w+:\\d+)?");
        sregex_iterator itt(line.begin(), line.end(), pattern_6);
        while(itt!=end){
            string fld = (*itt)[0];
            current_node->fields.push_back(fld);

            itt++;
        }
    }

    if(current_node!=nullptr){
        nodes.push_back(*current_node);
        delete current_node;
    }
    return true;
}

bool write_dot(vector<Node> nodes, vector<Edge> edges){
    ofstream file("ast.dot",ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to  create ast.dot" << std::endl;
        return false;
    }

    file << "digraph AST {" << endl;
    file << "node [shape=record];" << endl;
    for (auto n : nodes)
    {
        string line="";
        line += n.seq + " ";
        line += "[label = \"{";
        line += n.seq + " " + n.name;
        line += " | { ";
        for(auto f : n.fields){
            line += "<" + f + "> " + f + " |";
        }
        line.pop_back();
        line += " }}\"];";
        file << line << endl;
    }

    for(auto e : edges){
        string line = e.start + " -> " + e.end + ";";
        file << line << endl;
    }

    file << "}" << endl;
    file.close();
    return true;
}

int main(int argc, char *argv[]){

    string original_file_name = argv[1];
    vector<Node> nodes;
    vector<Edge> edges;
    cout << "read and analyze original file..." << endl;
    if (read_and_analyze(original_file_name, nodes, edges)){
        cout << "success!" << endl;
        cout << "generate ast.dot..." << endl;
        if(write_dot(nodes, edges)){
            cout << "success!" << endl;
            cout << "now you can execute command 'dot -Tpng ast.dot -o ast.png' to generate image" << endl;
        }
    }
    /*
    for(auto n : nodes){
        cout << n.seq << endl;
        for(auto fld : n.fields){
            cout << fld << " | ";
        }
        cout << endl;
    }*/

    /*for(auto e : edges){
        cout << e.start << " -> " << e.end << endl;
    }*/
}
