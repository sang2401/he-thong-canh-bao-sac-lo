#include "ManHinhLCD.h"

ManHinhLCD::ManHinhLCD(uint8_t diaChi, uint8_t cot, uint8_t hang)
  : _lcd(diaChi, cot, hang) {}

void ManHinhLCD::begin() {
  _lcd.init();
  _lcd.backlight();
}

bool ManHinhLCD::_ketNoi() {
  Wire.beginTransmission(DIA_CHI_LCD);
  return Wire.endTransmission() == 0;
}

void ManHinhLCD::clear() {
  _lcd.clear();
}

void ManHinhLCD::hienThiThongBao(const __FlashStringHelper *dong1, const __FlashStringHelper *dong2) {
  _lcd.setCursor(0, 0);
  _lcd.print(dong1);
  if (dong2) {
    _lcd.setCursor(0, 1);
    _lcd.print(dong2);
  }
}

void ManHinhLCD::hienThiTienTrinhHieuChuan(int mau, int tong) {
  _lcd.setCursor(0, 1);
  _lcd.print(F("Mau do: "));
  _lcd.print(mau);
  _lcd.print(F("/"));
  _lcd.print(tong);
}

void ManHinhLCD::hienThiDuLieu(bool hienGoc, unsigned long thoiGianBatDauHienGoc,
                               bool gy87Ok, bool hx711Ok,
                               float gocNghiengGoc, float rungDongGoc, float trongLuongGoc,
                               int phanTramDoAm, float gocNghieng, float rungDong, float trongLuongTho) {
  if (!_ketNoi()) return;

  unsigned long thoiGianHienTai = millis();

  if (hienGoc) {
    _lcd.setCursor(0, 0);
    _lcd.print(F("GIA TRI GOC_"));

    unsigned long thoiGianConLai = (THOI_GIAN_HIEN_GOC - (thoiGianHienTai - thoiGianBatDauHienGoc)) / 1000;
    _lcd.setCursor(12, 0);
    if (thoiGianConLai < 10) _lcd.print(F("0"));
    _lcd.print(thoiGianConLai);
    _lcd.print(F("s"));

    _lcd.setCursor(0, 1);
    if (gy87Ok) {
      _lcd.print(F("N:")); _lcd.print(gocNghiengGoc, 1);
      _lcd.print(F(" R:")); _lcd.print(rungDongGoc, 1);
    }
    if (hx711Ok) {
      if (gy87Ok) _lcd.print(F(" "));
      _lcd.print(F("L:")); _lcd.print(trongLuongGoc, 1);
    }
  } else {
    _lcd.setCursor(0, 0);
    _lcd.print(F("DA:"));
    if (phanTramDoAm < 100) _lcd.print(F(" "));
    if (phanTramDoAm < 10) _lcd.print(F(" "));
    _lcd.print(phanTramDoAm); _lcd.print(F("%"));

    _lcd.setCursor(9, 0);
    _lcd.print(F("G:"));
    if (gy87Ok) {
      if (gocNghieng < 100) _lcd.print(F(" "));
      if (gocNghieng < 10) _lcd.print(F(" "));
      _lcd.print(gocNghieng, 1); _lcd.print((char)223);
    } else {
      _lcd.print(F("--"));
    }

    _lcd.setCursor(0, 1);
    _lcd.print(F("R:"));
    if (gy87Ok) {
      if (rungDong < 10) _lcd.print(F(" "));
      _lcd.print(rungDong, 1); _lcd.print(F("m"));
    } else {
      _lcd.print(F("--"));
    }

    _lcd.setCursor(8, 1);
    _lcd.print(F("T:"));
    if (hx711Ok) {
      long giaTriLayNguyen = (long)((trongLuongTho < 0) ? -trongLuongTho : trongLuongTho);
      if (giaTriLayNguyen >= 10000) {
        _lcd.print(giaTriLayNguyen / 1000); _lcd.print(F("k"));
      } else {
        if (giaTriLayNguyen < 1000) _lcd.print(F(" "));
        if (giaTriLayNguyen < 100) _lcd.print(F(" "));
        if (giaTriLayNguyen < 10) _lcd.print(F(" "));
        _lcd.print(giaTriLayNguyen);
      }
    } else {
      _lcd.print(F("---"));
    }
  }
}
