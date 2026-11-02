#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "ISort.h"
#include <stdexcept>
using namespace std;

template<class T>
class QuickSort : public ISort<T> {
private:
    int (*pivotSelection)(T*, int);

public:
    QuickSort(int (*pivotSelection)(T*, int) = 0)
        : pivotSelection(pivotSelection) {}

    void sort(T array[], int size, int (*comparator)(T&, T&) = 0) override {
        if (size <= 1) {
            return;
        }
        quickSort(array, 0, size - 1, comparator);
    }

private:
    void doiCho(T& a, T& b) {
        T tam = a;
        a = b;
        b = tam;
    }

    void quickSort(T array[], int trai, int phai, int (*comparator)(T&, T&) = 0) {
        if (trai < phai) {
            int viTriChot = partition(array, trai, phai, comparator);
            
            // Goi de quy cho 2 nua mang
            quickSort(array, trai, viTriChot - 1, comparator);
            quickSort(array, viTriChot + 1, phai, comparator);
        }
    }

    int partition(T array[], int trai, int phai, int (*comparator)(T&, T&) = 0) {
        int chieuDai = phai - trai + 1;
        int viTriChot;
        
        // Tim vi tri cua pivot
        if (pivotSelection != 0) {
            viTriChot = trai + pivotSelection(array + trai, chieuDai);
        } else {
            viTriChot = trai + (chieuDai / 2);
        }

        T giaTriChot = array[viTriChot];
        
        // Doi cho pivot ve cuoi de de xu ly
        doiCho(array[viTriChot], array[phai]);

        int vitriTam = trai;
        for (int j = trai; j < phai; j++) {
            bool nhoHon = false;
            
            if (comparator != 0) {
                if (comparator(array[j], giaTriChot) < 0) {
                    nhoHon = true;
                }
            } else {
                if (array[j] < giaTriChot) {
                    nhoHon = true;
                }
            }
            
            if (nhoHon == true) {
                doiCho(array[vitriTam], array[j]);
                vitriTam = vitriTam + 1;
            }
        }
        
        // Dua pivot ve lai vi tri giua
        doiCho(array[vitriTam], array[phai]);
        
        return vitriTam;
    }
};

#endif
