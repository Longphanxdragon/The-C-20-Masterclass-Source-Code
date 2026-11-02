#include "BusRoute.h"

BusRoute::BusRoute(string routeId)
    : routeId(routeId), outboundCount(0), built(false) {}

string BusRoute::getId() const {
    return routeId;
}

bool BusRoute::isBuilt() const {
    return built;
}

int BusRoute::getStopCount(Direction direction) {
    if (built == false) {
        return 0;
    }
    if (direction == OUTBOUND) {
        return outboundCount;
    } else {
        return stops.size() - outboundCount + 2;
    }
}

int BusRoute::physicalIndex(int index, Direction direction) {
    if (direction == OUTBOUND) {
        return index;
    } else {
        return (outboundCount - 1 + index) % stops.size();
    }
}

BusStop& BusRoute::getStop(int index, Direction direction) {
    if (built == false) {
        throw out_of_range("Route nay chua build");
    }
    int soTram = getStopCount(direction);
    if (index < 0 || index >= soTram) {
        throw out_of_range("Index sai roi");
    }
    return stops.get(physicalIndex(index, direction));
}

void BusRoute::build(SLinkedList<BusStop>& outbound, SLinkedList<BusStop>& inbound) {
    stops.clear();
    outboundCount = outbound.size();
    
    for (int i = 0; i < outboundCount; i++) {
        stops.add(outbound.get(i));
    }
    
    int soTramIn = inbound.size();
    for (int i = 1; i < soTramIn - 1; i++) {
        stops.add(inbound.get(i));
    }
    built = true;
}

int BusRoute::getHopCount(string fromStopId, string toStopId, Direction direction) {
    if (built == false) {
        return -1;
    }
    
    int tongSoTram = getStopCount(direction);
    int viTriDau = -1;
    
    // Tim tram bat dau
    for (int i = 0; i < tongSoTram; i++) {
        BusStop tramHienTai = getStop(i, direction);
        if (tramHienTai.getId() == fromStopId) {
            viTriDau = i;
            break;
        }
    }
    
    if (viTriDau == -1) {
        return -1;
    }
    
    // Tim tram ket thuc tu vi tri bat dau
    for (int i = viTriDau; i < tongSoTram; i++) {
        BusStop tramHienTai = getStop(i, direction);
        if (tramHienTai.getId() == toStopId) {
            return i - viTriDau;
        }
    }
    
    return -1;
}
