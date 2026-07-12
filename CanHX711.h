#ifndef CAN_HX711_H
#define CAN_HX711_H

#include <Arduino.h>
#include "HX711.h"
#include "CauHinhChan.h"

class CanHX711 {
public:
  CanHX711(uint8_t chanDT, uint8_t chanSCK, float heSoHieuChuan);

  bool begin();
  void tare(uint8_t soLan = 20);
  void capNhat(bool truGoc = false);
  bool docMauHieuChuan(float &giaTriMau);

  bool sanSang() const { return _sanSang; }
  bool daHieuChuan() const { return _daHieuChuan; }
  float getTrongLuong() const { return _trongLuong; }
  float getTrongLuongTho() const { return _trongLuongTho; }
  float getTrongLuongGoc() const { return _trongLuongGoc; }
  void datTrongLuongGoc(float giaTri);
  void datDaHieuChuan(bool trangThai);

private:
  HX711 _can;
  uint8_t _chanDT;
  uint8_t _chanSCK;
  float _heSoHieuChuan;
  bool _sanSang;
  bool _daHieuChuan;
  float _trongLuong;
  float _trongLuongTho;
  float _trongLuongGoc;
};

#endif
