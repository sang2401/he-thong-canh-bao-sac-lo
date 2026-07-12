#ifndef HE_THONG_CANH_BAO_H
#define HE_THONG_CANH_BAO_H

#include <Arduino.h>
#include "CauHinhChan.h"

class HeThongCanhBao {
public:
  HeThongCanhBao(uint8_t ledXanhLa, uint8_t ledXanhDuong, uint8_t ledDo);

  void khoiTaoChan();
  MucCanhBao danhGia(float gocNghieng, float rungDong, int doAm, float trongLuong);
  void capNhatLED(MucCanhBao muc);
  MucCanhBao getMucHienTai() const { return _mucHienTai; }

private:
  uint8_t _ledXanhLa;
  uint8_t _ledXanhDuong;
  uint8_t _ledDo;
  MucCanhBao _mucHienTai;
};

#endif
