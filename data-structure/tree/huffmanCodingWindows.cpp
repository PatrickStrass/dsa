#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include <fstream>
#include <Windows.h>

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

void gerarArqDot(Node *root, string folder = "dotFiles", string filename = "arvoreBinGerado.dot");
void treeNavigation(Node *root, string parent, ofstream &arqSaida, string curr, string parent_curr);

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

void treeNavigation(Node *root, string parent, ofstream &arqSaida, string curr, string parent_curr) {
  if (!root)
    return;

  string nextParent;
  string labelData = "";

  // Leaf node represents a character.
  if (!root->left && !root->right) {
    if(isalnum(root->ch))
      nextParent = root->ch;
    else
    {
      nextParent = "chr" + to_string((int)root->ch);
    }
  }
  else
  {
    nextParent = to_string(root->freq);
  }

  if(curr.size() > 0)
    arqSaida << curr << " [label=\"" << nextParent << "\"];" << endl;
  else
    arqSaida << "root [label=\"" << nextParent << "\"];" << endl;


  if(parent.size() > 0)
  {
    if(parent_curr.size() > 0)
      arqSaida << parent_curr << " [label=\"" << parent << "\"];" << endl;
    else
      arqSaida << "root [label=\"" << parent << "\"];" << endl;

    if(curr.size() > 0)
    {
      labelData = (curr[curr.length() - 1]);
      labelData = labelData.append("\"]");
      labelData = " [label=\"" + labelData;
    }

		arqSaida << (parent_curr.size() > 0 ? parent_curr : "root") << " -> " << (curr.size() > 0 ? curr : "root") << labelData << ";" << endl;
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

  gerarArqDot(root);

  root = pq.top();

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
  //string msg = "paralelepipedo";
  string msg = "abcdefghijklmnopqrstuvwxyz";
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

  system("dot -Tsvg dotFiles\\ArvoreBinGerado.dot -o svgFiles\\ArvoreBinGerado.svg");
	cout << "SVG Gerado!\n"; 

  return 0;
}

void gerarArqDot(Node *root, string folder, string filename)
{
	ofstream arqSaida;

	wstring temp = wstring(folder.begin(), folder.end());
	LPCWSTR wideStringFolder = temp.c_str();
	if (CreateDirectoryW(wideStringFolder, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		// Pasta criada ou localizada com sucesso!
	} else {
		cout << "Houve uma falha ao criar uma pasta DOT!\n";
	}

	arqSaida.open(folder+"\\"+filename,ios::out);
	if (arqSaida.is_open()) {
        cout << "Arquivo aberto com sucesso!\n";
        
        // Escreve no arquivo
        
		arqSaida << "strict digraph ArvoreBinaria {" << endl;
		arqSaida << "node [shape=circle, style=\"filled, radial\", fontname=\"Arial Black\", color=\"white:#770000\", fillcolor=\"white:#FF7700\", gradientangle=\"110\"];" << endl;
		arqSaida << "edge [style=dashed];" << endl;
		treeNavigation(root, "", arqSaida, "", "");

		arqSaida << "}" << endl;

        
        // Fecha o arquivo
        arqSaida.close();
		cout << "Arquivo salvo com sucesso!\n";

		wideStringFolder = L"svgFiles";
		if (CreateDirectoryW(wideStringFolder, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
		{
			// Pasta criada ou localizada com sucesso!
		} else {
			cout << "Houve uma falha ao criar uma pasta SVG!\n";
		}
      //Cria o arquivo svg
    } else {
        cout << "Erro ao abrir o arquivo.\n";
    }
    return;
}