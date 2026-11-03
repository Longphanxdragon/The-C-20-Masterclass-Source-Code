# CẨM NANG ÔN TẬP TOÀN DIỆN: C++ TỪ CƠ BẢN ĐẾN NÂNG CAO & CẤU TRÚC DỮ LIỆU - GIẢI THUẬT (DSA)

Tài liệu được biên soạn dành cho sinh viên ngành Khoa học và Kỹ thuật Máy tính (Đại học Bách Khoa ĐHQG-HCM và các trường đại học đào tạo CNTT), hệ thống hóa toàn bộ kiến thức từ nền tảng ngôn ngữ C++ hiện đại (C++11/17/20) đến các Cấu trúc Dữ liệu & Giải thuật kinh điển, kèm theo kinh nghiệm bảo vệ bài tập lớn/phỏng vấn kỹ thuật.

---

## 📚 MỤC LỤC TỔNG QUAN

1. [**01. Nền tảng C++ & Lập trình hướng đối tượng (OOP)**](01_CPP_Foundation_To_OOP.md)
   - Cú pháp cơ bản, con trỏ (Pointer), tham chiếu (Reference), con trỏ hàm (Function Pointer).
   - Quản lý bộ nhớ (Stack vs Heap, `new`/`delete`, memory leak).
   - 4 tính chất OOP: Đóng gói (Encapsulation), Kế thừa (Inheritance), Đa hình (Polymorphism - `virtual`, `vtable`), Trừu tượng (Abstraction).
   - Rule of 3, Rule of 5, Rule of 0.

2. [**02. C++ Hiện đại, Smart Pointers, Templates & STL**](02_Modern_CPP_Advanced.md)
   - Con trỏ thông minh (Smart Pointers): `unique_ptr`, `shared_ptr`, `weak_ptr` và nguyên lý RAII.
   - Move Semantics & Rvalue Reference: `std::move`, `std::forward`, tránh sao chép lãng phí.
   - Generic Programming & Templates: Function Templates, Class Templates, Template Specialization.
   - C++20: Concepts & Constraints, Ranges Library, Toán tử phi thuyền (`<=>`).
   - Standard Template Library (STL): Vector, Deque, List, Set, Map, Unordered Map, Algorithms & Lambdas.

3. [**03. Cấu trúc Dữ liệu Chuyên sâu (Data Structures in Depth)**](03_Data_Structures_In_Depth.md)
   - Danh sách liên kết: Singly Linked List (với và không có Dummy node), Doubly Linked List, Circular Linked List.
   - Ngăn xếp & Hàng đợi: Stack, Queue, Deque, Monotonic Stack/Queue.
   - Cây nhị phân tìm kiếm (BST): Thao tác thêm, tìm kiếm, xóa 3 trường hợp (0, 1, 2 con với Inorder Successor).
   - Cây cân bằng AVL: Hệ số cân bằng (Balance Factor), 4 phép xoay (LL, RR, LR, RL), tái cân bằng tự động $O(\log N)$.
   - Đống nhị phân (Binary Heap / Priority Queue): Mảng phẳng, `reheapUp`, `reheapDown`, thuật toán `heapify` Bottom-Up chạy trong $O(N)$.

4. [**04. Thiết kế & Phân tích Giải thuật (Algorithms Design & Analysis)**](04_Algorithms_Sorting_Searching_Graphs.md)
   - Độ phức tạp thuật toán: Ký pháp Big-O, Big-$\Omega$, Big-$\Theta$, phân tích thời gian và không gian.
   - Thuật toán sắp xếp: QuickSort (Lomuto, Hoare, pivot tie-break), MergeSort, HeapSort. So sánh toàn diện các thuật toán.
   - Tìm kiếm: Binary Search (cận trái, cận phải), Kỹ thuật Hai con trỏ (Two Pointers), Cửa sổ trượt (Sliding Window).
   - Giải thuật đồ thị: BFS, DFS, Đường đi ngắn nhất (Dijkstra, Bellman-Ford), Cây khung nhỏ nhất (Prim, Kruskal với DSU).

5. [**05. Cẩm nang Bài tập lớn BKMove & Bí kíp Vấn đáp (Exam Defense Prep)**](05_BKMove_Assignments_And_Defense_Guide.md)
   - Phân tích chi tiết kiến trúc BKMove Assignment 1 (Singly & Circular Linked List, Route Hop Count, QuickSort, Direct & 1-Transfer routing).
   - Phân tích chi tiết kiến trúc BKMove Assignment 2 (BST, AVL Tree, Binary Heap, Stop Finder với tiền tố và phân trang, Live Arrival Board).
   - Bộ 20+ câu hỏi bẫy thường gặp của giảng viên khi bảo vệ bài tập lớn DSA và cách trả lời thuyết phục đạt điểm tối đa.

---

## ⏱️ BẢNG TỔNG HỢP ĐỘ PHỨC TẠP (BIG-O CHEATSHEET)

### 1. Cấu trúc dữ liệu

| Cấu trúc dữ liệu | Truy cập theo index | Tìm kiếm | Chèn | Xóa | Ghi chú |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **Mảng / `std::vector`** | $O(1)$ | $O(N)$ | Cuối: $O(1)^*$ / Giữa: $O(N)$ | Cuối: $O(1)$ / Giữa: $O(N)$ | Bộ nhớ liên tục, cache-friendly |
| **Singly Linked List** | $O(N)$ | $O(N)$ | Đầu: $O(1)$ / Cuối (có tail): $O(1)$ | Đầu: $O(1)$ / Giữa, cuối: $O(N)$ | Tốn bộ nhớ con trỏ `next` |
| **Doubly Linked List** | $O(N)$ | $O(N)$ | Đầu/Cuối: $O(1)$ | Node đã biết: $O(1)$ | Có cả con trỏ `prev` và `next` |
| **Stack (LIFO)** | Không hỗ trợ | $O(N)$ | Push: $O(1)$ | Pop: $O(1)$ | Cài bằng mảng hoặc DSLK |
| **Queue (FIFO)** | Không hỗ trợ | $O(N)$ | Enqueue: $O(1)$ | Dequeue: $O(1)$ | Cài bằng DSLK hoặc mảng vòng |
| **Binary Search Tree (BST)** | Không hỗ trợ | TB: $O(\log N)$ / Xấu: $O(N)$ | TB: $O(\log N)$ / Xấu: $O(N)$ | TB: $O(\log N)$ / Xấu: $O(N)$ | Bị suy biến thành đường thẳng nếu dữ liệu đã sắp |
| **AVL Tree** | Không hỗ trợ | Luôn là $O(\log N)$ | Luôn là $O(\log N)$ | Luôn là $O(\log N)$ | Chiều cao nghiêm ngặt $\le 1.44 \log_2 N$ |
| **Binary Heap** | Top: $O(1)$ | $O(N)$ | Push: $O(\log N)$ | Pop: $O(\log N)$ | Heapify: $O(N)$ Bottom-Up |
| **Hash Table** | Không hỗ trợ | TB: $O(1)$ / Xấu: $O(N)$ | TB: $O(1)$ / Xấu: $O(N)$ | TB: $O(1)$ / Xấu: $O(N)$ | Tránh đụng độ băm tốt |

### 2. Thuật toán sắp xếp

| Thuật toán | Trường hợp tốt nhất | Trường hợp trung bình | Trường hợp xấu nhất | Bộ nhớ phụ | Tính ổn định (Stable) |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Selection Sort** | $O(N^2)$ | $O(N^2)$ | $O(N^2)$ | $O(1)$ | Không |
| **Insertion Sort** | $O(N)$ (đã sắp) | $O(N^2)$ | $O(N^2)$ | $O(1)$ | Có |
| **Bubble Sort** | $O(N)$ (có cờ hiệu) | $O(N^2)$ | $O(N^2)$ | $O(1)$ | Có |
| **Quick Sort** | $O(N \log N)$ | $O(N \log N)$ | $O(N^2)$ (pivot tồi) | $O(\log N)$ (call stack) | Không |
| **Merge Sort** | $O(N \log N)$ | $O(N \log N)$ | $O(N \log N)$ | $O(N)$ | Có |
| **Heap Sort** | $O(N \log N)$ | $O(N \log N)$ | $O(N \log N)$ | $O(1)$ | Không |
