# CHƯƠNG 4: THIẾT KẾ & PHÂN TÍCH GIẢI THUẬT (ALGORITHMS)

---

## 1. TƯ DUY ĐỘ PHỨC TẠP THUẬT TOÁN (BIG-O CHO NGƯỜI MỚI)

Nhiều bạn sợ Big-O vì nghĩ nó là toán cao cấp, nhưng thực chất Big-O chỉ là câu hỏi: **"Khi số lượng dữ liệu $N$ tăng lên gấp 10, gấp 100 lần, thì máy tính mất thêm bao nhiêu công sức?"**

- $O(1)$ - **Tức thì**: Lấy đồ trong túi áo ra xem. Dù túi bạn có 1 cái kẹo hay 1 triệu cái kẹo thì thọc tay vào lấy ngay món đầu tiên luôn tốn 1 tích tắc. Ví dụ: `arr[0]`, `stack.push()`.
- $O(\log N)$ - **Cực kỳ nhanh (Chia đôi liên tục)**: Tra từ điển hoặc đoán số từ 1 đến 100. Mỗi lần đoán bạn chia đôi phạm vi còn lại. Có 1 triệu phần tử thì bạn chỉ tốn khoảng 20 lần chia đôi là tìm ra!
- $O(N)$ - **Tuyến tính**: Điểm danh từng người trong lớp. Lớp 50 người điểm danh 50 lần, lớp 100 người điểm danh 100 lần. Ví dụ: vòng lặp `for (int i = 0; i < N; i++)`.
- $O(N \log N)$ - **Tốc độ vàng của thuật toán sắp xếp tốt**: QuickSort, MergeSort, HeapSort. Với 1 triệu phần tử, máy chỉ mất khoảng 20 triệu phép tính (chạy trong chớp mắt 0.05 giây).
- $O(N^2)$ - **Chậm chạp (Hai vòng lặp lồng nhau)**: Mỗi người trong lớp bắt tay với tất cả những người còn lại. Lớp 1000 người mất 1.000.000 cái bắt tay. Ví dụ: Bubble Sort lồng nhau.

---

## 2. GIẢI THUẬT SẮP XẾP NHANH (QUICKSORT) DỄ HIỂU NHẤT

### 2.1. Ý tưởng đời thực: "Chia phe theo mốc"
Tưởng tượng bạn là lớp trưởng cần xếp hàng học sinh theo chiều cao:
1. Bạn chọn bừa một bạn làm **mốc (Pivot)**, ví dụ bạn Nam cao 1m60.
2. Bạn hô: "Ai thấp hơn Nam đứng dạt hết về bên trái, ai cao hơn Nam dạt hết về bên phải!".
3. Lúc này bạn Nam đứng ở **chính giữa**, và bạn Nam đã đứng **đúng 100% vị trí cuối cùng của mình** trong hàng ngũ!
4. Giờ bạn chỉ cần lặp lại đúng câu hô đó cho nhóm bên trái và nhóm bên phải cho tới khi xong. Đó chính là QuickSort!

### 2.2. Minh họa từng bước code Partition:
```cpp
int partition(T array[], int trai, int phai) {
    // Chọn phần tử ở giữa làm mốc
    int viTriChot = trai + (phai - trai) / 2;
    T giaTriChot = array[viTriChot];

    // Mẹo: Cất tạm mốc xuống cuối mảng để không vướng chân lúc duyệt
    doiCho(array[viTriChot], array[phai]);

    int ranhGioi = trai; // Đánh dấu nơi các phần tử nhỏ hơn mốc sẽ đứng
    for (int j = trai; j < phai; j++) {
        // Thằng nào nhỏ hơn mốc thì kéo nó về phía bên trái ranh giới
        if (array[j] < giaTriChot) {
            doiCho(array[ranhGioi], array[j]);
            ranhGioi++;
        }
    }
    // Trả mốc từ cuối mảng về đúng vị trí ranh giới ở giữa
    doiCho(array[ranhGioi], array[phai]);
    return ranhGioi;
}
```

---

## 3. TÌM KIẾM NHỊ PHÂN (BINARY SEARCH)

### 3.1. Ý tưởng:
Tìm một số trong mảng **đã được sắp xếp**. 
Thay vì duyệt từ đầu tới cuối ($O(N)$), ta luôn kiểm tra phần tử chính giữa:
- Bằng giá trị cần tìm $\rightarrow$ Xong!
- Nhỏ hơn giá trị cần tìm $\rightarrow$ Vứt bỏ toàn bộ nửa bên trái, chỉ tìm ở nửa bên phải.
- Lớn hơn giá trị cần tìm $\rightarrow$ Vứt bỏ toàn bộ nửa bên phải, chỉ tìm ở nửa bên trái.

```cpp
int binarySearch(int arr[], int n, int target) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1; // Không tìm thấy
}
```

---

## 4. GIẢI THUẬT ĐỒ THỊ CƠ BẢN (GRAPH ALGORITHMS)

```
        (A)
       /   \
     (B)---(C)
      |     |
     (D)---(E)
```

### 4.1. BFS (Breadth-First Search - Duyệt theo chiều rộng):
- **Tư duy:** Giống như gợn sóng lan tỏa trên mặt nước khi ném hòn đá. Thăm hết những người bạn trực tiếp (khoảng cách 1), rồi mới thăm tới bạn của bạn (khoảng cách 2).
- **Dùng:** Cấu trúc dữ liệu **Hàng đợi (Queue - FIFO)**.
- **Ứng dụng:** Tìm đường đi **ngắn nhất có ít chặng nhất** trên đồ thị không có trọng số (ví dụ: tìm số lần chuyển trạm xe buýt ít nhất!).

### 4.2. DFS (Depth-First Search - Duyệt theo chiều sâu):
- **Tư duy:** Giống như đi vào mê cung. Bạn cứ cắm đầu đi thẳng một mạch cho tới khi gặp đường cụt thì mới quay lui lại một ngã rẽ (Backtracking) để đi đường khác.
- **Dùng:** Cấu trúc dữ liệu **Ngăn xếp (Stack - LIFO)** hoặc hàm đệ quy.
- **Ứng dụng:** Tìm chu trình, duyệt tất cả các trường hợp giải đố (Sudoku, cờ vua).
