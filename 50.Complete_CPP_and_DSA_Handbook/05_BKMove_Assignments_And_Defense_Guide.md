# CHƯƠNG 5: CẨM NANG BÀI TẬP LỚN BKMOVE & KINH NGHIỆM VẤN ĐÁP

---

## 1. TỔNG QUAN KIẾN TRÚC BKMOVE QUA 2 HỌC PHẦN

Ứng dụng **BKMove** mô phỏng một hệ thống điều phối và tìm kiếm hành trình xe buýt thông minh tại TP.HCM:
- **Bài tập lớn 1 (BTL 1):** 
  - Lưu trữ tuyến xe có 2 chiều (chiều đi `OUTBOUND` và chiều về `INBOUND`) bằng một danh sách vòng duy nhất (`CircularLinkedList<BusStop>`).
  - Tìm kiếm lộ trình đi thẳng (Direct) và lộ trình 1 lần chuyển tuyến (1-Transfer).
  - Sắp xếp kết quả bằng thuật toán `QuickSort` với bộ so sánh ưu tiên nhiều tiêu chí (Tie-break).
- **Bài tập lớn 2 (BTL 2):**
  - Quản lý danh mục trạm xe buýt và tìm kiếm nhanh tiền tố (Prefix Search) có phân trang bằng cây nhị phân cân bằng `AVL Tree`.
  - Quản lý bảng hiển thị chuyến xe cập bến theo thời gian thực (Live Arrival Board) cho từng trạm buýt bằng `Binary Heap (Min-Heap)`.

---

## 2. BÍ KÍP TRẢ LỜI CÁC CÂU HỎI BẪY KHI BẢO VỆ MÔN DSA

### Câu 1: "Tại sao trong BTL1 hàm `build` lại bỏ qua trạm đầu và trạm cuối của chiều `inbound`?"
- **Trả lời:** 
  "Dạ thưa thầy/cô, một tuyến xe buýt đi khép kín thành một vòng tròn. Trạm cuối cùng của chiều đi (`outbound`) chính là trạm đầu tiên của chiều về (`inbound`), và trạm cuối cùng của chiều về lại chính là trạm xuất phát đầu tiên của chiều đi. Nếu ta thêm cả 2 trạm này vào danh sách vòng thì 2 trạm đầu/cuối sẽ bị lặp lại hai lần, làm sai lệch số trạm và số chặng thực tế. Do đó ta giữ nguyên chiều đi và chỉ lấy các trạm ở giữa của chiều về ạ."

### Câu 2: "Tại sao trong BTL2 em lại dùng 2 cây AVL riêng biệt (`cayStopId` và `caySearchKey`)?"
- **Trả lời:**
  "Dạ thưa thầy/cô, một cây AVL chỉ có thể sắp xếp và cân bằng theo **một khóa (Key) duy nhất**:
  1. Cây `cayStopId` dùng `stopId` làm key để tìm kiếm, xóa trạm theo mã trạm với tốc độ $O(\log N)$.
  2. Cây `caySearchKey` dùng chuỗi kết hợp `searchKey + \t + stopId` làm key. Nhờ đó, khi ta duyệt cây theo thứ tự LNR (`ascendingList`), các trạm sẽ tự động được sắp xếp chuẩn theo tên trạm tăng dần và giải quyết trường hợp trùng tên bằng mã trạm, giúp hàm tìm kiếm tiền tố và phân trang (`findStops`) chạy cực kỳ chính xác và nhanh chóng ạ."

### Câu 3: "Khi xóa trạm (`removeStop`), tại sao phải kiểm tra heap của trạm trước?"
- **Trả lời:**
  "Dạ vì một trạm xe buýt chỉ có thể được gỡ bỏ khỏi hệ thống khi không còn bất kỳ chuyến xe nào đang trên đường đến trạm đó. Nếu heap của trạm vẫn còn sự kiện đến (`pHeap->size() > 0`), hàm sẽ từ chối xóa và trả về `false` để đảm bảo tính toàn vẹn dữ liệu ạ."

### Câu 4: "Tại sao trong cây BST/AVL khi xóa node có 2 con, ta lại chọn Inorder Successor?"
- **Trả lời:**
  "Dạ thưa thầy/cô, Inorder Successor là phần tử nhỏ nhất ở nhánh cây con bên phải. Vì nó nhỏ nhất bên phải nên nó lớn hơn mọi phần tử bên trái và nhỏ hơn mọi phần tử còn lại bên phải. Do đó khi đưa nó lên làm gốc mới thì tính chất của cây tìm kiếm nhị phân được giữ nguyên 100%. Hơn nữa, vì là phần tử nhỏ nhất bên phải nên nó chắc chắn không có con trái (tối đa chỉ có 1 con phải), việc xóa nó khỏi nhánh phải quy về trường hợp xóa node lá hoặc node 1 con cực kỳ đơn giản ạ."
