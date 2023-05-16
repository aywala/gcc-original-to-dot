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

class Function{
public:
    vector<Node*> nodes;
    vector<Edge*> edges;
    string name;
    Function(string n) : name(n){};
};

vector<Function*> read_and_analyze(string original_file_name){

    vector<Function*> functions;

    ifstream infile(original_file_name);
    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << original_file_name << std::endl;
        return functions;
    }
    string line;
    Node *current_node = nullptr;
    Function *current_function = nullptr;
    string seq;
    string function_name;
    smatch match;
    while (getline(infile, line))
    {
        if (line.empty() || line.substr(0, 4) == ";; e") {
            continue;
        }
        if (line.substr(0, 4) == ";; F"){
            function_name = line.substr(12, line.length() - 12);
            current_function = new Function(function_name);
            functions.push_back(current_function);
        }
        if (line[0] == '@') {
            regex pattern_1(R"(^@\d+)");
            regex_search(line, match, pattern_1);
            string temp = match[0];
            seq = temp.substr(1);
            regex pattern_2("\\s+(\\w+)");
            regex_search(line, match, pattern_2);
            string name = match[1];
            current_node = new Node(seq, name);
            current_function->nodes.push_back(current_node);
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
            Edge* edge = new Edge(edge_start, edge_end);
            current_function->edges.push_back(edge);

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
    return functions;
}

bool write_dot(Function* function){
    string function_name = function->name;
    for (int i = 0; i < function_name.length(); i++) {
        if (!isalnum(function_name[i])) {
            function_name.replace(i, 1, "_");
        }
    }
    string output_file_name = "ast_" + function_name + ".dot";
    ofstream file(output_file_name,ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to  create ast.dot" << std::endl;
        return false;
    }

    file << "digraph " << function_name << " {" << endl;
    file << "node [shape=record];" << endl;
    for (auto n : function->nodes)
    {
        string line="";
        line += n->seq + " ";
        line += "[label = \"{";
        line += n->seq + " " + n->name;
        line += " | { ";
        for(auto f : n->fields){
            line += "<" + f + "> " + f + " |";
        }
        line.pop_back();
        line += " }}\"];";
        file << line << endl;
    }

    for(auto e : function->edges){
        string line = e->start + " -> " + e->end + ";";
        file << line << endl;
    }

    file << "}" << endl;
    file.close();
    return true;
}

int main(int argc, char *argv[]){

    string original_file_name = argv[1];
    vector<Function*> functions;
    functions = read_and_analyze(original_file_name);
    for(auto f : functions){
        write_dot(f);
    }
}
