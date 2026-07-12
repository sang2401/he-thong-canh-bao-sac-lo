#ifndef CAU_HINH_CHAN_H
#define CAU_HINH_CHAN_H

#include <Arduino.h>

// --- Chân kết nối ---
#define DO_AM_DAT_AO     A0
#define HX711_DT         6
#define HX711_SCK        7
#define COI_BAO_DONG     13
#define LED_XANH_LA      11
#define LED_XANH_DUONG   12
#define LED_DO           10

// --- Địa chỉ I2C GY-87 ---
#define DIA_CHI_MPU6050   0x68
#define DIA_CHI_HMC5883L  0x1E
#define DIA_CHI_BMP180    0x77
#define DIA_CHI_LCD       0x27

// --- Cân HX711 ---
const float NGUONG_KHOI_DONG_VE_0   = 0.2f;
const float HE_SO_HIEU_CHUAN_CAN    = -7050.0f;

// --- MPU6050 ---
const float TI_LE_GIA_TOC_MPU6050   = 16384.0f;
const float GIA_TOC_TRONG_TRUONG    = 9.81f;
const float HE_SO_LOC_ALPHA         = 0.98f;
const float TI_LE_VAN_TOC_GOC       = 131.0f;
const int   SO_MAU_HIEU_CHUAN       = 10;

// --- Ngưỡng cảnh báo ---
const int   DO_AM_BINH_THUONG       = 80;
const int   DO_AM_CAO               = 90;
const float NGUONG_NGHIENG_BINH_THUONG = 10.0f;
const float NGUONG_NGHIENG_CANH_BAO    = 10.0f;
const float NGUONG_NGHIENG_CAO         = 15.0f;
const float NGUONG_NGHIENG_SAT_LO      = 20.0f;
const float RUNG_DONG_MANH          = 2.0f;
const float TRONG_LUONG_BINH_THUONG   = 3.0f;
const float TRONG_LUONG_CANH_BAO      = 2.0f;
const float TRONG_LUONG_CAO           = 1.0f;
const float TRONG_LUONG_SAT_LO        = 0.5f;

// --- Định thời ---
const unsigned long CHU_KY_CAP_NHAT   = 3000;
const unsigned long THOI_GIAN_HIEN_GOC = 20000;

enum MucCanhBao {
  BINH_THUONG,
  CANH_BAO_NHE,
  CANH_BAO_CAO,
  BAO_DONG_SAT_LO
};

#endif
