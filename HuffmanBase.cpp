#include "HuffmanBase.hpp"
#include "HeapQueue.hpp"
#include <string>
#include <map>
#include <stack>

// Sadaf Sayeed Chowdhury

// Implementation of HuffmanNode member functions

char HuffmanNode::getCharacter() const {
    return character;
}

size_t HuffmanNode::getFrequency() const {
    return frequency;
}

bool HuffmanNode::isLeaf() const {
    return left == nullptr && right == nullptr;
}

bool HuffmanNode::isBranch() const {
    return !isLeaf();
}

bool HuffmanNode::isRoot() const {
    return parent == nullptr;
}

bool HuffmanNode::Compare::operator()(const HuffmanNode &n1, const HuffmanNode &n2) const {
    if(n1.frequency == n2.frequency){
        return lessThan ? n1.character < n2.character : n1.character > n2.character;
    }
    else{
        return lessThan ? n1.frequency < n2.frequency : n1.frequency > n2.frequency;
    }
}

bool HuffmanNode::Compare::operator()(const HuffmanNode *n1, const HuffmanNode *n2) const {
    if(n1->frequency == n2->frequency){
        return lessThan ? n1->character < n2->character : n1->character > n2->character;
    }
    else{
        return lessThan ? n1->frequency < n2->frequency : n1->frequency > n2->frequency;
    }
}

// Implementation of HuffmanTreeBase member functions 

// This function does preorder travel on the tree to generate the compressed string
void preOrderTraversal(HuffmanNode* node, std::string c, std::map<char, std::string>& map){
    if(node == nullptr){
        return;
    }

    if(node->isLeaf()){
        map[node->getCharacter()] = c;
    }

    // Append '0' to the string every time it goes left
    preOrderTraversal(node->left, c + "0", map);
    // Append '1' to the string every time it goes right
    preOrderTraversal(node->right, c + "1", map);
}

// This function does postorder travel on the tree to create the serialized string
void postOrderTraversal(HuffmanNode* node, std::string &s){
    if(node==nullptr){
        return;
    }

    postOrderTraversal(node->left, s);
    postOrderTraversal(node->right, s);

    // If the current node is a leaf, add "L" to the string s followed by the character
    if(node->isLeaf()){
        s+= "L";
        s+=node->getCharacter();
    }
    // If the current node is a branch, add "B" to the string s
    else{
        s+="B";

    }
}

// This function takes the serialized string as input and deserializes it
HuffmanNode* deserialize(const std::string &serialized){
    std::stack<HuffmanNode*> stack;
    // Iterate through the whole serialized string
    for(int i = 0; i< serialized.length(); i++){
        // If character is 'L', push the character next to it into the stack
        if(serialized[i] == 'L'){
            i++;
            char character = serialized[i];
            HuffmanNode* node = new HuffmanNode(character, 0);
            stack.push(node);
        }
        // If the character is 'B', merge the top two nodes in the stack and insert the merged node into the stack 
        else if(serialized[i] == 'B'){
            HuffmanNode* r = stack.top();
            stack.pop();
            HuffmanNode* l = stack.top();
            stack.pop();
            HuffmanNode* b= new HuffmanNode('\0', 0);
            b->left = l;
            b->right = r;
            l->parent = b;
            r->parent = b;
            stack.push(b);
        }
    }
    return stack.top();
}

// This node contains the root of the tree
HuffmanNode* root;

std::string HuffmanTreeBase::compress (const std::string inputStr) {

    // This creates an instance of the HeapQueue
    HeapQueue<HuffmanNode*, HuffmanNode::Compare> queue;

    // This creates the frequency map for the characters in the inputStr 
    std::map<char,size_t> map;
    for(char c : inputStr){
            map[c]++;
    }
    
    // This inserts the character-frequency pairs of the map into the HeapQueue
    for(std::map<char,size_t>::const_iterator i = map.begin(); i!=map.end(); i++){
        HuffmanNode* node = new HuffmanNode(i->first, i->second);
        queue.insert(node);
    }
  
    // This creates the Huffman tree by merging two nodes until there is one node left which is the root
    while(queue.size()!=1){
        HuffmanNode* t1 = queue.min();
        queue.removeMin();
        HuffmanNode* t2 = queue.min();
        queue.removeMin();
        root = new HuffmanNode('\0', t1->getFrequency() + t2->getFrequency());

        root->left=t1;
        root->right=t2;
        t1->parent=root;
        t2->parent=root;   

        queue.insert(root);
    }

    std::map<char, std::string> new_map;
    // This uses the recursive function to generate the Huffman codes for each character
    preOrderTraversal(root, "", new_map);

    // This combines all the string in the new_map to create one single string of Huffman code
    std::string compressedString;
    for (char c: inputStr) {
        compressedString += new_map[c];
    }

    return compressedString;
}

std::string HuffmanTreeBase::serializeTree() const {
   std::string serialize;
   // This does post order travel on the tree to serialize the string
   postOrderTraversal(root, serialize);
   return serialize;
}

std::string HuffmanTreeBase::decompress(const std::string inputCode, const std::string serializedTree) {
    HuffmanNode* r = deserialize(serializedTree);
    HuffmanNode* curr = r;
    std::string decompressedString;

    for(char c : inputCode){
        // If c is 0, curr moves to the left
        if(c == '0'){
            curr = curr->left;
        }
        else{
            // if c is 1, curr moves to the right
            curr = curr->right;
        }

        // If curr is a leaf, the corresponding character is appended to decompressedString
        if (curr->isLeaf()){
            decompressedString += curr->getCharacter();
            curr = r;
        }
    }

    return decompressedString;
}


