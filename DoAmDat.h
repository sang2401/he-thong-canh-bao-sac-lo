#ifndef DO_AM_DAT_H
#define DO_AM_DAT_H

#include <Arduino.h>
#include "CauHinhChan.h"

class DoAmDat {
public:
  explicit DoAmDat(uint8_t chanAnalog);

  void capNhat();
  int getPhanTram() const { return _phanTramDoAm; }
  int getGiaTriTho() const { return _giaTriTho; }

private:
  uint8_t _chanAnalog;
  int _giaTriTho;
  int _phanTramDoAm;
};

#endif
