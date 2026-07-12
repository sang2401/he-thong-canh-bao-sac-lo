#include "CoiBaoDong.h"

CoiBaoDong::CoiBaoDong(uint8_t chan)
  : _chan(chan), _thoiGianBipTruoc(0), _trangThaiBip(false) {}

void CoiBaoDong::khoiTaoChan() {
  pinMode(_chan, OUTPUT);
  digitalWrite(_chan, LOW);
}

void CoiBaoDong::capNhat(MucCanhBao muc) {
  if (muc == BAO_DONG_SAT_LO) {
    tone(_chan, 2500);
  } else if (muc == CANH_BAO_CAO) {
    unsigned long thoiGianHienTai = millis();
    if (thoiGianHienTai - _thoiGianBipTruoc >= 500) {
      _thoiGianBipTruoc = thoiGianHienTai;
      _trangThaiBip = !_trangThaiBip;
      if (_trangThaiBip) {
        tone(_chan, 2000, 200);
      } else {
        noTone(_chan);
      }
    }
  } else {
    noTone(_chan);
    _trangThaiBip = false;
  }
}
