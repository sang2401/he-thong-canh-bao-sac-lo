#include "HeThongCanhBao.h"

HeThongCanhBao::HeThongCanhBao(uint8_t ledXanhLa, uint8_t ledXanhDuong, uint8_t ledDo)
  : _ledXanhLa(ledXanhLa), _ledXanhDuong(ledXanhDuong), _ledDo(ledDo),
    _mucHienTai(BINH_THUONG) {}

void HeThongCanhBao::khoiTaoChan() {
  pinMode(_ledXanhLa, OUTPUT);
  pinMode(_ledXanhDuong, OUTPUT);
  pinMode(_ledDo, OUTPUT);
  digitalWrite(_ledXanhLa, LOW);
  digitalWrite(_ledXanhDuong, LOW);
  digitalWrite(_ledDo, LOW);
}

MucCanhBao HeThongCanhBao::danhGia(float gocNghieng, float rungDong, int doAm, float trongLuong) {
  if (gocNghieng > NGUONG_NGHIENG_SAT_LO ||
      (rungDong >= RUNG_DONG_MANH && doAm > DO_AM_CAO) ||
      trongLuong <= TRONG_LUONG_SAT_LO ||
      (gocNghieng > NGUONG_NGHIENG_CAO && trongLuong <= TRONG_LUONG_CAO)) {
    _mucHienTai = BAO_DONG_SAT_LO;
  }
  else if ((doAm > DO_AM_CAO && gocNghieng > NGUONG_NGHIENG_CAO) ||
           rungDong >= RUNG_DONG_MANH ||
           trongLuong <= TRONG_LUONG_CAO ||
           (doAm > DO_AM_BINH_THUONG && trongLuong <= TRONG_LUONG_CANH_BAO)) {
    _mucHienTai = CANH_BAO_CAO;
  }
  else if ((doAm > DO_AM_BINH_THUONG && gocNghieng > NGUONG_NGHIENG_CANH_BAO) ||
           trongLuong <= TRONG_LUONG_CANH_BAO) {
    _mucHienTai = CANH_BAO_NHE;
  }
  else if (doAm <= DO_AM_BINH_THUONG &&
           gocNghieng <= NGUONG_NGHIENG_BINH_THUONG &&
           rungDong < RUNG_DONG_MANH &&
           trongLuong > TRONG_LUONG_BINH_THUONG) {
    _mucHienTai = BINH_THUONG;
  }
  else {
    _mucHienTai = CANH_BAO_NHE;
  }
  return _mucHienTai;
}

void HeThongCanhBao::capNhatLED(MucCanhBao muc) {
  digitalWrite(_ledXanhLa, (muc == BINH_THUONG) ? HIGH : LOW);
  digitalWrite(_ledXanhDuong, (muc == CANH_BAO_NHE) ? HIGH : LOW);

  if (muc == BAO_DONG_SAT_LO) {
    digitalWrite(_ledDo, (millis() % 200 < 100) ? HIGH : LOW);
  } else if (muc == CANH_BAO_CAO) {
    digitalWrite(_ledDo, HIGH);
  } else {
    digitalWrite(_ledDo, LOW);
  }
}
