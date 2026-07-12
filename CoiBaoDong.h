#ifndef COI_BAO_DONG_H
#define COI_BAO_DONG_H

#include <Arduino.h>
#include "CauHinhChan.h"

class CoiBaoDong {
public:
  explicit CoiBaoDong(uint8_t chan);

  void khoiTaoChan();
  void capNhat(MucCanhBao muc);

private:
  uint8_t _chan;
  unsigned long _thoiGianBipTruoc;
  bool _trangThaiBip;
};

#endif
