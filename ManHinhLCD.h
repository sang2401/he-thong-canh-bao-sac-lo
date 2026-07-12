#ifndef MAN_HINH_LCD_H
#define MAN_HINH_LCD_H

#include <Arduino.h>
#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include "CauHinhChan.h"

class ManHinhLCD {
public:
  ManHinhLCD(uint8_t diaChi, uint8_t cot, uint8_t hang);

  void begin();
  void hienThiThongBao(const __FlashStringHelper *dong1, const __FlashStringHelper *dong2 = nullptr);
  void hienThiTienTrinhHieuChuan(int mau, int tong);
  void hienThiDuLieu(bool hienGoc, unsigned long thoiGianBatDauHienGoc,
                     bool gy87Ok, bool hx711Ok,
                     float gocNghiengGoc, float rungDongGoc, float trongLuongGoc,
                     int phanTramDoAm, float gocNghieng, float rungDong, float trongLuongTho);
  void clear();

private:
  LiquidCrystal_I2C _lcd;
  bool _ketNoi();
};

#endif
