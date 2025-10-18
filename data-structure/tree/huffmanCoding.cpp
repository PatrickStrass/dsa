#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Node {
public:
  char ch;
  int freq;
  Node *left, *right;

  Node(char ch, int freq) {
    this->ch = ch;
    this->freq = freq;
    left = nullptr;
    right = nullptr;
  }

  Node(int freq) {
    this->ch = '\0';
    this->freq = freq;
    left = nullptr;
    right = nullptr;
  }
};

class Compare {
public:
  bool operator()(Node *a, Node *b) {
    if (a->freq != b->freq)
      return a->freq > b->freq;   // smaller freq = higher priority

    bool aLeaf = (a->ch != '\0');
    bool bLeaf = (b->ch != '\0');

    if (aLeaf != bLeaf)
      return !aLeaf; // prefer leaf nodes over internal ones
    
    if (!aLeaf && !bLeaf) // do not care the order if both are internal nodes
      return true;

    return a->ch > b->ch; // smaller ASCII char first
  }
};

// Traverse the tree in preorder manner and push the Huffman code of each
// character
void preOrder(Node *root, vector<string> &ans, string curr) {
  if (!root)
    return;

  // Leaf node represents a character.
  if (!root->left && !root->right) {
    ans.push_back(curr);

    return;
  }

  preOrder(root->left, ans, curr + '0');
  preOrder(root->right, ans, curr + '1');
}

vector<string> encode(string msg) {
  // Min heap
  priority_queue<Node *, vector<Node *>, Compare> pq;
  unordered_map<char, int> charFreq;

  for (char ch : msg)
    charFreq[ch]++;

  for (auto &pair : charFreq) {
    Node *node = new Node(pair.first, pair.second);
    pq.push(node);
  }

  // Build huffman tree
  while (pq.size() >= 2) {
    Node *left = pq.top();
    pq.pop();

    Node *right = pq.top();
    pq.pop();

    Node *merged = new Node(left->freq + right->freq);
    merged->left = left;
    merged->right = right;

    pq.push(merged);
  }

  Node *root = pq.top();
  vector<string> ans;
  preOrder(root, ans, "");

  return ans;
}

int main() {
  string msg = "paralelepipedo"; // p = 3; a = 2; r = 1; l = 2; e = 3; i = 1; d = 1; o = 1
  vector<string> ans = encode(msg);

  cout << "Word: " << msg << endl;

  for (int i = 0; i < ans.size(); i++) {
    cout << ans[i] << " ";
  }

  return 0;
}
