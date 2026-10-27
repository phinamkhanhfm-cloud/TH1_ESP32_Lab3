/*
 * BAI 3 - VI DU 2: TEST ADC2 TREN CHAN STRAPPING (GPIO12) - DE THAY HAU QUA THAT
 * ---------------------------------------------------------------
 * MUC DICH SU PHAM: cho SV tu tay thay 2 van de khi chon sai chan ADC,
 * khong chi hoc thuoc tren slide ly thuyet.
 *
 * Mach TEST (CHI DUNG DE THU NGHIEM, KHONG dung cho san pham that):
 *   bien tro (chan giua) -> GPIO12 (ADC2_CH5, DONG THOI la chan
 *   strapping MTDI - chon dien ap flash luc boot: keo LOW = 3.3V,
 *   keo HIGH (hoac tha noi voi pull-up manh ben ngoai) = 1.8V).
 *
 * VAN DE 1 - LOI BOOT (quan sat bang mat, khong can code chay):
 *   Neu con truot bien tro dang o vi tri keo GPIO12 len muc HIGH manh
 *   dung luc cap nguon/nhan EN (reset), board co the boot sai dien ap
 *   flash -> khong len Serial, hoac in ra ky tu loi/khong boot duoc.
 *   Hay thu: xoay bien tro ve 2 dau (min/max), nhan nut EN tren board,
 *   quan sat: co luc board KHONG boot binh thuong.
 *   -> KET LUAN: khong bao gio dung chan strapping cho tin hieu ngoai
 *      analog lien tuc (bien tro keo dien ap thay doi ngay luc boot).
 *
 * VAN DE 2 - LOI DOC KHI WI-FI BAT:
 *   ADC2 dung chung phan cung voi Wi-Fi radio. Khi Wi-Fi dang bat
 *   (WiFi.begin()/softAP...), analogRead() tren chan ADC2 co the:
 *     - tra ve gia tri sai/co dinh (vd luon 0 hoac luon 4095), hoac
 *     - ham/treo mot thoi gian ngan.
 *   Code duoi day BAT WiFi (khong can ket noi duoc mang) chi de ep
 *   dieu kien "Wi-Fi dang bat" va cho SV thay analogRead(ADC2) tro nen
 *   khong dang tin, roi so sanh voi vd1 (ADC1, GPIO34) van doc binh
 *   thuong trong cung dieu kien.
 *
 * Cach quan sat truc quan: python plot_serial_vd3.py COM7
 *   (dung chung script voi vd1 - cung dinh dang ">tho:gia_tri")
 */

#include <Arduino.h>
#include <WiFi.h>

#define ADC2_PIN  12   // CANH BAO: GPIO12 = ADC2_CH5 VA la chan strapping (MTDI)

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("=== BAI 3 - VD2: Test ADC2 tren chan strapping (GPIO12) ===");
  Serial.println("CANH BAO: day la vi du DE THAY LOI, khong dung lam mach that.");
  Serial.println("Buoc 1: quan sat boot - thu xoay bien tro + nhan EN nhieu lan.");
  Serial.println("Buoc 2: Wi-Fi se duoc bat sau 3 giay de ep dieu kien doc ADC2 loi.");

  // Chua bat Wi-Fi ngay - cho SV quan sat boot binh thuong truoc (Buoc 1)
}

void loop() {
  static bool daBatWifi = false;
  static unsigned long tBatDau = 0;
  if (tBatDau == 0) tBatDau = millis();

  // Sau 3 giay moi bat Wi-Fi, de thay ro doi chieu truoc/sau khi Wi-Fi bat
  if (!daBatWifi && millis() - tBatDau > 3000) {
    WiFi.mode(WIFI_STA);
    WiFi.begin("khong-can-ket-noi-duoc", "12345678");  // chi can RADIO bat
    daBatWifi = true;
    Serial.println(">>> Da bat Wi-Fi - tu day analogRead(ADC2) co the SAI/HANG <<<");
  }

  int tho = analogRead(ADC2_PIN);   // ADC2 - co the tra ve gia tri khong dang tin

  Serial.print(">tho:");
  Serial.println(tho);
  Serial.print(">wifiBat:");
  Serial.println(daBatWifi ? 1 : 0);

  delay(20);
}
