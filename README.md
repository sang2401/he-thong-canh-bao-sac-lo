# Hệ thống cảnh báo sạt lở tự động

Hệ thống giám sát điều kiện địa hình trên taluy và cảnh báo sớm nguy cơ sạt lở. Đo góc nghiêng, rung động, độ ẩm đất và trọng lượng đất, đánh giá mức cảnh báo và kích hoạt tín hiệu báo động khi có nguy hiểm.

## Luồng hoạt động
1. Cảm biến GY-87 (IMU) đo góc nghiêng và rung động.
2. Cảm biến độ ẩm đất và cân HX711 đo lượng/trọng lượng đất.
3. Đánh giá mức: Bình thường → Cảnh báo nhẹ → Cảnh báo cao → Báo động sạt lở.
4. Bật LED, còi và hiển thị trạng thái trên LCD I2C.

## Công nghệ sử dụng
- Phần cứng: Arduino, GY-87 IMU, HX711, cảm biến độ ẩm đất, LCD I2C, LED, Buzzer
- Ngôn ngữ: C++ (Arduino)
- IDE: Arduino IDE
