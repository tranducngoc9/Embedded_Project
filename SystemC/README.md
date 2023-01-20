## Mô phỏng hệ thống tàu điện sử dụng SystemC :
- kiểm tra carid ID của khách hàng
- kiểm tra và thanh toán tự động
- đóng mở barie tự động
- kiểm tra khách lên và xuống
- ......

### Run on unbuntu:
```bash
g++ -I. -I$SYSTEMC_HOME/include -L. -L$SYSTEMC_HOME/lib-linux64 -Wl,-rpath=$SYSTEMC_HOME/lib-linux64 -o testC testC.cpp -lsystemc -lm
```
