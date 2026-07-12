#include "DoAmDat.h"

DoAmDat::DoAmDat(uint8_t chanAnalog)
  : _chanAnalog(chanAnalog), _giaTriTho(0), _phanTramDoAm(0) {}

void DoAmDat::capNhat() {
  _giaTriTho = analogRead(_chanAnalog);
  _phanTramDoAm = map(_giaTriTho, 0, 1023, 100, 0);
  _phanTramDoAm = constrain(_phanTramDoAm, 0, 100);
}
