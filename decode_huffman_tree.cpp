#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Node {
public:
    int data;
    Node *left, *right;
    Node (int d) : data(d), left(NULL), right(NULL) {}
};

Node* construct(int* in, int* post, int beg, int end, int* posti) {
    if (beg > end) { // you gotta start before the end man, that's just math
        cout << "nah man";
        return NULL;
    }
    
    Node* cur = new Node(post[*posti]); // take last num in postorder list as new break point
    (*posti)--;

    if (beg==end) return cur; // if theres nowhere else to go, go back

    int i;
    for (i=beg; i<end; i++) { // find new break point in inorder
        if (in[i] == cur->data) break;
    }

    cur->right = construct(in, post, i+1, end, posti);
    cur->left = construct(in, post, beg, i-1, posti);

    return cur;
}

Node* bigconstruct (int* in, int* post, int len) {
    int posti = len-1;
    return construct(in, post, 0, posti, &posti);
}

string decode (Node* root, string en) { // use substr(start, length);
    Node* cur = root;
    string fin = "";
    int len = en.size();
    for (int i=0; i<len; i++) {
        if (en[i]=='1') { // first num is 1
            cur = cur->right;
        }
        else { // first num is 0
            cur = cur->left;
        }

        if ((cur->left==NULL) && (cur->right==NULL)) {
            //cout << (char)cur->data;
            fin += cur->data;
            cur = root;
        }
    }
    return fin;
}

int main(int argc, char **argv) {
    ifstream inorder, postorder, encoded;

    inorder.open(argv[1]); // open files
    postorder.open(argv[2]); 
    encoded.open(argv[3]); 

    if (!inorder.is_open()) { // assure proper opening of files
        cout << "Could not open file " << argv[1] << endl;
        return 1;
    }
    if (!postorder.is_open()) {
        cout << "Could not open file " << argv[2] << endl;
        return 2;
    }
    if (!encoded.is_open()) {
        cout << "Could not open file " << argv[3] << endl;
        return 3;
    }

    int lenin = 0, lenpost = 0, dummy;
    while(!inorder.eof()) { //count both files lengths to declare proper length arrays
        inorder >> dummy;
        if (inorder.eof()) break;
        lenin++;
    }
    while(!postorder.eof()) {
        postorder >> dummy;
        if (postorder.eof()) break;
        lenpost++;
    }

    inorder.clear(); // reset files for rereading
    inorder.seekg(0, ios::beg);
    postorder.clear();
    postorder.seekg(0, ios::beg); 

    int in[lenin], post[lenpost], i=0;
    while(!inorder.eof()) { // read in inorder and postorder files
        inorder >> in[i];
        i++;
    }
    i=0;
    while(!postorder.eof()) {
        postorder >> post[i];
        i++;
    }
    i=0;

    Node* root = bigconstruct(in, post, lenin); // construct huffman tree recursively

    string en;
    encoded >> en;
    while (!encoded.eof()) {
        string temp;
        encoded >> temp;
        en += temp;
    }
    string soln = decode(root, en);
    cout << soln;

    return 0;
}
