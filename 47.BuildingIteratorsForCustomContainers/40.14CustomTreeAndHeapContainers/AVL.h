/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AVL.h
 * Author: LTSACH
 *
 * Created on 12 September 2020, 17:11
 */

#ifndef AVL_H
#define AVL_H
#include "BST.h"
#include "support/DLinkedList.h"
#include "support/Queue.h"

#define XNode typename BST<K,V>::Node


template<class K, class V>
class AVL : public BST<K,V>{
public:
    void add(K key, V value=0){
        // Tao nut moi roi them vao cay, sau do cap nhat height va rebalance
        bool keyTonTai = false;
        this->search(key, keyTonTai);
        if (keyTonTai == true) {
            return; // bo qua neu key da ton tai
        }
        
        XNode* nutMoi = new XNode(typename BST<K,V>::Entry(key, value));
        this->pRoot = add(this->pRoot, nutMoi);
        this->count = this->count + 1;
    }

    V remove(K key, bool* success=0){
        V retValue = 0; //O: NULL
        bool success_ = false;
        this->pRoot = remove(this->pRoot, key, success_, retValue);
        if(success_) this->count -= 1;
        //if(success != 0) if (success != 0) *success = success_;
        if (success != 0) *success = success_;
        return retValue;
    }
    int height(){
        if(this->pRoot == 0) return 0;
        return this->pRoot->height();
    }
    void println(string (*entry2str)(K&,V&)=0){
        cout << "Tree height: " << this->height() << endl;
        cout << "Tree nodes:"  << endl;
        cout << this->toString(entry2str, true) << endl;
    }
    /*
     * bfactor: designed for being used to test AVL tree
     *          so, maybe not useful for real application
     * It returns a list of balance factor (string) of nodes visited with BFS-traversal
     * (see AVLTest for example)
     */
    DLinkedList<string> bfactor(){
        DLinkedList<string> list;
        if(this->pRoot == 0) return list;
        
        Queue<XNode*> queue;
        queue.push(this->pRoot);
        while(!queue.empty()){
            XNode* pNode = queue.pop();
            if(pNode->bfactor == nEMPTY) list.add("EMPTY");
            else if(pNode->bfactor == nLH) list.add("LH");
            else if(pNode->bfactor == nEH) list.add("EH");
            else list.add("RH");
            
            if(pNode->bfactor == nEMPTY){
                delete pNode; //because alloc in [1] or [2]
                continue;
            } 
            if((pNode->pLeft == 0) && (pNode->pRight == 0) ) continue;
            
            if(pNode->pLeft != 0) queue.push(pNode->pLeft);
            else queue.push(new XNode()); //[1]
            if(pNode->pRight != 0) queue.push(pNode->pRight);
            else queue.push(new XNode()); //[2]
        }
        return list;
    }
private:
    // Xoay trai: con phai len lam goc moi
    XNode* rotateLeft(XNode* root){
        XNode* conPhai = root->pRight;
        root->pRight = conPhai->pLeft;
        conPhai->pLeft = root;
        
        root->updateHeight();
        conPhai->updateHeight();
        
        return conPhai;
    }
    // Xoay phai: con trai len lam goc moi
    XNode* rotateRight(XNode* root){
        XNode* conTrai = root->pLeft;
        root->pLeft = conTrai->pRight;
        conTrai->pRight = root;
        
        root->updateHeight();
        conTrai->updateHeight();
        
        return conTrai;
    }
    // Kiem tra can bang va xoay neu can
    XNode* rebalance(XNode* root){
        root->updateHeight();
        int bf = root->bfactor;
        
        if (bf < -1) {
            // Lech trai
            if (root->pLeft->bfactor > 0) {
                // LR case: xoay trai con trai truoc
                root->pLeft = rotateLeft(root->pLeft);
            }
            // LL case
            return rotateRight(root);
        }
        
        if (bf > 1) {
            // Lech phai
            if (root->pRight->bfactor < 0) {
                // RL case: xoay phai con phai truoc
                root->pRight = rotateRight(root->pRight);
            }
            // RR case
            return rotateLeft(root);
        }
        
        return root; // Da can bang roi
    }
    // Them nut moi vao AVL tree, tu dong rebalance
    XNode* add(XNode* root, XNode* newNode){
        if (root == 0) {
            return newNode;
        }
        
        if (newNode->data.key < root->data.key) {
            root->pLeft = add(root->pLeft, newNode);
        } else if (newNode->data.key > root->data.key) {
            root->pRight = add(root->pRight, newNode);
        } else {
            // Key trung lap, bo qua
            return root;
        }
        
        return rebalance(root);
    }
    // Xoa nut trong AVL tree
    XNode* remove(XNode* root, K key, bool& success, V& retValue){
        if (root == 0) {
            success = false;
            return 0;
        }
        
        if (key < root->data.key) {
            root->pLeft = remove(root->pLeft, key, success, retValue);
        } else if (key > root->data.key) {
            root->pRight = remove(root->pRight, key, success, retValue);
        } else {
            // Tim thay nut can xoa
            success = true;
            retValue = root->data.value;
            root->data.value = 0; // chuyen quyen so huu
            
            if (root->pLeft == 0 && root->pRight == 0) {
                delete root;
                return 0;
            } else if (root->pLeft == 0) {
                XNode* pTam = root->pRight;
                root->pRight = 0;
                delete root;
                return pTam;
            } else if (root->pRight == 0) {
                XNode* pTam = root->pLeft;
                root->pLeft = 0;
                delete root;
                return pTam;
            } else {
                // 2 con: tim inorder successor (nho nhat ben phai)
                XNode* pKeTiep = root->pRight;
                while (pKeTiep->pLeft != 0) {
                    pKeTiep = pKeTiep->pLeft;
                }
                // Lay key cua inorder successor roi xoa no
                K keyKeTiep = pKeTiep->data.key;
                
                // Xoa inorder successor (no chi co toi da 1 con)
                bool okTam = false;
                V retTam = 0;
                root->pRight = remove(root->pRight, keyKeTiep, okTam, retTam);
                
                // Thay the key cua root bang key cua successor
                // retValue da luu san gia tri cua root can xoa o dong 170
                root->data.key = keyKeTiep;
                root->data.value = retTam;
            }
        }
        
        return rebalance(root);
    }
};

#endif /* AVL_H */
