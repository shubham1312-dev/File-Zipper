#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
using namespace std;

struct HuffmanNode
{
    char data;
    int freq;
    HuffmanNode *left, *right;
    HuffmanNode(char data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

struct Compare
{
    bool operator()(HuffmanNode *l, HuffmanNode *r)
    {
        return l->freq > r->freq;
    }
};

void encode(HuffmanNode *root, string str, unordered_map<char, string> &huffmanCode)
{
    if (root == nullptr)
        return;

    if (!root->left && !root->right)
    {
        huffmanCode[root->data] = str;
    }

    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

void buildHuffmanTree(string text, unordered_map<char, string> &huffmanCode)
{
    unordered_map<char, int> freq;
    for (char c : text)
    {
        freq[c]++;
    }

    priority_queue<HuffmanNode *, vector<HuffmanNode *>, Compare> pq;
    for (auto pair : freq)
    {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    while (pq.size() != 1)
    {
        HuffmanNode *left = pq.top();
        pq.pop();

        HuffmanNode *right = pq.top();
        pq.pop();

        int sum = left->freq + right->freq;
        pq.push(new HuffmanNode('\0', sum));
    }

    HuffmanNode *root = pq.top();
    encode(root, "", huffmanCode);
}

void compressFile(string inputFile, string outputFile)
{
    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);

    string text((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    unordered_map<char, string> huffmanCode;
    buildHuffmanTree(text, huffmanCode);

    string encodedText;
    for (char c : text)
    {
        encodedText += huffmanCode[c];
    }

    int padding = 8 - (encodedText.length() % 8);
    padding = padding == 8 ? 0 : padding;

    outFile << padding << "\n";

    for (int i = 0; i < padding; i++)
    {
        encodedText += '0';
    }

    for (int i = 0; i < encodedText.length(); i += 8)
    {
        string byte = encodedText.substr(i, 8);
        char c = stoi(byte, nullptr, 2);
        outFile << c;
    }

    outFile.close();

    cout << "File compressed successfully!" << endl;
}

void decompressFile(string inputFile, string outputFile)
{
    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);

    int padding;
    inFile >> padding;

    string encodedText((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    encodedText.erase(0, to_string(padding).length() + 1);

    string decodedText;
    HuffmanNode *root = new HuffmanNode('\0', 0);
    HuffmanNode *current = root;

    for (char bit : encodedText)
    {
        if (bit == '0')
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }

        if (!current->left && !current->right)
        {
            decodedText += current->data;
            current = root;
        }
    }

    outFile << decodedText;
    outFile.close();

    cout << "File decompressed successfully!" << endl;
}

int main()
{
    string inputFileName = "input.txt";           // Replace with your input file name
    string compressedFileName = "compressed.bin"; // Replace with your compressed file name
    string decompressedFileName = "output.txt";   // Replace with your decompressed file name

    compressFile(inputFileName, compressedFileName);
    decompressFile(compressedFileName, decompressedFileName);

    return 0;
}
