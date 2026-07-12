#include "GY87_IMU.h"
#include <math.h>

GY87_IMU::GY87_IMU()
  : _daKetNoi(false), _daHieuChuan(false),
    _tongGocNghieng(0), _doRungDong(0),
    _gocNghiengGoc(0), _doRungDongGoc(0),
    _gocX(0), _gocY(0), _thoiGianDocTruoc(0),
    _nhietDo(0), _apSuat(0) {}

bool GY87_IMU::begin() {
  Wire.beginTransmission(DIA_CHI_MPU6050);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission();

  Wire.beginTransmission(DIA_CHI_HMC5883L);
  Wire.write(0x02);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.beginTransmission(DIA_CHI_BMP180);
  _daKetNoi = (Wire.endTransmission() == 0);
  return _daKetNoi;
}

void GY87_IMU::resetBoLoc() {
  _gocX = 0;
  _gocY = 0;
  _thoiGianDocTruoc = 0;
}

bool GY87_IMU::docMauHieuChuan(float &gocNghieng, float &bienDoRung) {
  if (!_daKetNoi) return false;

  Wire.beginTransmission(DIA_CHI_MPU6050);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(DIA_CHI_MPU6050, 14, true);

  if (Wire.available() < 14) return false;

  int16_t truocAx = Wire.read() << 8 | Wire.read();
  int16_t truocAy = Wire.read() << 8 | Wire.read();
  int16_t truocAz = Wire.read() << 8 | Wire.read();
  Wire.read(); Wire.read();
  Wire.read(); Wire.read(); Wire.read(); Wire.read();
  Wire.read(); Wire.read();

  float ax = (float)truocAx / TI_LE_GIA_TOC_MPU6050 * GIA_TOC_TRONG_TRUONG;
  float ay = (float)truocAy / TI_LE_GIA_TOC_MPU6050 * GIA_TOC_TRONG_TRUONG;
  float az = (float)truocAz / TI_LE_GIA_TOC_MPU6050 * GIA_TOC_TRONG_TRUONG;

  float giaTocToanPhan = sqrt(ax * ax + ay * ay + az * az);
  bienDoRung = abs(giaTocToanPhan - GIA_TOC_TRONG_TRUONG) * 5.8f;
  bienDoRung = constrain(bienDoRung, 0.0f, 90.0f);

  gocNghieng = 0;
  if (giaTocToanPhan > 0.1f) {
    float giaTocNamNgang = sqrt(ax * ax + ay * ay);
    gocNghieng = (az != 0 || giaTocNamNgang > 0.01f)
      ? (atan2(giaTocNamNgang, abs(az)) * 180.0f / PI)
      : 90.0f;
    gocNghieng = constrain(gocNghieng, 0.0f, 90.0f);
  }

  if (isnan(gocNghieng) || isinf(gocNghieng)) return false;
  if (isnan(bienDoRung) || isinf(bienDoRung)) return false;
  return true;
}

void GY87_IMU::capNhat(bool truGoc) {
  if (!_daKetNoi) return;

  Wire.beginTransmission(DIA_CHI_MPU6050);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(DIA_CHI_MPU6050, 14, true);

  if (Wire.available() >= 14) {
    int16_t giaTocX = Wire.read() << 8 | Wire.read();
    int16_t giaTocY = Wire.read() << 8 | Wire.read();
    int16_t giaTocZ = Wire.read() << 8 | Wire.read();
    _nhietDo = (Wire.read() << 8 | Wire.read()) / 340.00f + 36.53f;
    int16_t vanTocGocX = Wire.read() << 8 | Wire.read();
    int16_t vanTocGocY = Wire.read() << 8 | Wire.read();
    int16_t vanTocGocZ = Wire.read() << 8 | Wire.read();

    float ax = (float)giaTocX / TI_LE_GIA_TOC_MPU6050 * GIA_TOC_TRONG_TRUONG;
    float ay = (float)giaTocY / TI_LE_GIA_TOC_MPU6050 * GIA_TOC_TRONG_TRUONG;
    float az = (float)giaTocZ / TI_LE_GIA_TOC_MPU6050 * GIA_TOC_TRONG_TRUONG;
    float gx = (float)vanTocGocX / TI_LE_VAN_TOC_GOC;
    float gy = (float)vanTocGocY / TI_LE_VAN_TOC_GOC;

    float giaTocToanPhan = sqrt(ax * ax + ay * ay + az * az);
    _doRungDong = abs(giaTocToanPhan - GIA_TOC_TRONG_TRUONG) * 5.8f;
    _doRungDong = constrain(_doRungDong, 0.0f, 90.0f);

    float gocGiaTocX = 0, gocGiaTocY = 0;
    if (giaTocToanPhan > 0.1f) {
      gocGiaTocX = atan2(ay, sqrt(ax * ax + az * az)) * 180.0f / PI;
      gocGiaTocY = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0f / PI;
    }

    unsigned long thoiGianHienTai = millis();
    bool lanDocDauTien = (_thoiGianDocTruoc == 0);
    float dt = 0.01f;

    if (!lanDocDauTien) {
      dt = (thoiGianHienTai - _thoiGianDocTruoc) / 1000.0f;
      dt = constrain(dt, 0.001f, 0.1f);
    }

    if (lanDocDauTien) {
      _gocX = gocGiaTocX;
      _gocY = gocGiaTocY;
    } else {
      _gocX = HE_SO_LOC_ALPHA * (_gocX + gx * dt) + (1 - HE_SO_LOC_ALPHA) * gocGiaTocX;
      _gocY = HE_SO_LOC_ALPHA * (_gocY + gy * dt) + (1 - HE_SO_LOC_ALPHA) * gocGiaTocY;
    }
    _thoiGianDocTruoc = thoiGianHienTai;

    _tongGocNghieng = sqrt(_gocX * _gocX + _gocY * _gocY);
    _tongGocNghieng = constrain(_tongGocNghieng, 0.0f, 90.0f);

    if (truGoc && _daHieuChuan) {
      _tongGocNghieng = max(0.0f, _tongGocNghieng - _gocNghiengGoc);
      _doRungDong = max(0.0f, _doRungDong - _doRungDongGoc);
    }
  }

  Wire.beginTransmission(DIA_CHI_HMC5883L);
  Wire.write(0x03);
  Wire.endTransmission(false);
  Wire.requestFrom(DIA_CHI_HMC5883L, 6, true);
  if (Wire.available() >= 6) {
    Wire.read(); Wire.read();
    Wire.read(); Wire.read();
    Wire.read(); Wire.read();
  }

  Wire.beginTransmission(DIA_CHI_BMP180);
  Wire.write(0xF4); Wire.write(0x2E);
  Wire.endTransmission();
  delay(5);

  Wire.beginTransmission(DIA_CHI_BMP180);
  Wire.write(0xF6);
  Wire.endTransmission(false);
  Wire.requestFrom(DIA_CHI_BMP180, 2, true);
  if (Wire.available() >= 2) {
    _apSuat = Wire.read() << 8 | Wire.read();
  }
}

void GY87_IMU::datGocNghiengGoc(float giaTri) { _gocNghiengGoc = giaTri; }
void GY87_IMU::datRungDongGoc(float giaTri) { _doRungDongGoc = giaTri; }
void GY87_IMU::datDaHieuChuan(bool trangThai) { _daHieuChuan = trangThai; }
