/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BST.h
 * Author: LTSACH
 *
 * Created on 20 August 2020, 15:31
 */

#ifndef BST_H
#define BST_H
#include <string>
#include <sstream>

#include "support/IBST.h"
#include "support/ITreeWalker.h"
#include "support/Stack.h"
#include "support/Queue.h"

/*IMPOTANT: 
 *  >> Can change nEMPTY to another value
 *  >> CAN NOT change: nLH, nEH, and nRH
 *      :: Because, we define balance-factor = height(right subtree) - height(left subtree)
 */

#define nEMPTY 9
#define nLH -1
#define nEH  0
#define nRH +1
#define sEMPTY "EMPTY"
#define sLH "LH"
#define sEH "EH"
#define sRH "RH"


template<class K, class V>
class BST : public IBST<K,V>, public ITreeWalker<K, V>{
public:
    class Node; //forward declaration  
    class Entry; //forward declaration  
    
protected:
    int count;
    Node* pRoot;
    
public:
    BST();
    ~BST();
    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;
    void add(K key, V value=0);
    V remove(K key, bool* success=0);
    V search(K key, bool& found); //return NULL if not found
    int size();
    int height();
    void clear();
    DLinkedList<V> ascendingList();
    DLinkedList<V> descendingList();
    DLinkedList<V> dfs();
    DLinkedList<V> bfs();
    DLinkedList<K> bfsKey();
    DLinkedList<V> nlr();
    DLinkedList<V> lrn();
    DLinkedList<V> lnr();
    DLinkedList<V> select(K min, K max);
    bool empty(){
        return this->count == 0;
    }
    string toString(string (*entry2str)(K&,V&)=0, bool avl=false){
        if(this->pRoot == 0) return "(NULL)";
        else return this->pRoot->toString(entry2str, avl);
    }
    void println(string (*entry2str)(K&,V&)=0, bool avl=false){
        cout << toString(entry2str, avl) << endl;
    }
    
    
protected:
    Node* add(Node* pRoot, Entry data);
    Node* remove(Node* pRoot, K key, bool& success, V& retValue);
    V search(Node* pRoot, K key, bool& found);
    int height(Node* pRoot);
    void ascendingList(Node* pRoot, DLinkedList<V>& list);
    void descendingList(Node* pRoot, DLinkedList<V>& list);
    void nlr(Node* pRoot, DLinkedList<V>& list);
    void lrn(Node* pRoot, DLinkedList<V>& list);
    void lnr(Node* pRoot, DLinkedList<V>& list);
    void select(Node* pRoot, K min, K max, DLinkedList<V>& list);
    
    static void remove(Node* pRoot){
        if(pRoot == 0) return;
        if(pRoot->pLeft != 0) remove(pRoot->pLeft);
        if(pRoot->pRight != 0) remove(pRoot->pRight);
        delete pRoot;
    }
    
    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
    
public:
    class Entry{
    public:
        K key; //primitive type
        V value; //pointer always
        friend class BST<K, V>;

    public:
        Entry(K key, V value=0){
            this->key = key;
            this->value = value;
        }
        string toString(string (*entry2str)(K&,V&)=0){
            if(entry2str != 0) return entry2str(key, value);
            else{
                stringstream entryos;
                if(value != 0)
                    entryos << "<" << key << ", " << *(value) << ">";
                else entryos << key;
                return entryos.str();
            }
        }
        bool operator==(Entry rhs){
            return this->key == rhs.key;
        }
    };

    class Node{
    public:
        static int LH, EH, RH, EMPTY;
        int bfactor; //not used in BST, be used in AVL
        int height_;
        Entry data;
        Node *pLeft, *pRight;
        
        friend class BST<K, V>;
    public:
        Node():data(0,0){
            this->pLeft = 0;
            this->pRight = 0;
            this->bfactor = EMPTY;
            this->height_ = 1;
        }
        Node(Entry newdata, Node* pLeft=0, Node* pRight=0): data(newdata){
            this->pLeft = pLeft;
            this->pRight = pRight;
            this->bfactor = EH;
            this->height_ = 1;
        }
        ~Node(){
            if(data.value != 0) delete data.value;
        }
        void updateHeight(){
            int leftH =0, rightH=0;
            if(pLeft != 0) leftH = pLeft->height();
            if(pRight != 0) rightH = pRight->height();
            height_ = 1 + max(leftH, rightH);
            //
            bfactor = rightH - leftH;
        }
        int height(){ return height_; }
        
        bool isLH(){ return bfactor <= LH; }
        void markLH() { bfactor = LH; }
        bool isEH(){ return bfactor == EH; }
        void markEH() { bfactor = EH; }
        bool isRH(){ return bfactor >= RH; }
        void markRH() { bfactor = RH; }
        bool isBalanced(){ return (bfactor >= -1) && (bfactor <= +1); }
        string balance(){
            if(isLH()) return "LH ";
            else if(isEH()) return "EH ";
            else return "RH ";
        }
        
        string toString(string (*entry2str)(K&,V&)=0, bool avl=false){
            stringstream nodestr;
            if(avl){
                if((pLeft == 0) && (pRight == 0))
                    nodestr << "[" << data.toString(entry2str) << ":" << balance() << "]";
                if((pLeft == 0) && (pRight != 0))
                    nodestr << " (" 
                            << data.toString(entry2str) << ":" << balance() << "[.]" 
                            << pRight->toString(entry2str, avl)
                            << ")";
                if((pLeft != 0) && (pRight == 0))
                    nodestr << " (" 
                            << data.toString(entry2str) << ":" << balance()
                            << pLeft->toString(entry2str, avl)
                            << "[.]"
                            << ")";
                if((pLeft != 0) && (pRight != 0))
                    nodestr << " (" 
                            << data.toString(entry2str)  << ":" << balance()
                            << pLeft->toString(entry2str, avl)
                            << pRight->toString(entry2str, avl)
                            << ") ";
            }
            else{
                if((pLeft == 0) && (pRight == 0))
                    nodestr << "[" << data.toString(entry2str) << "]";
                if((pLeft == 0) && (pRight != 0))
                    nodestr << " (" 
                            << data.toString(entry2str) << "[.]" 
                            << pRight->toString(entry2str)
                            << ")";
                if((pLeft != 0) && (pRight == 0))
                    nodestr << " (" 
                            << data.toString(entry2str) 
                            << pLeft->toString(entry2str)
                            << "[.]"
                            << ")";
                if((pLeft != 0) && (pRight != 0))
                    nodestr << " (" 
                            << data.toString(entry2str) 
                            << pLeft->toString(entry2str)
                            << pRight->toString(entry2str)
                            << ") ";
            }
            return nodestr.str();
        }
    };
};

template<class K, class V>
int BST<K,V>::Node::LH = nLH;
template<class K, class V>
int BST<K,V>::Node::EH = nEH;
template<class K, class V>
int BST<K,V>::Node::RH = nRH;
template<class K, class V>
int BST<K,V>::Node::EMPTY = nEMPTY;

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template<class K, class V>
BST<K, V>::BST() {
    count = 0;
    pRoot = 0;
}


template<class K, class V>
BST<K, V>::~BST() {
    remove(this->pRoot);
}


template<class K, class V>
void BST<K, V>::add(K key, V value) {
    // dem so nut truoc de so sanh sau
    int soNutTruoc = count;
    pRoot = add(pRoot, Entry(key, value));
    if (count > soNutTruoc) {
        // da them thanh cong, khong lam gi them
    }
}


template<class K, class V>
typename BST<K, V>::Node* BST<K, V>::add(Node* pRoot, Entry data) {
    // Neu cay rong thi tao nut moi
    if (pRoot == 0) {
        count = count + 1;
        return new Node(data);
    }
    
    // So sanh key de chon huong di
    if (data.key < pRoot->data.key) {
        pRoot->pLeft = add(pRoot->pLeft, data);
    } else if (data.key > pRoot->data.key) {
        pRoot->pRight = add(pRoot->pRight, data);
    }
    // Neu key bang nhau thi bo qua (duplicate)
    
    return pRoot;
}


template<class K, class V>
int BST<K, V>::size(){
    return count;
}


template<class K, class V>
V BST<K, V>::remove(K key, bool* success){
    V retValue = 0;
    bool success_ = false;
    this->pRoot = remove(this->pRoot, key, success_, retValue);
    if(success_) this->count -= 1;
    if(success != 0) *success = success_;
    
    return retValue;
}


template<class K, class V>
typename BST<K, V>::Node* BST<K, V>::remove(
    Node* pRoot, K key, bool& success, V& retValue) {
    // Khong tim thay
    if (pRoot == 0) {
        success = false;
        return 0;
    }
    
    if (key < pRoot->data.key) {
        // Di sang trai
        pRoot->pLeft = remove(pRoot->pLeft, key, success, retValue);
    } else if (key > pRoot->data.key) {
        // Di sang phai
        pRoot->pRight = remove(pRoot->pRight, key, success, retValue);
    } else {
        // Tim thay nut can xoa
        success = true;
        retValue = pRoot->data.value;
        pRoot->data.value = 0; // chuyen quyen so huu sang caller
        
        if (pRoot->pLeft == 0 && pRoot->pRight == 0) {
            // La: xoa truc tiep
            delete pRoot;
            return 0;
        } else if (pRoot->pLeft == 0) {
            // Chi co con phai
            Node* conPhai = pRoot->pRight;
            pRoot->pRight = 0;
            delete pRoot;
            return conPhai;
        } else if (pRoot->pRight == 0) {
            // Chi co con trai
            Node* conTrai = pRoot->pLeft;
            pRoot->pLeft = 0;
            delete pRoot;
            return conTrai;
        } else {
            // Co ca 2 con: tim nut nho nhat trong cay con phai (inorder successor)
            Node* pTam = pRoot->pRight;
            while (pTam->pLeft != 0) {
                pTam = pTam->pLeft;
            }
            // Copy key tu inorder successor len nut goc, giu nguyen value cua goc
            // roi xoa inorder successor (no chi co toi da 1 con phai)
            K keyKeTiep = pTam->data.key;
            bool okTam = false;
            V retTam = 0;
            pRoot->pRight = remove(pRoot->pRight, keyKeTiep, okTam, retTam);
            // Giu retValue la value cua nut hien tai (da luu o tren)
            // Thay the key cua pRoot bang key cua successor
            pRoot->data.key = keyKeTiep;
            // Dat lai value cua pRoot bang value cua successor (bo sung)
            // Phai hoan tra gia tri cua pRoot cho caller, dat gia tri cua successor vao pRoot
            V giaTamRetTam = retTam;
            // retValue da duoc gan o tren (la gia tri goc cua pRoot)
            // Con pRoot->data.value bay gio la gia tri cua pRoot cu
            // Ta can dat gia tri cua successor vao pRoot thay vi xoa no
            // Nhung successor's value da bi hoan tra qua retTam...
            // => Dat lai vao node
            pRoot->data.value = giaTamRetTam;
        }
    }
    
    return pRoot;
}


template<class K, class V>
V BST<K, V>::search(K key, bool& found){
    found = false;
    return search(this->pRoot, key, found);
}

template<class K, class V>
V BST<K, V>::search(Node* pRoot, K key, bool& found) {
    // Neu cay rong thi khong tim thay
    if (pRoot == 0) {
        found = false;
        return V{};
    }
    
    if (key == pRoot->data.key) {
        found = true;
        return pRoot->data.value;
    } else if (key < pRoot->data.key) {
        return search(pRoot->pLeft, key, found);
    } else {
        return search(pRoot->pRight, key, found);
    }
}


template<class K, class V>
int BST<K, V>::height(){
    return height(pRoot);
}

template<class K, class V>
int BST<K, V>::height(Node* pRoot){
    if(pRoot == 0) return 0;
    else{
        int leftH = height(pRoot->pLeft);
        int rightH = height(pRoot->pRight);
        return 1 + (leftH > rightH ? leftH : rightH);
    }
}
template<class K, class V>
void BST<K, V>::clear(){
    remove(this->pRoot);
    this->pRoot = 0;
    this->count = 0;
}

template<class K, class V>
DLinkedList<V> BST<K, V>::ascendingList(){
    DLinkedList<V> list;
    ascendingList(pRoot, list);
    return list;
}

template<class K, class V>
void BST<K, V>::ascendingList(Node* pRoot, DLinkedList<V>& list){
    if(pRoot == 0) return;
    ascendingList(pRoot->pLeft, list);
    list.add(pRoot->data.value);
    ascendingList(pRoot->pRight, list);
}

template<class K, class V>
DLinkedList<V> BST<K, V>::descendingList(){
    DLinkedList<V> list;
    descendingList(pRoot, list);
    return list;
}

template<class K, class V>
void BST<K, V>::descendingList(Node* pRoot, DLinkedList<V>& list){
    if(pRoot == 0) return;
    descendingList(pRoot->pRight, list);
    list.add(pRoot->data.value);
    descendingList(pRoot->pLeft, list);
}

template<class K, class V>
DLinkedList<V> BST<K, V>::dfs(){
    DLinkedList<V> list;
    if(pRoot == 0) return list;
    
    Stack<Node*> stack;
    stack.push(pRoot);
    while(!stack.empty()){
        Node* pNode = stack.pop();
        list.add(pNode->data.value);
        if(pNode->pRight != 0) stack.push(pNode->pRight);
        if(pNode->pLeft != 0) stack.push(pNode->pLeft);
    }
    return list;
}

template<class K, class V>
DLinkedList<V> BST<K, V>::bfs(){
    DLinkedList<V> list;
    if(pRoot == 0) return list;
    
    Queue<Node*> queue;
    queue.push(pRoot);
    while(!queue.empty()){
        Node* pNode = queue.pop();
        list.add(pNode->data.value);
        if(pNode->pLeft != 0) queue.push(pNode->pLeft);
        if(pNode->pRight != 0) queue.push(pNode->pRight);
    }
    return list;
}
template<class K, class V>
DLinkedList<K> BST<K, V>::bfsKey(){
    DLinkedList<K> list;
    if(pRoot == 0) return list;
    
    Queue<Node*> queue;
    queue.push(pRoot);
    while(!queue.empty()){
        Node* pNode = queue.pop();
        list.add(pNode->data.key);
        if(pNode->pLeft != 0) queue.push(pNode->pLeft);
        if(pNode->pRight != 0) queue.push(pNode->pRight);
    }
    return list;
}

template<class K, class V>
DLinkedList<V> BST<K, V>::nlr(){
    DLinkedList<V> list;
    nlr(pRoot, list);
    return list;
}
template<class K, class V>
void BST<K, V>::nlr(Node* pRoot, DLinkedList<V>& list){
    if(pRoot == 0) return;
    list.add(pRoot->data.value);
    nlr(pRoot->pLeft, list);
    nlr(pRoot->pRight, list);
}

template<class K, class V>
DLinkedList<V> BST<K, V>::lrn(){
    DLinkedList<V> list;
    lrn(pRoot, list);
    return list;
}
template<class K, class V>
void BST<K, V>::lrn(Node* pRoot, DLinkedList<V>& list){
    if(pRoot == 0) return;
    lrn(pRoot->pLeft, list);
    lrn(pRoot->pRight, list);
    list.add(pRoot->data.value);
}

template<class K, class V>
DLinkedList<V> BST<K, V>::lnr(){
    DLinkedList<V> list;
    lnr(pRoot, list);
    return list;
}
template<class K, class V>
void BST<K, V>::lnr(Node* pRoot, DLinkedList<V>& list){
    if(pRoot == 0) return;
    lnr(pRoot->pLeft, list);
    list.add(pRoot->data.value);
    lnr(pRoot->pRight, list);
}

template<class K, class V>
void BST<K, V>::select(Node* pRoot, K min, K max, DLinkedList<V>& list){
    if(pRoot == 0) return;
    
    if(pRoot->data.key < min) select(pRoot->pRight, min, max, list);
    else if(pRoot->data.key == min){
        //key: == min
        list.add(pRoot->data.value);
        select(pRoot->pRight, min, max, list);
    }
    else if(pRoot->data.key <= max){
        // key: (min, max]
        select(pRoot->pLeft, min, max, list);
        list.add(pRoot->data.value);
        select(pRoot->pRight, min, max, list);
    }
    else{
        //key: (max, inf)
        select(pRoot->pLeft, min, max, list);
    }
}

template<class K, class V>
DLinkedList<V> BST<K, V>::select(K min, K max){
    DLinkedList<V> list;
    select(this->pRoot, min, max, list);
    return list;
}


#endif /* BST_H */

