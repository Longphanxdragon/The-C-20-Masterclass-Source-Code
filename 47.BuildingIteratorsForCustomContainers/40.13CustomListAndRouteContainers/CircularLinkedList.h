#ifndef CIRCULARLINKEDLIST_H
#define CIRCULARLINKEDLIST_H

#include "IList.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
using namespace std;

template<class T>
class CircularLinkedList : public IList<T> {
public:
    class Node;

protected:
    Node* head;
    Node* tail;
    int count;
    bool (*itemEqual)(T& lhs, T& rhs);
    void (*deleteUserData)(CircularLinkedList<T>*);

public:
    CircularLinkedList(
        void (*deleteUserData)(CircularLinkedList<T>*) = 0,
        bool (*itemEqual)(T&, T&) = 0
    )
        : head(nullptr), tail(nullptr), count(0),
          itemEqual(itemEqual), deleteUserData(deleteUserData) {}

    CircularLinkedList(const CircularLinkedList<T>& list)
        : head(nullptr), tail(nullptr), count(0),
          itemEqual(list.itemEqual), deleteUserData(list.deleteUserData) {
        copyFrom(list);
    }

    CircularLinkedList<T>& operator=(const CircularLinkedList<T>& list) {
        if (this == &list) return *this;
        removeInternalData();
        itemEqual = list.itemEqual;
        deleteUserData = list.deleteUserData;
        copyFrom(list);
        return *this;
    }

    ~CircularLinkedList() {
        removeInternalData();
    }

    void add(T e) override {
        Node* nutMoi = new Node(e);
        if (count == 0) {
            head = nutMoi;
            tail = nutMoi;
            nutMoi->next = nutMoi;
        } else {
            tail->next = nutMoi;
            nutMoi->next = head;
            tail = nutMoi;
        }
        count++;
    }

    void add(int index, T e) override {
        if (index < 0 || index > count) {
            throw out_of_range("Loi index roi");
        }
        
        if (index == count) {
            add(e);
            return;
        }
        
        Node* nutMoi = new Node(e);
        if (index == 0) {
            nutMoi->next = head;
            head = nutMoi;
            tail->next = head;
        } else {
            Node* pTam = head;
            for (int i = 0; i < index - 1; i++) {
                pTam = pTam->next;
            }
            nutMoi->next = pTam->next;
            pTam->next = nutMoi;
        }
        count++;
    }

    T removeAt(int index) override {
        if (index < 0 || index >= count) {
            throw out_of_range("Index sai nhe");
        }
        
        Node* nutXoa = head;
        if (index == 0) {
            if (count == 1) {
                head = nullptr;
                tail = nullptr;
            } else {
                head = head->next;
                tail->next = head;
            }
        } else {
            Node* pTruoc = head;
            for (int i = 0; i < index - 1; i++) {
                pTruoc = pTruoc->next;
            }
            nutXoa = pTruoc->next;
            pTruoc->next = nutXoa->next;
            
            if (index == count - 1) {
                tail = pTruoc;
            }
        }
        
        T kqua = nutXoa->data;
        delete nutXoa;
        count--;
        
        return kqua;
    }

    bool removeItem(T item, void (*removeItemData)(T) = 0) override {
        if (count == 0) return false;
        
        Node* hienTai = head;
        Node* pTruoc = tail;
        
        for (int i = 0; i < count; i++) {
            if (equals(hienTai->data, item, itemEqual)) {
                if (count == 1) {
                    head = nullptr;
                    tail = nullptr;
                } else {
                    pTruoc->next = hienTai->next;
                    if (hienTai == head) {
                        head = hienTai->next;
                    }
                    if (hienTai == tail) {
                        tail = pTruoc;
                    }
                }
                
                if (removeItemData != nullptr) {
                    removeItemData(hienTai->data);
                }
                
                delete hienTai;
                count--;
                return true;
            }
            pTruoc = hienTai;
            hienTai = hienTai->next;
        }
        return false;
    }

    void clear() override {
        removeInternalData();
    }

    T& get(int index) override {
        if (index < 0 || index >= count) {
            throw out_of_range("Loi index!");
        }
        
        Node* pTam = head;
        for (int i = 0; i < index; i++) {
            pTam = pTam->next;
        }
        return pTam->data;
    }

    int indexOf(T item) override {
        if (count == 0) return -1;
        
        Node* pTam = head;
        for (int i = 0; i < count; i++) {
            if (equals(pTam->data, item, itemEqual)) {
                return i;
            }
            pTam = pTam->next;
        }
        return -1;
    }

    bool empty() override { return count == 0; }
    int size() override { return count; }
    bool contains(T item) override { return indexOf(item) >= 0; }

    string toString(string (*item2str)(T&) = 0) override {
        stringstream ss;
        ss << "[";
        Node* cur = head;
        for (int i = 0; i < count; ++i) {
            if (i > 0) ss << ", ";
            if (item2str) ss << item2str(cur->data);
            else ss << cur->data;
            cur = cur->next;
        }
        ss << "]";
        return ss.str();
    }

    void println(string (*item2str)(T&) = 0) {
        cout << toString(item2str) << endl;
    }

protected:
    static bool equals(T& lhs, T& rhs, bool (*itemEqual)(T&, T&)) {
        return itemEqual ? itemEqual(lhs, rhs) : (lhs == rhs);
    }

    void copyFrom(const CircularLinkedList<T>& list) {
        Node* cur = list.head;
        for (int i = 0; i < list.count; ++i) {
            add(cur->data);
            cur = cur->next;
        }
    }

    void removeInternalData() {
        if (deleteUserData != 0) deleteUserData(this);
        Node* cur = head;
        for (int i = 0; i < count; ++i) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        head = tail = nullptr;
        count = 0;
    }

public:
    class Node {
    public:
        T data;
        Node* next;
        Node(T data, Node* next = nullptr) : data(data), next(next) {}
    };
};

#endif
