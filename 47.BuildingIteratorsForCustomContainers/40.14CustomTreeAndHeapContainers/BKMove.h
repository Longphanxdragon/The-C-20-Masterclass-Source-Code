#ifndef BKMOVE_A2_H
#define BKMOVE_A2_H

#include <string>
#include <vector>
#include "AVL.h"
#include "Heap.h"
#include "support/DLinkedList.h"

struct StopInfo {
    std::string stopId;
    std::string displayName;
    std::string searchKey;
};

inline std::ostream& operator<<(std::ostream& os, const StopInfo& s) {
    os << s.stopId;
    return os;
}

struct ArrivalEvent {
    std::string eventId;
    std::string stopId;
    std::string routeId;
    int arrivalTime;
};

inline std::ostream& operator<<(std::ostream& os, const ArrivalEvent& e) {
    os << e.eventId;
    return os;
}

template<class T>
inline std::ostream& operator<<(std::ostream& os, const Heap<T>&) {
    os << "Heap";
    return os;
}

class BKMove {
public:
    BKMove();
    ~BKMove();

    BKMove(const BKMove&) = delete;
    BKMove& operator=(const BKMove&) = delete;

    // Q3: Bus Stop Finder
    bool addStop(const StopInfo& stop);
    bool removeStop(const std::string& stopId);
    bool findStop(const std::string& stopId, StopInfo& out) const;
    int getStopCount() const;
    std::vector<StopInfo> findStops(
        const std::string& prefix, int page, int pageSize) const;

    // Q4: Live Arrival Board
    bool scheduleArrival(const ArrivalEvent& event);
    bool peekNextArrival(const std::string& stopId,
                         ArrivalEvent& out) const;
    bool processNextArrival(const std::string& stopId,
                            ArrivalEvent& out);
    bool rescheduleArrival(const std::string& eventId,
                           int newArrivalTime);
    bool cancelArrival(const std::string& eventId);

private:
    // Q3: dung AVL tree de luu tru cac tram bus theo stopId
    // Va mot cau truc de tim kiem theo prefix (searchKey)
    
    struct StopEntry {
        StopInfo thongTin;
    };
    
    // AVL tree: key = stopId (string), value = StopInfo*
    AVL<std::string, StopInfo*>* cayStopId;
    
    // Cau truc phu tro de luu theo thu tu searchKey + stopId
    // Dung DLinkedList de luu danh sach sap xep
    // (key = searchKey+"_"+stopId, value = StopInfo*)
    AVL<std::string, StopInfo*>* caySearchKey;
    
    // Q4: Moi tram co 1 heap rieng de luu cac su kien
    // eventId -> (stopId, heap entry) de ho tro reschedule/cancel
    struct SuKienArrival {
        ArrivalEvent duLieu;
        bool operator<(const SuKienArrival& other) const {
            if (duLieu.arrivalTime != other.duLieu.arrivalTime) {
                return duLieu.arrivalTime < other.duLieu.arrivalTime;
            }
            return duLieu.eventId < other.duLieu.eventId;
        }
        bool operator>(const SuKienArrival& other) const {
            return other < *this;
        }
        bool operator==(const SuKienArrival& other) const {
            return duLieu.eventId == other.duLieu.eventId;
        }
        friend std::ostream& operator<<(std::ostream& os, const SuKienArrival& sk) {
            os << sk.duLieu.eventId;
            return os;
        }
    };
    
    static int soSanhSuKien(SuKienArrival& a, SuKienArrival& b) {
        if (a.duLieu.arrivalTime != b.duLieu.arrivalTime) {
            return a.duLieu.arrivalTime < b.duLieu.arrivalTime ? -1 : 1;
        }
        if (a.duLieu.eventId < b.duLieu.eventId) return -1;
        if (a.duLieu.eventId > b.duLieu.eventId) return 1;
        return 0;
    }
    
    // AVL tree: key = stopId, value = Heap<SuKienArrival>*
    AVL<std::string, Heap<SuKienArrival>*>* cayHeapTheoTram;
    
    // AVL tree: key = eventId, value = stopId* (de biet eventId thuoc tram nao)
    AVL<std::string, std::string*>* cayEventIdToStopId;
    
    // Tong so tram dang duoc quan ly
    int tongSoTram;
};

#endif  // BKMOVE_A2_H
