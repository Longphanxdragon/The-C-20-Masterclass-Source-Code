#include "BKMove.h"

void BKMove::addRoute(BusRoute* route) {
    if (route == nullptr) throw invalid_argument("route must not be null");
    routes.push_back(route);
}

int BKMove::getRouteCount() const {
    return static_cast<int>(routes.size());
}

BusRoute* BKMove::getRoute(int index) {
    if (index < 0 || index >= static_cast<int>(routes.size())) {
        throw out_of_range("BKMove route index is out of range");
    }
    return routes[index];
}

#include "QuickSort.h"

static int routeResultComparator(RouteResult& a, RouteResult& b) {
    if (a.hopCount != b.hopCount) {
        return a.hopCount < b.hopCount ? -1 : 1;
    }
    if (a.routeId != b.routeId) {
        return a.routeId < b.routeId ? -1 : 1;
    }
    if (a.direction != b.direction) {
        return a.direction == OUTBOUND ? -1 : 1;
    }
    return 0;
}

static int journeyResultComparator(JourneyResult& a, JourneyResult& b) {
    if (a.transfers != b.transfers) {
        return a.transfers < b.transfers ? -1 : 1;
    }
    if (a.totalHops != b.totalHops) {
        return a.totalHops < b.totalHops ? -1 : 1;
    }
    if (a.firstRouteId != b.firstRouteId) {
        return a.firstRouteId < b.firstRouteId ? -1 : 1;
    }
    if (a.firstDirection != b.firstDirection) {
        return a.firstDirection == OUTBOUND ? -1 : 1;
    }
    if (a.transferStopId != b.transferStopId) {
        return a.transferStopId < b.transferStopId ? -1 : 1;
    }
    if (a.secondRouteId != b.secondRouteId) {
        return a.secondRouteId < b.secondRouteId ? -1 : 1;
    }
    if (a.secondDirection != b.secondDirection) {
        return a.secondDirection == OUTBOUND ? -1 : 1;
    }
    return 0;
}

vector<RouteResult> BKMove::findDirectRoutes(string fromStopId, string toStopId) {
    vector<RouteResult> mangKetQua;
    
    int soTuyen = getRouteCount();
    for (int i = 0; i < soTuyen; i++) {
        BusRoute* tuyenXe = getRoute(i);
        if (tuyenXe->isBuilt() == false) {
            continue;
        }
        
        int soChangOut = tuyenXe->getHopCount(fromStopId, toStopId, OUTBOUND);
        if (soChangOut > 0) {
            RouteResult rr(tuyenXe->getId(), OUTBOUND, soChangOut);
            mangKetQua.push_back(rr);
        }
        
        int soChangIn = tuyenXe->getHopCount(fromStopId, toStopId, INBOUND);
        if (soChangIn > 0) {
            RouteResult rr(tuyenXe->getId(), INBOUND, soChangIn);
            mangKetQua.push_back(rr);
        }
    }
    
    if (mangKetQua.size() > 0) {
        QuickSort<RouteResult> maySapXep;
        maySapXep.sort(mangKetQua.data(), mangKetQua.size(), routeResultComparator);
    }
    
    return mangKetQua;
}

vector<JourneyResult> BKMove::findJourneys(string fromStopId, string toStopId) {
    vector<JourneyResult> mangKetQua;
    
    int soTuyen = getRouteCount();
    
    // 1. Tim cac tuyen di thang (Direct)
    for (int i = 0; i < soTuyen; i++) {
        BusRoute* tuyenXe = getRoute(i);
        if (tuyenXe->isBuilt() == false) {
            continue;
        }
        
        int soChangOut = tuyenXe->getHopCount(fromStopId, toStopId, OUTBOUND);
        if (soChangOut > 0) {
            JourneyResult jr(tuyenXe->getId(), OUTBOUND, soChangOut);
            mangKetQua.push_back(jr);
        }
        
        int soChangIn = tuyenXe->getHopCount(fromStopId, toStopId, INBOUND);
        if (soChangIn > 0) {
            JourneyResult jr(tuyenXe->getId(), INBOUND, soChangIn);
            mangKetQua.push_back(jr);
        }
    }
    
    // 2. Tim cac tuyen 1 lan chuyen xe
    Direction haiChieu[2] = {OUTBOUND, INBOUND};
    
    for (int i = 0; i < soTuyen; i++) {
        BusRoute* tuyen1 = getRoute(i);
        if (tuyen1->isBuilt() == false) continue;
        
        for (int chieu1 = 0; chieu1 < 2; chieu1++) {
            Direction dir1 = haiChieu[chieu1];
            int soTram1 = tuyen1->getStopCount(dir1);
            
            // Tim vi tri cua tram bat dau
            int viTriBatDau = -1;
            for (int k = 0; k < soTram1; k++) {
                if (tuyen1->getStop(k, dir1).getId() == fromStopId) {
                    viTriBatDau = k;
                    break;
                }
            }
            
            if (viTriBatDau == -1) {
                continue;
            }
            
            // Duyet cac tram tiep theo lam tram trung chuyen
            for (int k = viTriBatDau + 1; k < soTram1; k++) {
                string tramChuyen = tuyen1->getStop(k, dir1).getId();
                int soChang1 = k - viTriBatDau;
                
                // Tim tuyen thu 2
                for (int j = 0; j < soTuyen; j++) {
                    if (i == j) continue; // Khong cung 1 tuyen
                    
                    BusRoute* tuyen2 = getRoute(j);
                    if (tuyen2->isBuilt() == false) continue;
                    
                    for (int chieu2 = 0; chieu2 < 2; chieu2++) {
                        Direction dir2 = haiChieu[chieu2];
                        int soChang2 = tuyen2->getHopCount(tramChuyen, toStopId, dir2);
                        
                        if (soChang2 > 0) {
                            JourneyResult jr(tuyen1->getId(), dir1, tramChuyen, tuyen2->getId(), dir2, soChang1 + soChang2);
                            mangKetQua.push_back(jr);
                        }
                    }
                }
            }
        }
    }
    
    // Xep hang ket qua
    if (mangKetQua.size() > 0) {
        QuickSort<JourneyResult> maySapXep;
        maySapXep.sort(mangKetQua.data(), mangKetQua.size(), journeyResultComparator);
    }
    
    return mangKetQua;
}
