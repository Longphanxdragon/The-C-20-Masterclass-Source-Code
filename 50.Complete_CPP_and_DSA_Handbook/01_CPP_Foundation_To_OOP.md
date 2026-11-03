# CHƯƠNG 1: NỀN TẢNG C++ & LẬP TRÌNH HƯỚNG ĐỐI TƯỢNG (OOP)

---

## 1. MÔ HÌNH BỘ NHỚ TRONG C++ (STACK VS HEAP)

Khi một chương trình C++ được thực thi, hệ điều hành cấp phát một không gian địa chỉ bộ nhớ được chia thành các phân vùng chính:

```
+------------------------------------+ Cao
|         Stack (Ngăn xếp)           | -> Tự động cấp phát/thu hồi khi vào/ra hàm
|                 |                  |
|                 v                  |
|                                    |
|                 ^                  |
|                 |                  |
|          Heap (Vùng nhớ tự do)     | -> Cấp phát động bằng new, giải phóng bằng delete
+------------------------------------+
|          BSS / Data Segment        | -> Biến toàn cục (global), biến tĩnh (static)
+------------------------------------+
|          Text / Code Segment       | -> Mã máy thực thi của chương trình
+------------------------------------+ Thấp
```

### So sánh Stack và Heap:
- **Stack (Bộ nhớ ngăn xếp):**
  - Quản lý tự động theo cơ chế LIFO (Last In, First Out).
  - Tốc độ truy cập cực nhanh vì con trỏ thanh ghi ngăn xếp (`ESP/RSP`) chỉ việc tăng/giảm.
  - Kích thước giới hạn (thường từ 1MB đến 8MB tùy OS). Nếu đệ quy quá sâu hoặc khai báo mảng quá lớn trong hàm cục bộ sẽ gây lỗi **Stack Overflow**.
- **Heap (Bộ nhớ cấp phát động):**
  - Kích thước lớn (giới hạn bởi RAM vật lý và bộ nhớ ảo).
  - Do lập trình viên chủ động quản lý thông qua toán tử `new` và `delete`.
  - Nếu `new` mà quên `delete` $\rightarrow$ **Memory Leak (Rò rỉ bộ nhớ)**.
  - Nếu `delete` xong mà vẫn tiếp tục truy cập qua con trỏ $\rightarrow$ **Dangling Pointer (Con trỏ lạc/treo)**.
  - Xóa 2 lần một vùng nhớ $\rightarrow$ **Double Free** gây crash chương trình (`SIGSEGV` hoặc `0xC0000374`).

---

## 2. CON TRỎ (POINTER) VÀ THAM CHIẾU (REFERENCE)

### 2.1. Con trỏ (Pointer)
- Con trỏ là một biến lưu trữ **địa chỉ ô nhớ** của một biến khác.
- Kích thước con trỏ phụ thuộc vào kiến trúc hệ thống (4 bytes trên hệ điều hành 32-bit, 8 bytes trên 64-bit).

```cpp
int a = 100;
int* ptr = &a; // ptr giữ địa chỉ của a
*ptr = 200;    // Toán tử giải tham chiếu (dereference), thay đổi giá trị của a thành 200
```

### 2.2. Tham chiếu (Reference)
- Tham chiếu là một **tên gọi khác (alias)** cho một biến đã tồn tại.
- Bắt buộc phải khởi tạo ngay khi khai báo và không thể trỏ sang đối tượng khác sau khi đã gắn.
- Không thể mang giá trị `nullptr`.

```cpp
int x = 50;
int& ref = x; // ref chính là x
ref = 70;     // x bây giờ bằng 70
```

### 2.3. Ba kiểu truyền tham số vào hàm

| Cách truyền | Cú pháp | Bản chất | Khi nào sử dụng? |
| :--- | :--- | :--- | :--- |
| **Pass-by-value** | `void foo(int x)` | Tạo bản sao của biến. Thay đổi trong hàm không ảnh hưởng biến gốc. | Các kiểu dữ liệu nguyên thủy nhỏ (`int`, `float`, `char`). |
| **Pass-by-pointer** | `void foo(int* x)` | Truyền địa chỉ. Có thể kiểm tra `nullptr`. Cho phép hàm thay đổi giá trị gốc. | Khi tham số có thể là tùy chọn (`nullptr`), hoặc cần quản lý bộ nhớ động. |
| **Pass-by-reference** | `void foo(int& x)` | Truyền trực tiếp đối tượng mà không tạo bản sao. Cú pháp tự nhiên như truyền giá trị. | Khi cần sửa đổi biến gốc hoặc khi truyền object lớn (`const ClassName&` để tối ưu hiệu năng). |

---

## 3. CON TRỎ HÀM (FUNCTION POINTER) & CALLBACK

Con trỏ hàm lưu trữ địa chỉ của đoạn mã máy thực thi hàm đó, cho phép truyền hàm như một tham số vào hàm khác. Đây là nền tảng của cơ chế **Callback** và **Custom Comparator** trong các cấu trúc dữ liệu tổng quát (như `QuickSort`, `Heap`, `SLinkedList`).

### Cú pháp khai báo:
```cpp
// Khai báo con trỏ hàm nhận 2 tham số T& và trả về int:
int (*comparator)(T& a, T& b);
```

### Ứng dụng thực tế trong hàm so sánh:
```cpp
// Hàm so sánh thứ tự tăng dần
int soSanhTangDan(int& a, int& b) {
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

// Hàm sắp xếp nhận con trỏ hàm
void sapXep(int arr[], int n, int (*cmp)(int&, int&)) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (cmp(arr[i], arr[j]) > 0) {
                std::swap(arr[i], arr[j]);
            }
        }
    }
}
```

---

## 4. 4 TÍNH CHẤT TRỌNG TÂM CỦA LẬP TRÌNH HƯỚNG ĐỐI TƯỢNG (OOP)

### 4.1. Tính đóng gói (Encapsulation)
- Gom dữ liệu (attributes) và các hàm xử lý dữ liệu đó (methods) vào trong một thực thể gọi là `class`.
- Che giấu chi tiết cài đặt nội bộ thông qua các phạm vi truy cập:
  - `private`: Chỉ nội bộ các hàm thành viên trong class hoặc hàm bạn (`friend`) mới được truy cập.
  - `protected`: Cho phép class con trong quan hệ kế thừa truy cập.
  - `public`: Cho phép bất kỳ đâu bên ngoài truy cập.

### 4.2. Khởi tạo, Hủy bỏ & Quy tắc Rule of Three
Khi một class tự quản lý tài nguyên động (như con trỏ `new` mảng hoặc liên kết các node), cần tuân thủ **Rule of Three**:
1. **Destructor (`~ClassName()`):** Giải phóng toàn bộ bộ nhớ Heap đã cấp phát khi đối tượng ra khỏi phạm vi.
2. **Copy Constructor (`ClassName(const ClassName& other)`):** Thực hiện **Deep Copy** (sao chép sâu) thay vì **Shallow Copy** (chỉ sao chép địa chỉ con trỏ). Nếu sao chép nông, hai đối tượng sẽ trỏ chung vào một vùng nhớ, dẫn đến lỗi Double Free khi hủy.
3. **Copy Assignment Operator (`ClassName& operator=(const ClassName& other)`):** Tự giải phóng bộ nhớ cũ của chính mình trước khi Deep Copy dữ liệu từ đối tượng mới, đồng thời kiểm tra tự gán (`if (this == &other) return *this;`).

```cpp
class DynamicArray {
private:
    int* data;
    int size;

public:
    // 1. Constructor
    DynamicArray(int s = 10) : size(s), data(new int[s]) {}

    // 2. Destructor
    ~DynamicArray() {
        delete[] data;
    }

    // 3. Copy Constructor (Deep Copy)
    DynamicArray(const DynamicArray& other) : size(other.size), data(new int[other.size]) {
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    // 4. Copy Assignment Operator
    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) return *this; // Tránh tự gán
        delete[] data;                    // Giải phóng bộ nhớ hiện tại
        size = other.size;
        data = new int[size];             // Cấp phát mới
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        return *this;
    }
};
```

### 4.3. Tính kế thừa (Inheritance)
- Cho phép class con tái sử dụng thuộc tính và phương thức từ class cha.
- Trong C++, kế thừa mặc định trong `class` là `private`, trong `struct` là `public`. Thông thường ta sử dụng `public inheritance` để biểu diễn quan hệ "là một" (is-a):
```cpp
template<class T>
class SLinkedList : public IList<T> { ... };
```

### 4.4. Tính đa hình (Polymorphism) & Cơ chế VTable
- **Đa hình lúc biên dịch (Compile-time / Static):** Nạp chồng hàm (Function Overloading), nạp chồng toán tử (Operator Overloading), Templates.
- **Đa hình lúc thực thi (Runtime / Dynamic):** Thực hiện qua hàm ảo (`virtual`). Cho phép một con trỏ kiểu class cha (`Base*`) trỏ tới đối tượng class con (`Derived`) và gọi đúng phương thức của class con tại thời điểm chạy.

#### Cơ chế hoạt động của `virtual`:
1. Mọi class có ít nhất một hàm `virtual` sẽ được trình biên dịch tạo một bảng con trỏ hàm gọi là **VTable (Virtual Method Table)**.
2. Mỗi đối tượng thuộc class đó sẽ tự động được gán thêm một con trỏ ẩn gọi là **vptr** trỏ tới bảng VTable tương ứng.
3. Khi gọi hàm ảo qua con trỏ hoặc tham chiếu: chương trình tra cứu qua `vptr` $\rightarrow$ `VTable` $\rightarrow$ gọi đúng địa chỉ hàm của class con.

> **QUAN TRỌNG:** Luôn phải khai báo **Virtual Destructor** trong class cha nếu class đó được dùng làm interface/kế thừa:
> ```cpp
> virtual ~IBST() {}
> ```
> Nếu destructor không phải là `virtual`, khi xóa con trỏ cha (`Base* p = new Derived(); delete p;`), chỉ có destructor của class cha được gọi, gây rò rỉ bộ nhớ của class con!
