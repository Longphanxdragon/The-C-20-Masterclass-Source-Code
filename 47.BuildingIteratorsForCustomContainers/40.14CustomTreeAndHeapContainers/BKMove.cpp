#include "BKMove.h"
#include <algorithm>

BKMove::BKMove() {
    cayStopId = new AVL<std::string, StopInfo*>();
    caySearchKey = new AVL<std::string, StopInfo*>();
    cayHeapTheoTram = new AVL<std::string, Heap<SuKienArrival>*>();
    cayEventIdToStopId = new AVL<std::string, std::string*>();
    tongSoTram = 0;
}

BKMove::~BKMove() {
    // BST::Node::~Node() da tu dong delete data.value roi
    // Nen chi can delete cay la tat ca nut va data.value deu duoc giai phong sach se
    delete cayStopId;
    delete caySearchKey;
    delete cayHeapTheoTram;
    delete cayEventIdToStopId;
}

// Q3 -- Bus Stop Finder

bool BKMove::addStop(const StopInfo& stop) {
    if (stop.stopId.empty() || stop.displayName.empty() || stop.searchKey.empty()) {
        return false;
    }
    
    bool daCoRoi = false;
    cayStopId->search(stop.stopId, daCoRoi);
    if (daCoRoi == true) {
        return false;
    }
    
    // Tao 2 ban copy rieng biet de moi cay quan ly 1 con tro
    StopInfo* infoChoStopId = new StopInfo(stop);
    StopInfo* infoChoSearch = new StopInfo(stop);
    
    cayStopId->add(stop.stopId, infoChoStopId);
    
    std::string keySearch = stop.searchKey + "\t" + stop.stopId;
    caySearchKey->add(keySearch, infoChoSearch);
    
    Heap<SuKienArrival>* heapMoi = new Heap<SuKienArrival>(soSanhSuKien);
    cayHeapTheoTram->add(stop.stopId, heapMoi);
    
    tongSoTram = tongSoTram + 1;
    return true;
}

bool BKMove::removeStop(const std::string& stopId) {
    bool coTon = false;
    StopInfo* pInfo = cayStopId->search(stopId, coTon);
    if (coTon == false || pInfo == nullptr) {
        return false;
    }
    
    // Kiem tra con su kien arrival nao dang cho tai tram nay khong
    bool coHeap = false;
    Heap<SuKienArrival>* pHeap = cayHeapTheoTram->search(stopId, coHeap);
    if (coHeap == true && pHeap != nullptr && pHeap->size() > 0) {
        return false;
    }
    
    std::string keySearch = pInfo->searchKey + "\t" + stopId;
    
    // Xoa khoi caySearchKey
    bool okSearch = false;
    StopInfo* pXoaSearch = caySearchKey->remove(keySearch, &okSearch);
    if (pXoaSearch != nullptr) delete pXoaSearch;
    
    // Xoa khoi cayHeapTheoTram
    if (coHeap == true && pHeap != nullptr) {
        bool okH = false;
        Heap<SuKienArrival>* pH = cayHeapTheoTram->remove(stopId, &okH);
        if (pH != nullptr) delete pH;
    }
    
    // Xoa khoi cayStopId
    bool okStop = false;
    StopInfo* pXoaStop = cayStopId->remove(stopId, &okStop);
    if (pXoaStop != nullptr) delete pXoaStop;
    
    tongSoTram = tongSoTram - 1;
    return true;
}

bool BKMove::findStop(const std::string& stopId, StopInfo& out) const {
    bool timThay = false;
    StopInfo* pInfo = cayStopId->search(stopId, timThay);
    if (timThay == false || pInfo == nullptr) {
        return false;
    }
    out = *pInfo;
    return true;
}

int BKMove::getStopCount() const {
    return tongSoTram;
}

std::vector<StopInfo> BKMove::findStops(
    const std::string& prefix, int page, int pageSize) const {
    if (pageSize <= 0 || page < 0) {
        return {};
    }
    
    // Lay tat ca theo thu tu tang dan cua caySearchKey
    // Cay nay sap xep theo (searchKey, stopId)
    DLinkedList<StopInfo*> danhSach = caySearchKey->ascendingList();
    
    // Loc theo prefix
    std::vector<StopInfo> danhSachKhop;
    for (auto it = danhSach.begin(); it != danhSach.end(); ++it) {
        StopInfo* p = *it;
        if (p == nullptr) continue;
        
        if (prefix.empty()) {
            danhSachKhop.push_back(*p);
        } else {
            // Kiem tra searchKey co bat dau bang prefix khong
            if (p->searchKey.rfind(prefix, 0) == 0) {
                danhSachKhop.push_back(*p);
            }
        }
    }
    
    int tongSoKhop = static_cast<int>(danhSachKhop.size());
    int batDau = page * pageSize;
    if (batDau >= tongSoKhop) {
        return {};
    }
    
    std::vector<StopInfo> ketQuaTrang;
    int ketThuc = std::min(batDau + pageSize, tongSoKhop);
    for (int i = batDau; i < ketThuc; i++) {
        ketQuaTrang.push_back(danhSachKhop[i]);
    }
    
    return ketQuaTrang;
}

// Q4 -- Live Arrival Board

bool BKMove::scheduleArrival(const ArrivalEvent& event) {
    if (event.eventId.empty() || event.stopId.empty() || event.routeId.empty() || event.arrivalTime < 0) {
        return false;
    }
    
    bool coTram = false;
    cayStopId->search(event.stopId, coTram);
    if (coTram == false) {
        return false;
    }
    
    bool eventDaTon = false;
    cayEventIdToStopId->search(event.eventId, eventDaTon);
    if (eventDaTon == true) {
        return false;
    }
    
    bool coHeap = false;
    Heap<SuKienArrival>* pHeap = cayHeapTheoTram->search(event.stopId, coHeap);
    if (coHeap == false || pHeap == nullptr) {
        return false;
    }
    
    SuKienArrival sk;
    sk.duLieu = event;
    pHeap->push(sk);
    
    std::string* pStopId = new std::string(event.stopId);
    cayEventIdToStopId->add(event.eventId, pStopId);
    
    return true;
}

bool BKMove::peekNextArrival(
    const std::string& stopId, ArrivalEvent& out) const {
    bool coTram = false;
    cayStopId->search(stopId, coTram);
    if (coTram == false) {
        return false;
    }
    
    bool coHeap = false;
    Heap<SuKienArrival>* pHeap = cayHeapTheoTram->search(stopId, coHeap);
    if (coHeap == false || pHeap == nullptr || pHeap->size() == 0) {
        return false;
    }
    
    out = pHeap->peek().duLieu;
    return true;
}

bool BKMove::processNextArrival(
    const std::string& stopId, ArrivalEvent& out) {
    bool coTram = false;
    cayStopId->search(stopId, coTram);
    if (coTram == false) {
        return false;
    }
    
    bool coHeap = false;
    Heap<SuKienArrival>* pHeap = cayHeapTheoTram->search(stopId, coHeap);
    if (coHeap == false || pHeap == nullptr || pHeap->size() == 0) {
        return false;
    }
    
    SuKienArrival sk = pHeap->pop();
    out = sk.duLieu;
    
    bool ok = false;
    std::string* pStr = cayEventIdToStopId->remove(sk.duLieu.eventId, &ok);
    if (pStr != nullptr) delete pStr;
    
    return true;
}

bool BKMove::rescheduleArrival(
    const std::string& eventId, int newArrivalTime) {
    if (newArrivalTime < 0) {
        return false;
    }
    
    bool coEvent = false;
    std::string* pStopId = cayEventIdToStopId->search(eventId, coEvent);
    if (coEvent == false || pStopId == nullptr) {
        return false;
    }
    
    std::string stopId = *pStopId;
    bool coHeap = false;
    Heap<SuKienArrival>* pHeap = cayHeapTheoTram->search(stopId, coHeap);
    if (coHeap == false || pHeap == nullptr) {
        return false;
    }
    
    ArrivalEvent eventCu;
    bool timThay = false;
    for (auto it = pHeap->begin(); it != pHeap->end(); ++it) {
        if ((*it).duLieu.eventId == eventId) {
            eventCu = (*it).duLieu;
            timThay = true;
            break;
        }
    }
    if (timThay == false) return false;
    
    SuKienArrival skXoa;
    skXoa.duLieu = eventCu;
    pHeap->remove(skXoa);
    
    eventCu.arrivalTime = newArrivalTime;
    SuKienArrival skMoi;
    skMoi.duLieu = eventCu;
    pHeap->push(skMoi);
    
    return true;
}

bool BKMove::cancelArrival(const std::string& eventId) {
    bool coEvent = false;
    std::string* pStopId = cayEventIdToStopId->search(eventId, coEvent);
    if (coEvent == false || pStopId == nullptr) {
        return false;
    }
    
    std::string stopId = *pStopId;
    bool coHeap = false;
    Heap<SuKienArrival>* pHeap = cayHeapTheoTram->search(stopId, coHeap);
    if (coHeap == false || pHeap == nullptr) {
        return false;
    }
    
    ArrivalEvent eventCan;
    bool timThay = false;
    for (auto it = pHeap->begin(); it != pHeap->end(); ++it) {
        if ((*it).duLieu.eventId == eventId) {
            eventCan = (*it).duLieu;
            timThay = true;
            break;
        }
    }
    if (timThay == false) return false;
    
    SuKienArrival skXoa;
    skXoa.duLieu = eventCan;
    pHeap->remove(skXoa);
    
    bool ok = false;
    std::string* pStr = cayEventIdToStopId->remove(eventId, &ok);
    if (pStr != nullptr) delete pStr;
    
    return true;
}
