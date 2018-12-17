#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

struct bdnode{
    std::string val;
    bdnode* left;
    bdnode* right;

};

typedef bdnode* bdt;

bdt buildcompactbdt(const std::vector<std::string>& fvalues);
std::string evalcompactbdt(bdt t, const std::string& input);

bdt createTree(int xn, int nodeHeight);
void createPath(bdt treenode, const std::string& temp);

void compactTree(bdt &node, bdt &rootNode, bool &treeDidChange);
bool sameSubtrees(bdt &nodeA, bdt&nodeB);
void deleteNode(bdt &nodeRight);

/// FOR TESTING ONLY ----------------------------------------------------

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <unordered_set>
#include <iomanip>

bool is_leaf(bdt node){
    return (node->val.length() == 1); // only leafs has strings of length 1
}

void generate_combinations(std::string current, int len, std::vector<std::string>& combinations){
    if(len == 0){
        combinations.push_back(current);
        return;
    }
    generate_combinations(current + "0", len - 1, combinations);
    generate_combinations(current + "1", len - 1, combinations);
}

void test_all(const std::vector<std::string>& valid, bdt tree){
    std::unordered_set<std::string> ones(valid.begin(), valid.end());
    std::vector<std::string> combinations;
    generate_combinations("", valid[0].length(), combinations);
    for(std::string combination: combinations){
        std::string res = evalcompactbdt(tree, combination);
        std::cout << combination << ": " << res << "  ";
        if((res == "1" && ones.find(combination) == ones.end()) ||
           (res == "0" && ones.find(combination) != ones.end())){
            std::cout << "ERROR ###############";
        }
        std::cout << std::endl;
    }
}

int max(int a, int b){
    if(a > b)
        return a;
    return b;
}

int tree_height(bdt tree){
    if(is_leaf(tree))
        return 0;
    return max(tree_height(tree->left), tree_height(tree->right)) +1;
}

int count_nodes(bdt tree){
    if(is_leaf(tree))
        return 1;
    return 1 + count_nodes(tree->left) + count_nodes(tree->right);
}

void print_tree(bdt tree, std::string path){
    if(is_leaf(tree)){
        std::cout << path << ": " << tree->val << std::endl;
        return;
    }
    print_tree(tree->left, path + "L ");
    std::cout << path << ": " << tree->val << std::endl;
    print_tree(tree->right, path + "R ");
}

void create_rand_input(std::vector<std::string>& input, int len, bool print){
    int num_of_ones = rand() % int(pow(2, len) - 1) + 1;
    for(int combination = 0; combination < num_of_ones; combination++){
        std::string curr = "";
        for(int i = 0; i < len; i++){
            curr.push_back(char( rand() % 2 ) + '0');
        }
        if(print)
            std::cout << curr << std::endl;
        input.push_back(curr);
    }
}

int main(){
    srand(time(NULL));
    int num_of_tests = 10;
    for(int len = 3; len < 4; len++){
        int tot_nodes = 0;
        int tot_height = 0;
        double time_elapsed = 0;
        for(int test = 0; test < num_of_tests; test++){
            std::vector<std::string> input;
            create_rand_input(input, len, false);

            std::clock_t start;
            start = std::clock();

            bdt tree = buildcompactbdt(input);

            time_elapsed += (std::clock() - start ) / (double) CLOCKS_PER_SEC;

            tot_nodes += count_nodes(tree);
            tot_height += tree_height(tree);


            print_tree(tree, "");
            //printorder(tree, 0);
            test_all(input, tree);
            std::cout << "\n==========================================\n";

        }

        int def_nodes = (int)(2 * pow(2, len) - 1);
        int avg_nodes = tot_nodes / num_of_tests;
        double avg_time = time_elapsed / (double)num_of_tests;

        std::cout << "Tree length: " << len;
        std::cout << "\nAverage tree height: " << tot_height / num_of_tests << "\nCompact tree nodes: " << avg_nodes;
        std::cout << "\nOriginal tree nodes: " << def_nodes;
        std::cout << "\nReduced tree size is: " << avg_nodes * 100 / def_nodes << "% of the original";
        std::cout << "\nAverage time to build the tree: " << avg_time << std::endl << std::endl;
    }
    return 0;
}

/// END OF TESTING ------------------------------------------------------

//build the tree data structure
bdt buildcompactbdt(const std::vector<std::string>& fvalues){
  int nodeNumber = 1;
  int createHeight = fvalues[0].size();
  bdt hdtree = createTree(nodeNumber, createHeight);

  for (int i = 0; i < fvalues.size(); i++){
      std::string temp = fvalues[i];
      createPath(hdtree, temp);
  }

  bool treeDidChange = true;
	while (treeDidChange == true) {
		treeDidChange = false;
		compactTree(hdtree, hdtree, treeDidChange);
	}

    return hdtree;
}

//traverse the tree and check whether 0 or 1 is present in input
std::string evalcompactbdt(bdt t, const std::string& input){

  while (t->val.length() > 1){   //check that a leaf is not reached
      int node_depth = std::stoi(t->val.substr(1), nullptr) - 1;

      if(input[node_depth] == '0'){
          t = t->left;
      }
      else{
          t = t->right;
      }
  }
  return t->val;
}   //function works in similar way to createPath()

/// add here the implementation for any other functions you wish to define and use
bdt createTree(int xn, int nodeHeight){

    bdt tmptree = new bdnode;

    if (nodeHeight == 0){ //condition for base case
        tmptree->val = "0";
        tmptree->left = NULL;
        tmptree->right = NULL;
    }
    else{
        tmptree->val = "x" + std::to_string(xn); //create name of nodes starting from x1
        tmptree->left = createTree(xn + 1, nodeHeight - 1);
        tmptree->right = createTree(xn + 1, nodeHeight - 1);
    }
    return tmptree;
}

void createPath(bdt treenode, const std::string& temp){

    for (int digit = 0; digit < temp.length(); digit++){

        if (temp[digit] == '0'){   //set all zeros in leaves
            treenode = treenode->left;
        }
        else {
            treenode = treenode->right;
        }
    }

    treenode->val = "1";  //set 1 in the required pathway
}


void compactTree(bdt &hdtree, bdt &node, bool &treeDidChange){
  //std::cout << "\ncompactTree called on node " << node->val;
  if(node->left != NULL && node->right != NULL){
    if (sameSubtrees(node->left, node->right) == true){    //check whether subtree has same value in its nodes
      //std::cout << "\ntrees are identical";
      bool setRootNodeToNewNode = false;
      if (hdtree == node){
        setRootNodeToNewNode = true;
      }

      deleteNode(node->right);

      *node = *node->left;

      if (setRootNodeToNewNode == true){
        hdtree = node;
      }

      treeDidChange = true;
    }
  }

  if (node->left != NULL || node->right != NULL){   //compact the tree recursively, if nodes have not reached leaf
    compactTree(hdtree, node->left, treeDidChange);
    compactTree(hdtree, node->right, treeDidChange);
  }

}

bool sameSubtrees(bdt &nodeA, bdt &nodeB){
  //std::cout << "\ncalled identical";
  if (nodeA->left != NULL && nodeB->left != NULL){
    return ((sameSubtrees(nodeA->left, nodeB->left) &&
            sameSubtrees(nodeA->right, nodeB->right)) &&
            nodeA->val == nodeB->val);
  }

  if (nodeA->val == nodeB->val){
    return true;
  }

  return false;
}

void deleteNode(bdt &nodeRight){
  if (nodeRight->right != NULL){
    deleteNode(nodeRight->right);
  }
  nodeRight = NULL;
}
