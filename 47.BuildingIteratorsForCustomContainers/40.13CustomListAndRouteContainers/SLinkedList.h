#ifndef SLINKEDLIST_H
#define SLINKEDLIST_H

#include "IList.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
using namespace std;

template<class T>
class SLinkedList : public IList<T> {
public:
    class Iterator;
    class Node;

protected:
    Node* head;
    Node* tail;
    int count;
    bool (*itemEqual)(T& lhs, T& rhs);
    void (*deleteUserData)(SLinkedList<T>*);

public:
    SLinkedList(
        void (*deleteUserData)(SLinkedList<T>*) = 0,
        bool (*itemEqual)(T&, T&) = 0
    ) {
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->next = head;
        count = 0;
        this->itemEqual = itemEqual;
        this->deleteUserData = deleteUserData;
    }

    SLinkedList(const SLinkedList<T>& list) {
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->next = head;
        count = 0;
        itemEqual = list.itemEqual;
        deleteUserData = list.deleteUserData;
        copyFrom(list);
    }

    SLinkedList<T>& operator=(const SLinkedList<T>& list) {
        if (this == &list) return *this;
        removeInternalData();
        head->next = tail;
        tail->next = head;
        count = 0;
        itemEqual = list.itemEqual;
        deleteUserData = list.deleteUserData;
        copyFrom(list);
        return *this;
    }

    ~SLinkedList() {
        removeInternalData();
        delete head;
        delete tail;
    }

    void add(T e) override {
        Node* nutCuoi = tail->next;
        Node* nutMoi = new Node(e, tail);
        nutCuoi->next = nutMoi;
        tail->next = nutMoi;
        count++;
    }

    void add(int index, T e) override {
        if (index < 0 || index > count) {
            throw out_of_range("Index sai");
        }
        if (index == count) {
            add(e);
            return;
        }
        
        Node* pTam = head;
        int i = 0;
        while (i < index) {
            pTam = pTam->next;
            i = i + 1;
        }
        
        Node* nutMoi = new Node(e, pTam->next);
        pTam->next = nutMoi;
        count = count + 1;
    }

    T removeAt(int index) override {
        if (index < 0 || index >= count) {
            throw out_of_range("Index sai roi");
        }
        
        Node* pTruoc = head;
        for (int i = 0; i < index; i++) {
            pTruoc = pTruoc->next;
        }
        
        Node* pXoa = pTruoc->next;
        pTruoc->next = pXoa->next;
        
        if (pXoa == tail->next) { // cap nhat cai tail->next neu xoa ngay duoi
            tail->next = pTruoc;
        }
        
        T kq = pXoa->data;
        delete pXoa;
        count--;
        
        return kq;
    }

    bool removeItem(T item, void (*removeItemData)(T) = 0) override {
        Node* pTruoc = head;
        while (pTruoc->next != tail) {
            if (equals(pTruoc->next->data, item, itemEqual)) {
                Node* pXoa = pTruoc->next;
                pTruoc->next = pXoa->next;
                
                if (pXoa == tail->next) {
                    tail->next = pTruoc;
                }
                
                if (removeItemData != 0) {
                    removeItemData(pXoa->data);
                }
                
                delete pXoa;
                count = count - 1;
                return true;
            }
            pTruoc = pTruoc->next;
        }
        return false;
    }

    void clear() override {
        removeInternalData();
        head->next = tail;
        tail->next = head; 
        count = 0;
    }

    T& get(int index) override {
        if (index < 0 || index >= count) throw out_of_range("Index sai");
        
        Node* pTam = head->next;
        int i = 0;
        while (i < index) {
            pTam = pTam->next;
            i++;
        }
        return pTam->data;
    }

    int indexOf(T item) override {
        Node* pTam = head->next;
        int vitri = 0;
        
        while (pTam != tail) {
            if (equals(pTam->data, item, itemEqual)) {
                return vitri;
            }
            pTam = pTam->next;
            vitri++;
        }
        return -1;
    }

    bool empty() override { return count == 0; }
    int size() override { return count; }

    bool contains(T item) override {
        return indexOf(item) >= 0;
    }

    string toString(string (*item2str)(T&) = 0) override {
        stringstream ss;
        ss << "[";
        Node* cur = head->next;
        bool first = true;
        while (cur != tail) {
            if (!first) ss << ", ";
            if (item2str) ss << item2str(cur->data);
            else ss << cur->data;
            first = false;
            cur = cur->next;
        }
        ss << "]";
        return ss.str();
    }

    void println(string (*item2str)(T&) = 0) {
        cout << toString(item2str) << endl;
    }

    void setDeleteUserDataPtr(void (*deleteUserData)(SLinkedList<T>*) = 0) {
        this->deleteUserData = deleteUserData;
    }

    Iterator begin() { return Iterator(this, true); }
    Iterator end() { return Iterator(this, false); }

    static void free(SLinkedList<T>* list) {
        Iterator it = list->begin();
        while (it != list->end()) {
            delete *it;
            it++;
        }
    }

protected:
    static bool equals(T& lhs, T& rhs, bool (*itemEqual)(T&, T&)) {
        return itemEqual ? itemEqual(lhs, rhs) : (lhs == rhs);
    }

    void copyFrom(const SLinkedList<T>& list) {
        Node* src = list.head->next;
        while (src != list.tail) {
            add(src->data);
            src = src->next;
        }
    }

    void removeInternalData() {
        if (deleteUserData != 0) deleteUserData(this);
        Node* cur = head->next;
        while (cur != tail) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
    }

public:
    class Node {
    public:
        T data;
        Node* next;

        Node(Node* next = 0) : next(next) {}
        Node(T data, Node* next = 0) : data(data), next(next) {}
    };

    class Iterator {
    private:
        SLinkedList<T>* pList;
        Node* pNode;

    public:
        Iterator(SLinkedList<T>* pList = 0, bool begin = true) {
            this->pList = pList;
            if (pList == 0) pNode = 0;
            else pNode = begin ? pList->head->next : pList->tail;
        }

        Iterator& operator=(const Iterator& iterator) {
            pNode = iterator.pNode;
            pList = iterator.pList;
            return *this;
        }

        void remove(void (*removeItemData)(T) = 0) {
            if (pList == 0 || pNode == pList->tail) return;

            Node* prev = pList->head;
            int index = 0;
            while (prev->next != pList->tail && prev->next != pNode) {
                prev = prev->next;
                ++index;
            }
            if (prev->next != pNode) return;

            T removed = pList->removeAt(index);
            if (removeItemData != 0) removeItemData(removed);
            pNode = prev;
        }

        T& operator*() { return pNode->data; }
        bool operator!=(const Iterator& iterator) { return pNode != iterator.pNode; }
        Iterator& operator++() { pNode = pNode->next; return *this; }
        Iterator operator++(int) { Iterator old = *this; ++(*this); return old; }
    };
};

#endif
