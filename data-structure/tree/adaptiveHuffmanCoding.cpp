#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

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

  Node(int freq) {
    this->ch = '\0';
    this->freq = freq;
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
    root = new Node('\0', 0);
    NYT = root;
    implicitNumber = H_MAX * 2 + 1;
  }

  string encode(string msg) {
    for (int i = 0; i < msg.size(); i++) {
      if (i == 0)
        update(msg[i]);

      else {
        if (!leafs[msg[i]]) {             // first read of character
          encoded += getCode(NYT);        // output code of NYT
          encoded += getNBits(msg[i], 8); // output byte of ch
          createNode(msg[i]);
          update(leafs[msg[i]]->parent->ch);
        } else {
          encoded += getCode(leafs[msg[i]]); // output code of node
          update(leafs[msg[i]]->ch);
        }
      }
    }

    gerarArqDot(root);

    return encoded;
  }

  // TODO bugs?
  string decode(string bits) {
    // for (int i = 0; i < bits.size(); i++) {
    //   if (i == 0)
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
  void createNode(unsigned char ch) {
    NYT->left = new Node();
    NYT->right = new Node();

    NYT->left->parent = NYT;
    NYT->right->parent = NYT;

    NYT->ch = '\0';

    leafs[ch] = NYT->right;
    leafs[ch]->ch = ch;

    // Number the nodes
    NYT->number = implicitNumber--;
    leafs[ch]->number = implicitNumber--;

    // Store them in the hash map indexed by the node numbers
    numbers[NYT->number] = NYT;
    numbers[leafs[ch]->number] = leafs[ch];

    NYT = NYT->left;
    NYT->ch = '\0';
  }

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
    unsigned long freq;

    if (!node->isLeaf())
      freq = node->freq + 1;
    else
      freq = node->freq;

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

  void update(unsigned char ch) {
    Node *leader = nullptr; // block leader
    Node *leafIncrement = nullptr;
    Node *node = leafs[ch];

    if (!node) {
      createNode(ch);
      node = NYT->parent;
      leafIncrement = node->right;
    } else {
      leader = getBlockLeader(node);

      if (leader)
        swapNodes(node, leader);

      if (node->parent == NYT->parent) {
        leafIncrement = node;
        node = node->parent;
      }
    }

    while (node != root)
      slideAndIncrement(&node);

    if (leafIncrement)
      slideAndIncrement(&leafIncrement);
  }

  void swapNodes(Node *a, Node *b) {
    Node *parent, *temp = nullptr;
    int i;

    if (a->parent == b->parent) {
      parent = a->parent;

      if (parent->left == a) {
        parent->left = b;
        parent->right = a;
      } else {
        parent->left = a;
        parent->right = b;
      }

      // Swap nodes in numbers hash map and their numbers
      temp = numbers[a->number];
      numbers[a->number] = numbers[b->number];
      numbers[b->number] = temp;

      i = a->number;
      a->number = b->number;
      b->number = i;
    }

    if (a->parent->left == a)
      a->parent->left = b;
    else
      a->parent->right = a;

    if (b->parent->left == b)
      b->parent->left = a;
    else
      b->parent->right = b;

    // Swap nodes in numbers hash map and their numbers
    temp = numbers[a->number];
    numbers[a->number] = numbers[b->number];
    numbers[b->number] = temp;

    i = a->number;
    a->number = b->number;
    b->number = i;
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

  HuffmanTree *encoder = new HuffmanTree();
  HuffmanTree *decoder = new HuffmanTree();
  string msg = argv[1];
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
