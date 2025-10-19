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
      return a->freq > b->freq; // smaller freq = higher priority

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

unordered_map<char, string> encode(string msg) {
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
  unordered_map<char, string> codes;
  generateCodes(root, codes, "");

  return codes;
}

string decode(string encoded, unordered_map<char, string> codes) {
  string decoded = "";
  string current = "";

  for (char ch : encoded) {
    current += ch;

    for (auto &pair : codes) {
      if (pair.second == current) {
        decoded += pair.first;
        current = "";

        break;
      }
    }
  }

  return decoded;
}

int main() {
  string msg = "paralelepipedo";
  unordered_map<char, string> codes = encode(msg);
  string encoded = "";

  for (char ch : msg)
    encoded += codes[ch];

  string decoded = decode(encoded, codes);

  cout << "Word: " << msg << endl;
  cout << "Encoded: " << encoded << endl;

  cout << "Codes:" << endl;
  for (auto &pair : codes)
    cout << "'" << pair.first << "': " << "\"" << pair.second << "\"" << endl;

  cout << "Decoded: " << decoded << endl;

  return 0;
}
