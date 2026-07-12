#include "CanHX711.h"
#include <math.h>

CanHX711::CanHX711(uint8_t chanDT, uint8_t chanSCK, float heSoHieuChuan)
  : _chanDT(chanDT), _chanSCK(chanSCK), _heSoHieuChuan(heSoHieuChuan),
    _sanSang(false), _daHieuChuan(false),
    _trongLuong(0), _trongLuongTho(0), _trongLuongGoc(0) {}

bool CanHX711::begin() {
  _can.begin(_chanDT, _chanSCK);
  delay(500);

  if (!_can.is_ready()) {
    _sanSang = false;
    return false;
  }

  _sanSang = true;
  _can.set_scale(_heSoHieuChuan);
  _can.tare(20);
  delay(500);

  if (abs(_can.get_units(10)) > 0.3f) {
    _can.tare(20);
    delay(500);
  }
  return true;
}

void CanHX711::tare(uint8_t soLan) {
  if (_sanSang) {
    _can.tare(soLan);
  }
}

void CanHX711::capNhat(bool truGoc) {
  if (!_can.is_ready()) return;

  float giaTriDocDuoc = _can.get_units(5);
  if (isnan(giaTriDocDuoc) || isinf(giaTriDocDuoc)) return;

  _trongLuongTho = (giaTriDocDuoc < 0) ? -giaTriDocDuoc : giaTriDocDuoc;
  float trongLuongTruocGoc = _trongLuongTho;

  if (abs(_trongLuongTho) < NGUONG_KHOI_DONG_VE_0) {
    trongLuongTruocGoc = 0;
  }

  if (truGoc && _daHieuChuan && _sanSang) {
    _trongLuong = trongLuongTruocGoc - _trongLuongGoc;
  } else {
    _trongLuong = trongLuongTruocGoc;
  }

  _trongLuong = round(_trongLuong * 100) / 100.0;
  if (_trongLuong < 0) _trongLuong = 0;
}

bool CanHX711::docMauHieuChuan(float &giaTriMau) {
  if (!_sanSang || !_can.is_ready()) return false;

  float giaTriCanGoc = _can.get_units(1);
  if (isnan(giaTriCanGoc) || isinf(giaTriCanGoc)) return false;
  if (abs(giaTriCanGoc) < NGUONG_KHOI_DONG_VE_0) return false;

  giaTriMau = giaTriCanGoc;
  return true;
}

void CanHX711::datTrongLuongGoc(float giaTri) {
  _trongLuongGoc = giaTri;
  if (abs(_trongLuongGoc) < NGUONG_KHOI_DONG_VE_0) {
    _trongLuongGoc = 0;
  }
}

void CanHX711::datDaHieuChuan(bool trangThai) {
  _daHieuChuan = trangThai;
}
