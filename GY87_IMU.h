#ifndef GY87_IMU_H
#define GY87_IMU_H

#include <Arduino.h>
#include <Wire.h>
#include "CauHinhChan.h"

class GY87_IMU {
public:
  GY87_IMU();

  bool begin();
  void capNhat(bool truGoc = false);
  bool docMauHieuChuan(float &gocNghieng, float &bienDoRung);
  void resetBoLoc();

  bool daKetNoi() const { return _daKetNoi; }
  bool daHieuChuan() const { return _daHieuChuan; }
  float getGocNghieng() const { return _tongGocNghieng; }
  float getRungDong() const { return _doRungDong; }
  float getGocNghiengGoc() const { return _gocNghiengGoc; }
  float getRungDongGoc() const { return _doRungDongGoc; }
  void datGocNghiengGoc(float giaTri);
  void datRungDongGoc(float giaTri);
  void datDaHieuChuan(bool trangThai);

private:
  bool _daKetNoi;
  bool _daHieuChuan;
  float _tongGocNghieng;
  float _doRungDong;
  float _gocNghiengGoc;
  float _doRungDongGoc;
  float _gocX;
  float _gocY;
  unsigned long _thoiGianDocTruoc;
  float _nhietDo;
  float _apSuat;
};

#endif
