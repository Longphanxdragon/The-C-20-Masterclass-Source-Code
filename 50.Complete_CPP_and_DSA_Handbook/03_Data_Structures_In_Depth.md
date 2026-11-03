# CHƯƠNG 3: CẤU TRÚC DỮ LIỆU CHUYÊN SÂU (DATA STRUCTURES IN DEPTH)

---

## 1. DANH SÁCH LIÊN KẾT (LINKED LISTS)

Khác với mảng (`array`/`vector`) lưu trữ các phần tử trên vùng nhớ liền kề nhau, **Danh sách liên kết** lưu trữ các phần tử trong các `Node` nằm rải rác trên Heap, liên kết với nhau bằng con trỏ (`next` hoặc `prev`).

### 1.1. So sánh 3 biến thể Danh sách liên kết

```
1. Singly Linked List (Chuẩn có Dummy Node):
   [head] -> [data 1] -> [data 2] -> [tail] -> nullptr
     ^                                 ^
     |                                 |
   dummy                             dummy (tail->next trỏ vào node cuối thực tế)

2. Doubly Linked List (DSLK Kép có 2 lính canh):
   nullptr <- [head] <=======> [data 1] <=======> [data 2] <=======> [tail] -> nullptr

3. Circular Linked List (DSLK Vòng):
   [head: data 1] -> [data 2] -> [tail: data 3]
         ^                             |
         +-----------------------------+ (tail->next = head)
```

### 1.2. Kỹ thuật Dummy / Sentinel Node (Node lính canh)
- **Vấn đề:** Khi xóa hoặc chèn phần tử ở đầu mảng (`index = 0`), ta luôn phải viết câu lệnh `if (head == nullptr)` hoặc cập nhật con trỏ `head`, rất dễ sinh ra lỗi null pointer dereference.
- **Giải pháp:** Sử dụng 2 node bù nhìn không chứa dữ liệu người dùng (`head` và `tail`). Mọi node dữ liệu thực tế luôn nằm kẹp giữa `head` và `tail`.
  - Không bao giờ cần kiểm tra con trỏ rỗng khi thao tác chèn/xóa ở đầu hoặc cuối.
  - Mã nguồn ngắn gọn, loại bỏ hoàn toàn các trường hợp biên đặc biệt.

### 1.3. Các bài toán kinh điển trên Danh sách liên kết
1. **Phát hiện chu trình (Floyd's Cycle-Finding Algorithm - Rùa và Thỏ):**
   - Dùng 2 con trỏ: `slow` đi mỗi lần 1 bước, `fast` đi mỗi lần 2 bước.
   - Nếu `slow == fast` $\rightarrow$ Danh sách có vòng lặp ($O(N)$ thời gian, $O(1)$ bộ nhớ).
2. **Đảo ngược danh sách liên kết (Reverse Linked List):**
   ```cpp
   Node* daoNguoc(Node* head) {
       Node* prev = nullptr;
       Node* curr = head;
       while (curr != nullptr) {
           Node* nextTemp = curr->next;
           curr->next = prev;
           prev = curr;
           curr = nextTemp;
       }
       return prev;
   }
   ```

---

## 2. CÂY NHỊ PHÂN TÌM KIẾM (BINARY SEARCH TREE - BST)

### 2.1. Định nghĩa và Tính chất
- Mỗi node có tối đa 2 con: `pLeft` và `pRight`.
- **Bất biến BST:** Với mọi node $X$:
  $$\forall Y \in \text{Cây con trái}(X): \text{key}(Y) < \text{key}(X)$$
  $$\forall Z \in \text{Cây con phải}(X): \text{key}(Z) > \text{key}(X)$$
- **Duyệt trung thứ tự (In-order Traversal - LNR):** Luôn cho ra dãy khóa được sắp xếp tăng dần!

### 2.2. Thao tác Xóa (Remove) trong BST — 3 trường hợp kinh điển
```
TH1: Node lá (0 con)           TH2: Node có 1 con             TH3: Node có 2 con
      50                             50                             50 (Cần xóa)
     /  \                           /  \                           /  \
   30    70                       30    70                       30    70
    \                              \                            /  \   /
    [40] (Xóa: chỉ cần delete)     [40]                        20  40 60
                                      \                             \
                                       45 (Nối 30 thẳng tới 45)      45 (Successor là 60)
```

1. **Trường hợp 1 (Node lá - 0 con):** Xóa trực tiếp node và gán con trỏ của cha trỏ về `nullptr`.
2. **Trường hợp 2 (Node có đúng 1 con):** Bỏ qua node cần xóa, nối trực tiếp node cha vào node con duy nhất đó.
3. **Trường hợp 3 (Node có cả 2 con):**
   - Tìm **Inorder Successor** (phần tử nhỏ nhất ở cây con bên phải: đi sang phải 1 bước rồi rẽ trái hết cỡ). Hoặc tìm **Inorder Predecessor** (phần tử lớn nhất ở cây con bên trái).
   - Sao chép giá trị của Successor lên thay thế cho node cần xóa.
   - Gọi đệ quy xóa Successor khỏi cây con bên phải (lúc này Successor chỉ có tối đa 1 con nên bài toán quy về TH1 hoặc TH2).

---

## 3. CÂY CÂN BẰNG AVL (AVL BALANCED TREE)

### 3.1. Tại sao cần AVL Tree?
Cây BST thông thường có nhược điểm lớn: nếu chèn dữ liệu theo thứ tự đã sắp xếp (ví dụ: `1, 2, 3, 4, 5`), cây sẽ bị **suy biến thành danh sách liên kết** với chiều cao $h = N$, khiến độ phức tạp tìm kiếm/chèn/xóa từ $O(\log N)$ tụt dốc thành $O(N)$!
**Cây AVL** giải quyết triệt để vấn đề này bằng cách tự động cân bằng sau mỗi thao tác thêm/xóa để đảm bảo chiều cao luôn duy trì:
$$h \le 1.44 \log_2(N + 2) \rightarrow O(\log N)$$

### 3.2. Hệ số cân bằng (Balance Factor)
$$\text{bfactor} = \text{height}(\text{pRight}) - \text{height}(\text{pLeft})$$
- Cây cân bằng chuẩn khi: $\text{bfactor} \in \{-1, 0, +1\}$.
- Cây bị mất cân bằng khi: $|\text{bfactor}| > 1$.

### 3.3. Bốn phép xoay cân bằng (4 Rotations)

```
1. Left-Left (LL) - Lệch Trái-Trái       ==> Xoay Phải (Rotate Right) tại Gốc
         z (bf = -2)                           y
        /                                     / \
       y (bf = -1 hoặc 0)                    x   z
      /
     x

2. Right-Right (RR) - Lệch Phải-Phải     ==> Xoay Trái (Rotate Left) tại Gốc
     z (bf = +2)                               y
      \                                       / \
       y (bf = +1 hoặc 0)                    z   x
        \
         x

3. Left-Right (LR) - Lệch Trái-Phải      ==> Xoay Trái tại Con Trái, rồi Xoay Phải tại Gốc
         z                                     z                                 y
        /                                     /                                 / \
       x         -- Xoay Trái x -->          y         -- Xoay Phải z -->      x   z
        \                                   /
         y                                 x

4. Right-Left (RL) - Lệch Phải-Trái      ==> Xoay Phải tại Con Phải, rồi Xoay Trái tại Gốc
       z                                     z                                   y
        \                                     \                                 / \
         x       -- Xoay Phải x -->            y       -- Xoay Trái z -->      z   x
        /                                       \
       y                                         x
```

---

## 4. ĐỐNG NHỊ PHÂN (BINARY HEAP / PRIORITY QUEUE)

### 4.1. Cấu trúc mảng phẳng biểu diễn Cây nhị phân hoàn chỉnh
Binary Heap là một cây nhị phân hoàn chỉnh (mọi tầng đều đầy, tầng cuối cùng được điền từ trái sang phải), do đó được lưu trữ trực tiếp trên **mảng 1 chiều liên tục** mà không tốn con trỏ:
Với phần tử tại chỉ số $i$ (bắt đầu từ $0$):
- **Con trái:** $\text{left}(i) = 2i + 1$
- **Con phải:** $\text{right}(i) = 2i + 2$
- **Nút cha:** $\text{parent}(i) = \lfloor \frac{i - 1}{2} \rfloor$

### 4.2. Thao tác cốt lõi
- **`reheapUp(position)` (Nổi lên):** Dùng khi `push()`. Phần tử mới đặt ở cuối mảng, so sánh với cha; nếu ưu tiên hơn cha thì đổi chỗ, lặp lại cho tới gốc. Chi phí: $O(\log N)$.
- **`reheapDown(position)` (Chìm xuống):** Dùng khi `pop()`. Đưa phần tử cuối cùng lên thay thế gốc, so sánh với 2 con; đổi chỗ với đứa con có độ ưu tiên cao nhất, lặp lại cho tới lá. Chi phí: $O(\log N)$.

### 4.3. Chứng minh toán học: Thuật toán `heapify` Bottom-Up chạy trong $O(N)$
Nhiều sinh viên nhầm lẫn rằng tạo heap từ mảng $N$ phần tử mất $O(N \log N)$ (do gọi `push` $N$ lần). Nhưng giải thuật **Bottom-Up Heapify** (duyệt từ nút $\lfloor N/2 \rfloor - 1$ ngược về $0$ và gọi `reheapDown`) chỉ mất kỳ diệu đúng **$O(N)$**!

#### Chứng minh:
- Ở độ cao $h = 0$ (các lá, chiếm $N/2$ phần tử): tốn $0$ phép đổi chỗ.
- Ở độ cao $h = 1$ (chiếm $N/4$ phần tử): tốn tối đa $1$ phép đổi chỗ.
- Ở độ cao $h$ (chiếm $\frac{N}{2^{h+1}}$ phần tử): tốn tối đa $h$ phép đổi chỗ.
Tổng số phép thao tác:
$$S = \sum_{h=0}^{\log_2 N} \frac{N}{2^{h+1}} \cdot h = \frac{N}{2} \sum_{h=0}^{\infty} \frac{h}{2^h}$$
Áp dụng công thức chuỗi số học - hình học: $\sum_{h=0}^{\infty} \frac{h}{2^h} = 2$.
$$\Rightarrow S \le \frac{N}{2} \cdot 2 = N \rightarrow \mathbf{O(N)}$$
Một kết quả đẹp đẽ và cực kỳ tối ưu của giải thuật vun đống!
