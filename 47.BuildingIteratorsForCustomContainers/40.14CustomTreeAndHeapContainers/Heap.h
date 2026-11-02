/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Heap.h
 * Author: LTSACH
 *
 * Created on 22 August 2020, 18:18
 */

#ifndef HEAP_H
#define HEAP_H
#include <memory.h>
#include <stdexcept>
#include <limits>
#include "support/IHeap.h"
#include <sstream>
#include <iostream>
/*
 * function pointer: int (*comparator)(T& lhs, T& rhs)
 *      compares objects of type T given in lhs and rhs.
 *      return: sign of (lhs - rhs)
 *              -1: of lhs < rhs
 *              0 : lhs == rhs
 *              +1: ls > rhs
 * 
 * function pointer: void (*deleteUserData)(Heap<T>* pHeap)
 *      remove user's data in case that T is a pointer type
 *      Users should pass &Heap<T>::free for "deleteUserData"
 * 
 */
template<class T>
class Heap: public IHeap<T>{
public:
    class Iterator; //forward declaration
    
protected:
    T *elements;    //a dynamic array to contain user's data
    int capacity;   //size of the dynamic array
    int count;      //current count of elements stored in this heap
    int (*comparator)(T& lhs, T& rhs);      //see above
    void (*deleteUserData)(Heap<T>* pHeap); //see above
    
public:
    Heap(   int (*comparator)(T& , T&)=0, 
            void (*deleteUserData)(Heap<T>*)=0 );
    
    Heap(const Heap<T>& heap); //copy constructor 
    Heap<T>& operator=(const Heap<T>& heap); //assignment operator
    
    ~Heap();
    
    //Inherit from IHeap: BEGIN
    void push(T item);
    T pop();
    const T peek();
    void remove(T item, void (*removeItemData)(T)=0);
    bool contains(T item);
    int size();
    void heapify(T array[], int size);
    void clear();
    bool empty();
    string toString(string (*item2str)(T&)=0 );
    //Inherit from IHeap: END
    
    void println(string (*item2str)(T&)=0 ){
        cout << toString(item2str) << endl;
    }
    
    Iterator begin(){
        return Iterator(this, true);
    }
    Iterator end(){
        return Iterator(this, false);
    }
    
public:
    /* if T is pointer type:
     *     pass the address of method "free" to Heap<T>'s constructor:
     *     to:  remove the user's data (if needed)
     * Example:
     *  Heap<Point*> heap(&Heap<Point*>::free);
     *  => Destructor will call free via function pointer "deleteUserData"
     */
    static void free(Heap<T> *pHeap){
        for(int idx=0; idx < pHeap->count; idx++) delete pHeap->elements[idx];
    }
    
    
private:
    bool aLTb(T& a, T& b){
        return compare(a, b) < 0;
    }
    int compare(T& a, T& b){
        if(comparator != 0) return comparator(a, b);
        else{
            if (a < b) return -1;
            else if(a > b) return 1;
            else return 0;
        }
    }
    
    void ensureCapacity(int minCapacity); 
    void swap(int a, int b);
    void reheapUp(int position);
    void reheapDown(int position);
    int getItem(T item);
    
    void removeInternalData();
    void copyFrom(const Heap<T>& heap);
    
    
//////////////////////////////////////////////////////////////////////
////////////////////////  INNER CLASSES DEFNITION ////////////////////
//////////////////////////////////////////////////////////////////////
    
public:
    
    //Iterator: BEGIN
    class Iterator{
    private:
        Heap<T>* heap;
        int cursor;
    public:
        Iterator(Heap<T>* heap=0, bool begin=0){
            this->heap = heap;
            if(begin && (heap !=0)) cursor = 0;
            if(!begin && (heap !=0)) cursor = heap->size();
        }
        Iterator& operator=(const Iterator& iterator){
            this->heap = iterator.heap;
            this->cursor = iterator.cursor;
            return *this;
        }
        
        T& operator*(){
            return this->heap->elements[cursor];
        }
        bool operator!=(const Iterator& iterator){
            return this->cursor != iterator.cursor;
        }
        // Prefix ++ overload 
        Iterator& operator++(){
            cursor++;
            return *this; 
        }
        // Postfix ++ overload 
        Iterator operator++(int){
            Iterator iterator = *this; 
            ++*this; 
            return iterator; 
        }
        void remove(void (*removeItemData)(T)=0){
            this->heap->remove(this->heap->elements[cursor], removeItemData);
        }
    };
    //Iterator: END
};


//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
Heap<T>::Heap(
        int (*comparator)(T&, T&), 
        void (*deleteUserData)(Heap<T>* ) ){
    capacity = 10;
    count = 0;
    elements = new T[capacity];
    this->comparator = comparator;
    this->deleteUserData = deleteUserData;
}
template<class T>
Heap<T>::Heap(const Heap<T>& heap){
    copyFrom(heap);
}

template<class T>
Heap<T>& Heap<T>::operator=(const Heap<T>& heap){
    if (this==&heap) return *this;
    removeInternalData();
    copyFrom(heap);
    return *this;
}


template<class T>
Heap<T>::~Heap(){
    removeInternalData();
}

template<class T>
void Heap<T>::push(T item){
    // Dam bao du cho truoc khi them
    ensureCapacity(count + 1);
    elements[count] = item;
    count = count + 1;
    // Day phan tu moi len dung vi tri
    reheapUp(count - 1);
}


template<class T>
T Heap<T>::pop(){
    if (count == 0) throw std::underflow_error("pop on an empty heap");
    
    // Lay phan tu o goc (uu tien cao nhat)
    T kqua = elements[0];
    
    // Dua phan tu cuoi len goc roi reheap down
    count = count - 1;
    if (count > 0) {
        elements[0] = elements[count];
        reheapDown(0);
    }
    
    return kqua;
}


/*
      15
     /  \
    18   13
 => ReheapDown
      18
     /  \
    15   13
=> Array: [18, 15, 13, , , ]
 */

template<class T>
const T Heap<T>::peek(){
    if (count == 0) throw std::underflow_error("peek on an empty heap");
    return elements[0];
}


template<class T>
void Heap<T>::remove(T item, void (*removeItemData)(T)){
    // Tim vi tri cua phan tu can xoa
    int viTri = getItem(item);
    if (viTri == -1) {
        return; // Khong tim thay, khong lam gi
    }
    
    // Goi callback neu co
    if (removeItemData != 0) {
        removeItemData(elements[viTri]);
    }
    
    // Dua phan tu cuoi len vi tri can xoa
    count = count - 1;
    elements[viTri] = elements[count];
    
    if (viTri < count) {
        // Kiem tra ca 2 chieu: up va down
        reheapUp(viTri);
        reheapDown(viTri);
    }
}


template<class T>
bool Heap<T>::contains(T item){
    bool found = false;
    for(int idx=0; idx < count; idx++){
        if(compare(elements[idx], item) == 0){
            found = true;
            break;
        }
    }
    return found;
}

template<class T>
int Heap<T>::size(){
    return count;
}

template<class T>
void Heap<T>::heapify(T array[], int size){
    if (size < 0) throw std::invalid_argument("negative heap size");
    
    // Xoa heap hien tai truoc
    removeInternalData();
    capacity = size > 10 ? size : 10;
    count = size;
    elements = new T[capacity];
    
    // Copy mang vao
    for (int i = 0; i < size; i++) {
        elements[i] = array[i];
    }
    
    // Bottom-up heapify: bat dau tu nut co con cuoi cung
    int viTriBatDau = (size / 2) - 1;
    for (int i = viTriBatDau; i >= 0; i--) {
        reheapDown(i);
    }
}


template<class T>
void Heap<T>::clear(){
    removeInternalData();
    
    capacity = 10;
    count = 0;
    elements = new T[capacity];
}

template<class T>
bool Heap<T>::empty(){
    return count == 0;
}

template<class T>
string Heap<T>::toString(string (*item2str)(T&)){
    stringstream os;
    if(item2str != 0){
        os << "[";
        for(int idx=0; idx < count -1; idx++)
            os << item2str(elements[idx]) << ",";
        if(count > 0) os << item2str(elements[count - 1]);
        os << "]";
    }
    else{
        os << "[";
        for(int idx=0; idx < count -1; idx++)
            os << elements[idx] << ",";
        if(count > 0) os << elements[count - 1];
        os << "]";
    }
    return os.str();
}


//////////////////////////////////////////////////////////////////////
//////////////////////// (private) METHOD DEFNITION //////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
void Heap<T>::ensureCapacity(int minCapacity){
    if (minCapacity <= capacity) return;
    int nextCapacity=capacity>0 ? capacity : 10;
    while (nextCapacity < minCapacity) {
        if (nextCapacity > std::numeric_limits<int>::max()/2) {
            nextCapacity=minCapacity;
            break;
        }
        nextCapacity*=2;
    }
    T* newData=new T[nextCapacity];
    for (int i=0;i<count;++i) newData[i]=elements[i];
    delete[] elements;
    elements=newData;
    capacity=nextCapacity;
}

template<class T>
void Heap<T>::swap(int a, int b){
    T temp = this->elements[a];
    this->elements[a] = this->elements[b];
    this->elements[b] = temp;
}

template<class T>
void Heap<T>::reheapUp(int position){
    // Dua phan tu tai position len dung cho (so sanh voi cha)
    while (position > 0) {
        int viTriCha = (position - 1) / 2;
        if (aLTb(elements[position], elements[viTriCha])) {
            swap(position, viTriCha);
            position = viTriCha;
        } else {
            break;
        }
    }
}


template<class T>
void Heap<T>::reheapDown(int position){
    // Dua phan tu tai position xuong dung cho (so sanh voi 2 con)
    while (true) {
        int conTrai = 2 * position + 1;
        int conPhai = 2 * position + 2;
        int viTriNhoNhat = position;
        
        if (conTrai < count && aLTb(elements[conTrai], elements[viTriNhoNhat])) {
            viTriNhoNhat = conTrai;
        }
        if (conPhai < count && aLTb(elements[conPhai], elements[viTriNhoNhat])) {
            viTriNhoNhat = conPhai;
        }
        
        if (viTriNhoNhat != position) {
            swap(position, viTriNhoNhat);
            position = viTriNhoNhat;
        } else {
            break;
        }
    }
}


template<class T>
int Heap<T>::getItem(T item){
    int foundIdx = -1;
    for(int idx=0; idx < this->count; idx++){
        if(compare(elements[idx], item) == 0){
            foundIdx = idx;
            break;
        }
    }
    return foundIdx;
}

template<class T>
void Heap<T>::removeInternalData(){
    if(this->deleteUserData != 0) deleteUserData(this); //clear users's data if they want
    delete []elements;
}

template<class T>
void Heap<T>::copyFrom(const Heap<T>& heap){
    capacity = heap.capacity;
    count = heap.count;
    elements = new T[capacity];
    this->comparator = heap.comparator;
    this->deleteUserData = heap.deleteUserData;
    
    //Copy items from heap:
    for(int idx=0; idx < heap.count; idx++){
        this->elements[idx] = heap.elements[idx];
    }
}

#endif /* HEAP_H */

