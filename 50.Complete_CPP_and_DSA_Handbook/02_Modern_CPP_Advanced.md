# CHƯƠNG 2: C++ HIỆN ĐẠI, SMART POINTERS, TEMPLATES & STL

---

## 1. NGUYÊN LÝ RAII & CON TRỎ THÔNG MINH (SMART POINTERS)

### 1.1. Nguyên lý RAII (Resource Acquisition Is Initialization)
RAII là triết lý cốt lõi của C++: **Gắn vòng đời của một tài nguyên (bộ nhớ Heap, file handle, socket, mutex lock) với vòng đời của một đối tượng trên Stack**.
- Khi đối tượng được khởi tạo trên Stack (Constructor) $\rightarrow$ Cấp phát tài nguyên.
- Khi đối tượng ra khỏi phạm vi (scope) và bị hủy (Destructor) $\rightarrow$ Tự động giải phóng tài nguyên, kể cả khi xảy ra Exception.

### 1.2. Ba loại Smart Pointer (từ C++11)

| Loại Smart Pointer | Cú pháp khởi tạo | Đặc điểm sở hữu | Ứng dụng |
| :--- | :--- | :--- | :--- |
| `std::unique_ptr<T>` | `auto p = std::make_unique<T>();` | **Độc quyền (Exclusive)**: Không thể sao chép (copy), chỉ có thể chuyển giao sở hữu (`std::move`). Tự động `delete` khi ra khỏi scope. Không tốn thêm chi phí bộ nhớ so với con trỏ thô (Zero-cost abstraction). | Quản lý node nội bộ, đối tượng sở hữu riêng biệt. |
| `std::shared_ptr<T>` | `auto p = std::make_shared<T>();` | **Đồng sở hữu (Shared)**: Quản lý một bộ đếm tham chiếu (Reference Count). Khi có bản sao mới, counter tăng lên 1; khi một pointer bị hủy, counter giảm 1. Khi counter về 0, vùng nhớ mới thực sự bị `delete`. | Dữ liệu chia sẻ giữa nhiều luồng hoặc nhiều cấu trúc dữ liệu. |
| `std::weak_ptr<T>` | Gán từ `shared_ptr` | **Không tăng counter**: Tham chiếu tới đối tượng được quản lý bởi `shared_ptr` mà không giữ quyền sở hữu. Phải dùng `.lock()` để kiểm tra trước khi truy cập. | **Phá vỡ vòng tròn phụ thuộc (Circular Reference)** trong đồ thị hoặc cây 2 chiều. |

```cpp
#include <iostream>
#include <memory>

struct Node {
    int data;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev; // Dùng weak_ptr để tránh chu kỳ rò rỉ bộ nhớ
    Node(int val) : data(val) {}
};
```

---

## 2. MOVE SEMANTICS & RVALUE REFERENCES (C++11)

### 2.1. Lvalue vs Rvalue
- **Lvalue (Left value):** Đối tượng có danh tính, có tên biến, có địa chỉ bộ nhớ cố định (ví dụ: `int x = 10;`, `x` là lvalue).
- **Rvalue (Right value):** Giá trị tạm thời không có tên, biến mất ngay sau biểu thức (ví dụ: `10`, `x + y`, giá trị trả về của một hàm tạm).

### 2.2. Toán tử chuyển quyền sở hữu: `std::move`
Thay vì phải cấp phát vùng nhớ mới và copy toàn bộ dữ liệu của một rvalue (Deep Copy tốn $O(N)$), **Move Semantics** cho phép đối tượng mới "đánh cắp" con trỏ dữ liệu của đối tượng cũ trong thời gian $O(1)$.

```cpp
class BigBuffer {
private:
    int* buffer;
    size_t size;
public:
    // Move Constructor
    BigBuffer(BigBuffer&& other) noexcept : buffer(other.buffer), size(other.size) {
        // Tước quyền sở hữu của đối tượng cũ
        other.buffer = nullptr;
        other.size = 0;
    }

    // Move Assignment Operator
    BigBuffer& operator=(BigBuffer&& other) noexcept {
        if (this == &other) return *this;
        delete[] buffer; // Dọn dẹp của mình trước
        
        buffer = other.buffer;
        size = other.size;
        
        other.buffer = nullptr;
        other.size = 0;
        return *this;
    }
};
```

---

## 3. LẬP TRÌNH TỔNG QUÁT (GENERIC PROGRAMMING & TEMPLATES)

Templates cho phép viết code hoạt động với bất kỳ kiểu dữ liệu nào mà không cần viết lại mã nguồn nhiều lần.

### 3.1. Function Template
```cpp
template<typename T>
void hoanVi(T& a, T& b) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}
```

### 3.2. Class Template
```cpp
template<class K, class V>
class Entry {
public:
    K key;
    V value;
    Entry(K k, V v) : key(k), value(v) {}
};
```

### 3.3. Template Specialization (Chuyên biệt hóa Template)
Cho phép định nghĩa một cài đặt riêng cho một kiểu dữ liệu cụ thể (ví dụ: xử lý chuỗi ký tự `const char*` khác với số học thông thường):
```cpp
template<typename T>
bool isEqual(T a, T b) { return a == b; }

// Chuyên biệt hóa cho kiểu const char*
template<>
bool isEqual<const char*>(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}
```

---

## 4. CÁC TÍNH NĂNG MỚI TRỌNG TÂM CỦA C++20

1. **Concepts & Constraints:**
   - Thay thế các kỹ thuật SFINAE phức tạp của C++11/14. Cho phép ràng buộc kiểu dữ liệu đầu vào của Template ngay tại thời điểm biên dịch với thông báo lỗi rõ ràng.
   ```cpp
   #include <concepts>

   // Chỉ cho phép các kiểu dữ liệu có thể so sánh được với toán tử <
   template<typename T>
   concept Comparable = requires(T a, T b) {
       { a < b } -> std::convertible_to<bool>;
   };

   template<Comparable T>
   T timMin(T a, T b) {
       return (a < b) ? a : b;
   }
   ```

2. **Toán tử phi thuyền (Three-way Comparison `<=>`):**
   - Tự động sinh ra cả 6 toán tử so sánh (`<`, `<=`, `>`, `>=`, `==`, `!=`) chỉ với 1 dòng khai báo:
   ```cpp
   #include <compare>

   struct Point {
       int x, y;
       auto operator<=>(const Point&) const = default;
   };
   ```

3. **Ranges & Views Library:**
   - Xử lý chuỗi dữ liệu theo phong cách Functional Pipeline cực kỳ trực quan và lười đánh giá (lazy evaluation):
   ```cpp
   #include <ranges>
   #include <vector>

   std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
   auto result = numbers 
               | std::views::filter([](int n){ return n % 2 == 0; }) 
               | std::views::transform([](int n){ return n * n; });
   ```

---

## 5. CẨM NANG THƯ VIỆN CHUẨN C++ (STL CHEAT SHEET)

```
                       CÁC CONTAINER TRONG STL
                                  |
    +-----------------------------+-----------------------------+
    |                             |                             |
Sequence Containers      Associative Containers       Unordered Containers
(Bộ nhớ liên tục/DSLK)      (Cây Đỏ-Đen: O(log N))     (Bảng băm Hash: O(1))
- std::vector            - std::set                   - std::unordered_set
- std::deque             - std::map                   - std::unordered_map
- std::list (DSLK kép)   - std::multiset              - std::unordered_multimap
- std::forward_list      - std::multimap
```

### Container Adapters (Bộ điều hợp):
- `std::stack<T>`: Mặc định bọc ngoài `std::deque` (LIFO).
- `std::queue<T>`: Mặc định bọc ngoài `std::deque` (FIFO).
- `std::priority_queue<T>`: Mặc định cài bằng Max-Heap trên `std::vector`.

### Lambda Expressions (Hàm vô danh):
```cpp
// Cú pháp: [capture](parameters) -> return_type { body }
auto cmp = [](const auto& a, const auto& b) {
    return a.first < b.first;
};
```
