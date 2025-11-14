#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

#define H_MAX 256
#define ROOT_NODE_NUMBER (H_MAX * 2)

string getNBits(unsigned char ch, int size) {
  string bits = "";

  for (int i = size - 1; i >= 0; i--)
    bits.push_back((ch & (1 << i)) ? '1' : '0');

  return bits;
}

class Node {
public:
  unsigned char ch;
  int freq;
  int number;
  Node *parent, *left, *right;

  Node() {
    ch = 0;
    freq = 0;
    number = 0;
    parent = nullptr, left = nullptr, right = nullptr;
  }

  Node(unsigned char ch, int freq) {
    this->ch = ch;
    this->freq = freq;
    number = 0;
    parent = nullptr, left = nullptr, right = nullptr;
  }

  Node(unsigned char ch) {
    this->ch = ch;
    freq = 0;
    number = 0;
    parent = nullptr, left = nullptr, right = nullptr;
  }

  bool isLeaf() { return left == nullptr && right == nullptr; }
};

void gerarArqDot(Node *root, string folder = "dotFiles",
                 string filename = "arvoreBinGerado.dot");

class HuffmanTree {
public:
  Node *root;
  Node *NYT;
  int implicitNumber;
  unordered_map<char, Node *> leafs;
  unordered_map<int, Node *> numbers;
  unordered_map<char, string> codes;
  string encoded, decoded;

  HuffmanTree() {
    implicitNumber = ROOT_NODE_NUMBER - 1;
    root = new Node();
    root->number = implicitNumber--;
    numbers[root->number] = root;
    NYT = root;
  }

  string encode(string msg) {
    for (unsigned char ch : msg) {
      if (!leafs[ch]) {
        encoded += getCode(NYT);
        encoded += getNBits(ch, 8);
        createNode(ch);
        update(leafs[ch]->parent);
      } else {
        encoded += getCode(leafs[ch]);
        update(leafs[ch]);
      }

      printTree(); // Print tree after each update
    }

    gerarArqDot(root);

    return encoded;
  }

  // TODO bugs?
  string decode(string bits) {
    // for (int i = 0; i < bits.size(); i++) {
    //
    //   if (i == 0
    //     update(bits[i]);
    //   else {
    //     unsigned char ch = getCode(root);
    //
    //     if (ch == NYT->ch) {
    //       decoded += getNBits(ch, 8);
    //     }
    //   }
    // }

    return decoded;
  }

  void createNode(unsigned char ch) {
    NYT->left = new Node('\0', 0);
    NYT->right = new Node(ch, 1);

    NYT->left->parent = NYT;
    NYT->right->parent = NYT;

    NYT->ch = '\0';
    // NYT->freq = 1;

    leafs[ch] = NYT->right;

    // Number the nodes
    // NYT->number = implicitNumber--;
    leafs[ch]->number = implicitNumber--;
    NYT->left->number = implicitNumber--;

    // Store them in the hash map indexed by the node numbers
    // numbers[NYT->number] = NYT;
    numbers[leafs[ch]->number] = leafs[ch];
    numbers[NYT->left->number] = NYT->left;

    NYT = NYT->left;
  }

  string getCode(Node *leaf) {
    string code;
    Node *current = leaf;

    while (current && current->parent) {
      if (current == current->parent->left)
        code = '0' + code;
      else
        code = '1' + code;

      current = current->parent;
    }

    return code;
  }

  void codeTable() {
    for (auto &it : codes)
      cout << it.first << ": " << it.second << endl;
  }

private:
  Node *getBlockLeader(Node *node) {
    Node *highest = nullptr;

    for (int i = node->number + 1; i < ROOT_NODE_NUMBER; i++) {
      if (node->freq == numbers[i]->freq) {
        if (numbers[i] != node->parent) {
          if (!(node->isLeaf() && numbers[i]->isLeaf())) {
            highest = numbers[i];
          } else
            break;
        }
      } else
        break;
    }

    return highest;
  }

  void slideNode(Node *node) {
    if (!node)
      return;

    unsigned long freq = node->freq;

    for (int i = node->number + 1; i < ROOT_NODE_NUMBER; i++) {
      if (freq >= numbers[i]->freq) {
        if (numbers[i] != node->parent) {
          if (!node->isLeaf() && !numbers[i]->isLeaf() &&
              freq == numbers[i]->freq)
            break;
          else
            swapNodes(node, numbers[i]);
        }
      } else
        break;
    }
  }

  void slideAndIncrement(Node **node) {
    Node *temp = (*node)->parent;

    slideNode(*node);
    (*node)->freq++;

    if ((*node)->isLeaf())
      *node = (*node)->parent;
    else
      *node = temp;
  }

  void update(Node *node) {
    Node *leader = getBlockLeader(node);
    Node *leafIncrement = nullptr;

    if (leader)
      swapNodes(node, leader);

    if (node->parent == NYT->parent) {
      leafIncrement = node;
      node = node->parent;
    }

    while (node)
      slideAndIncrement(&node);

    if (leafIncrement)
      slideAndIncrement(&leafIncrement);
  }

  void swapNodes(Node *a, Node *b) {
    if (a == nullptr || b == nullptr || a == b)
      return;

    // Case: same parent (siblings)
    if (a->parent == b->parent && a->parent) {
      if (a->parent->left == a && a->parent->right == b) {
        a->parent->left = b;
        a->parent->right = a;
      } else if (a->parent->left == b && a->parent->right == a) {
        a->parent->left = a;
        a->parent->right = b;
      }

      swap(numbers[a->number], numbers[b->number]);
      swap(a->number, b->number);

      return;
    }

    // Different parents: attach swapped children to their new parents
    Node *aParent = a->parent;
    Node *bParent = b->parent;

    if (aParent) {
      if (aParent->left == a)
        aParent->left = b;
      else if (aParent->right == a)
        aParent->right = b;
    }
    if (bParent) {
      if (bParent->left == b)
        bParent->left = a;
      else if (bParent->right == b)
        bParent->right = a;
    }

    a->parent = bParent;
    b->parent = aParent;

    // Swap entries in numbers map and the numeric labels
    swap(numbers[a->number], numbers[b->number]);
    swap(a->number, b->number);
  }

  void printTree() {
    vector<int> keys;

    for (auto &p : numbers)
      keys.push_back(p.first);

    sort(keys.begin(), keys.end(), greater<int>()); // high->low

    for (int k : keys) {
      Node *n = numbers[k];

      if (!n)
        continue;

      cerr << "num=" << k << " freq=" << n->freq
           << " ch=" << (n->isLeaf() ? to_string(n->ch) : string("int"))
           << " parent="
           << (n->parent ? to_string(n->parent->number) : string("null"))
           << "\n";
    }

    cerr << endl;
  }
};

void treeNavigation(Node *root, string parent, ofstream &arqSaida, string curr,
                    string parent_curr);

void treeNavigation(Node *root, string parent, ofstream &arqSaida, string curr,
                    string parent_curr) {
  if (!root)
    return;

  string nextParent;
  string labelData = "";

  // Leaf node represents a character.
  if (!root->left && !root->right) {
    if (isalnum(root->ch))
      nextParent = root->ch;
    else {
      nextParent = "chr" + to_string((int)root->ch);
    }
  } else {
    nextParent = to_string(root->freq);
  }

  if (curr.size() > 0)
    arqSaida << curr << " [label=\"" << nextParent << "\"];" << endl;
  else
    arqSaida << "root [label=\"" << nextParent << "\"];" << endl;

  if (parent.size() > 0) {
    if (parent_curr.size() > 0)
      arqSaida << parent_curr << " [label=\"" << parent << "\"];" << endl;
    else
      arqSaida << "root [label=\"" << parent << "\"];" << endl;

    if (curr.size() > 0) {
      labelData = (curr[curr.length() - 1]);
      labelData = labelData.append("\"]");
      labelData = " [label=\"" + labelData;
    }

    arqSaida << (parent_curr.size() > 0 ? parent_curr : "root") << " -> "
             << (curr.size() > 0 ? curr : "root") << labelData << ";" << endl;
  }

  if (!root->left && !root->right) {

    return;
  }

  treeNavigation(root->left, nextParent, arqSaida, curr + '0', curr);
  treeNavigation(root->right, nextParent, arqSaida, curr + '1', curr);
}

// Traverse the tree in preorder manner and push the Huffman code of each
// character
void generateCodes(Node *root, unordered_map<char, string> &codes,
                   string curr) {
  if (!root)
    return;

  // Leaf node represents a character.
  if (!root->left && !root->right) {
    codes[root->ch] = curr;

    return;
  }

  generateCodes(root->left, codes, curr + '0');
  generateCodes(root->right, codes, curr + '1');
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Message to encode as argument is missing");

    return 1;
  }

  string msg = argv[1];
  HuffmanTree *encoder = new HuffmanTree();
  HuffmanTree *decoder = new HuffmanTree();
  string encoded = encoder->encode(msg);
  string decoded = decoder->decode(encoded);

  cout << "Word: " << msg << endl << endl;
  cout << "Encoded: " << encoded << endl << endl;
  cout << "Decoded: " << decoded << endl << endl;

  system("dot -Tsvg dotFiles\\ArvoreBinGerado.dot -o "
         "svgFiles\\ArvoreBinGerado.svg");
  cout << "SVG Gerado!\n";

  return 0;
}

void gerarArqDot(Node *root, string folder, string filename) {
  filesystem::create_directories(folder);
  filesystem::create_directories("svgFiles");

  string filepath = folder + "/" + filename;
  ofstream arqSaida(filepath);

  if (!arqSaida.is_open()) {
    cerr << "Error opening file: " << filepath << endl;
    return;
  }

  cout << "File created successfully: " << filepath << endl;

  arqSaida << "strict digraph ArvoreBinaria {\n";
  arqSaida << "node [shape=circle, style=\"filled, radial\", "
              "fontname=\"Arial Black\", color=\"white:#770000\", "
              "fillcolor=\"white:#FF7700\", gradientangle=\"110\"];\n";
  arqSaida << "edge [style=dashed];\n";

  treeNavigation(root, "", arqSaida, "", "");
  arqSaida << "}\n";

  arqSaida.close();
  cout << "DOT file saved successfully!\n";
}
