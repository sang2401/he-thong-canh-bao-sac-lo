#include <Wire.h>
#include "CauHinhChan.h"
#include "CanHX711.h"
#include "DoAmDat.h"
#include "GY87_IMU.h"
#include "HeThongCanhBao.h"
#include "ManHinhLCD.h"
#include "CoiBaoDong.h"

// --- Đối tượng hệ thống ---
CanHX711       can(HX711_DT, HX711_SCK, HE_SO_HIEU_CHUAN_CAN);
DoAmDat        doAm(DO_AM_DAT_AO);
GY87_IMU       imu;
HeThongCanhBao canhBao(LED_XANH_LA, LED_XANH_DUONG, LED_DO);
ManHinhLCD     lcd(DIA_CHI_LCD, 16, 2);
CoiBaoDong     coi(COI_BAO_DONG);

// --- Trạng thái hệ thống ---
unsigned long thoiGianCapNhatTruoc = 0;
unsigned long thoiGianBatDauHienGoc = 0;
bool daHieuChuanGoc = false;

void hieuChuanGiaTriGoc();
void inThongTinQuaSerial(MucCanhBao muc);

void setup() {
  Serial.begin(9600);
  delay(500);

  Serial.println(F("\n========================================"));
  Serial.println(F("HE THONG SA BAN CANH BAO SAT LO TU DONG"));
  Serial.println(F("========================================"));

  canhBao.khoiTaoChan();
  coi.khoiTaoChan();
  pinMode(DO_AM_DAT_AO, INPUT);

  Wire.begin();
  Wire.setClock(100000);
  delay(100);

  lcd.begin();
  lcd.hienThiThongBao(F("Dang khoi dong..."));
  delay(500);
  lcd.clear();

  imu.begin();

  if (can.begin()) {
    lcd.hienThiThongBao(F("Dang lay ve 0..."));
    delay(500);
    lcd.clear();
    lcd.hienThiThongBao(F("HE THONG SAN SANG"));
    delay(1000);
    lcd.clear();
  } else {
    lcd.hienThiThongBao(F("LOI: CAN HX711"), F("Khong ket noi  "));
  }

  if (imu.daKetNoi() || can.sanSang()) {
    hieuChuanGiaTriGoc();
  }

  thoiGianCapNhatTruoc = millis();
}

void loop() {
  if (Serial.available() > 0) {
    char lenh = Serial.read();
    if (lenh == 't' || lenh == 'T') {
      lcd.hienThiThongBao(F("Dang tare lai..."));
      can.tare(20);
      delay(500);
      lcd.clear();
    }
  }

  unsigned long thoiGianHienTai = millis();
  if (thoiGianHienTai - thoiGianCapNhatTruoc >= CHU_KY_CAP_NHAT) {
    thoiGianCapNhatTruoc = thoiGianHienTai;

    can.capNhat(daHieuChuanGoc);
    doAm.capNhat();
    imu.capNhat(daHieuChuanGoc);

    MucCanhBao muc = canhBao.danhGia(
      imu.getGocNghieng(),
      imu.getRungDong(),
      doAm.getPhanTram(),
      can.getTrongLuong()
    );
    canhBao.capNhatLED(muc);

    bool hienGoc = daHieuChuanGoc &&
      (thoiGianHienTai - thoiGianBatDauHienGoc < THOI_GIAN_HIEN_GOC);

    lcd.hienThiDuLieu(
      hienGoc, thoiGianBatDauHienGoc,
      imu.daKetNoi(), can.sanSang(),
      imu.getGocNghiengGoc(), imu.getRungDongGoc(), can.getTrongLuongGoc(),
      doAm.getPhanTram(), imu.getGocNghieng(), imu.getRungDong(), can.getTrongLuongTho()
    );

    inThongTinQuaSerial(muc);
  }

  coi.capNhat(canhBao.getMucHienTai());
  delay(10);
}

void hieuChuanGiaTriGoc() {
  float tongNghiengKhaoSat = 0, tongRungKhaoSat = 0, tongLucKhaoSat = 0;
  int mauNghiengHopLe = 0, mauRungHopLe = 0, mauLucHopLe = 0;

  imu.resetBoLoc();
  daHieuChuanGoc = false;
  lcd.hienThiThongBao(F("Dang lay mau... "));

  for (int i = 0; i < SO_MAU_HIEU_CHUAN; i++) {
    float gocMau = 0, rungMau = 0;
    if (imu.docMauHieuChuan(gocMau, rungMau)) {
      tongNghiengKhaoSat += gocMau;
      tongRungKhaoSat += rungMau;
      mauNghiengHopLe++;
      mauRungHopLe++;
    }

    float lucMau = 0;
    if (can.docMauHieuChuan(lucMau)) {
      tongLucKhaoSat += lucMau;
      mauLucHopLe++;
    }

    lcd.hienThiTienTrinhHieuChuan(i + 1, SO_MAU_HIEU_CHUAN);
    delay(200);
  }

  if (imu.daKetNoi() && mauNghiengHopLe > 0 && mauRungHopLe > 0) {
    imu.datGocNghiengGoc(tongNghiengKhaoSat / mauNghiengHopLe);
    imu.datRungDongGoc(tongRungKhaoSat / mauRungHopLe);
    imu.datDaHieuChuan(true);
    daHieuChuanGoc = true;
  }

  if (can.sanSang() && mauLucHopLe > 0) {
    can.datTrongLuongGoc(tongLucKhaoSat / mauLucHopLe);
    can.datDaHieuChuan(true);
    daHieuChuanGoc = true;
  }

  if (daHieuChuanGoc) {
    thoiGianBatDauHienGoc = millis();
  }
  lcd.clear();
}

void inThongTinQuaSerial(MucCanhBao muc) {
  Serial.println(F("----------------------------------------"));
  if (daHieuChuanGoc) {
    Serial.println(F("--- GIA TRI GOC BAN DAU ---"));
    if (imu.daKetNoi()) {
      Serial.print(F("  Goc nghieng goc: "));
      Serial.print(imu.getGocNghiengGoc(), 2);
      Serial.println(F(" do"));
      Serial.print(F("  Rung dong goc: "));
      Serial.print(imu.getRungDongGoc(), 3);
      Serial.println(F(" m/s2"));
    }
    if (can.sanSang()) {
      Serial.print(F("  Trong luong goc: "));
      Serial.print(can.getTrongLuongGoc(), 2);
      Serial.println(F(" kg"));
    }
  }

  float trongLuongTho = can.getTrongLuongTho();
  Serial.print(F("Trong luong tho: "));
  Serial.print((trongLuongTho < 0) ? -trongLuongTho : trongLuongTho, 3);
  Serial.println(F(" kg"));
  Serial.print(F("Trong luong hien thi: "));
  Serial.print(can.getTrongLuong(), 2);
  Serial.println(F(" kg"));
  Serial.print(F("Do am dat: "));
  Serial.print(doAm.getPhanTram());
  Serial.println(F("%"));

  Serial.print(F("Trang thai sa ban: "));
  switch (muc) {
    case BINH_THUONG:     Serial.println(F("An toan")); break;
    case CANH_BAO_NHE:    Serial.println(F("Canh bao thap")); break;
    case CANH_BAO_CAO:    Serial.println(F("CANH BAO CAO!")); break;
    case BAO_DONG_SAT_LO: Serial.println(F("NGUY HIEM SAT LO!!!")); break;
  }
  Serial.println(F("----------------------------------------"));
}
