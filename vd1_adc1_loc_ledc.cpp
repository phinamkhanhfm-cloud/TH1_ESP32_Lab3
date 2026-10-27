/*
 * BAI 3 - VI DU 1: DOC BIEN TRO QUA ADC1 + LOC TRUNG BINH TRUOT + LEDC
 * ---------------------------------------------------------------
 * Mach: bien tro (chan giua) -> GPIO34 (ADC1_CH6, input-only, AN TOAN
 * de dung ADC ca khi Wi-Fi bat) * LED -> GPIO18 (dieu khien bang LEDC).
 *
 * Vi sao chon GPIO34 (ADC1) thay vi mot chan ADC2:
 *   - ADC1 (GPIO32-39) doc on dinh bat ke Wi-Fi dang bat hay tat.
 *   - ADC2 (GPIO0,2,4,12-15,25-27) KHONG doc duoc khi Wi-Fi dang bat,
 *     va nhieu chan ADC2 lai trung voi chan strapping (vd GPIO12, GPIO15)
 *     -> xem vi du 2 (vd2_adc2_strapping.cpp) de thay hau qua that.
 *
 * Loc trung binh truot (moving average), cua so N=8 mau:
 *   - Gia tri ADC tho tu bien tro luon bi nhieu (dao dong vai don vi
 *     ngay ca khi khong dong vao bien tro) do nhieu dien tu/ADC noi.
 *   - Luu N mau gan nhat trong mang vong (circular buffer), tra ve
 *     trung binh cong -> lam muot tin hieu ma van bam sat xu huong that.
 *
 * LEDC: API PWM phan cung ESP32 (khac analogWrite() cua Arduino).
 *   ledcAttach(chan, tan_so_Hz, so_bit_phan_giai) roi ledcWrite(chan, duty).
 *
 * Cach quan sat truc quan (giong vi du bai 2):
 *   python plot_serial_vd3.py COM7
 *   -> 2 so lon (tho / loc) + do thi 2 duong theo thoi gian.
 */

#include <Arduino.h>

#define ADC_PIN   34   // Bien tro - ADC1_CH6, input-only, an toan voi Wi-Fi
#define LED_PIN   18   // LED - dieu khien qua LEDC
#define LEDC_FREQ 5000
#define LEDC_RES  8    // 8-bit -> duty 0-255
#define N         8    // So mau dung de loc trung binh truot

int buf[N];
int idx = 0;
bool bufDaDay = false;

int locTrungBinhTruot(int mauMoi) {
  buf[idx] = mauMoi;
  idx = (idx + 1) % N;
  if (idx == 0) bufDaDay = true;

  int soMauDung = bufDaDay ? N : idx;
  long tong = 0;
  for (int i = 0; i < soMauDung; i++) tong += buf[i];
  return tong / soMauDung;
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("=== BAI 3 - VD1: ADC1 (GPIO34) + loc trung binh truot + LEDC ===");
  Serial.println("Quay bien tro tu min den max, quan sat tho vs loc.");

  ledcAttach(LED_PIN, LEDC_FREQ, LEDC_RES);
}

void loop() {
  int tho = analogRead(ADC_PIN);      // 0-4095 (12-bit)
  int loc = locTrungBinhTruot(tho);

  int duty = map(loc, 0, 4095, 0, 255);
  ledcWrite(LED_PIN, duty);

  // Dinh dang Teleplot: moi dong ">ten:gia_tri" ve 1 duong do thi rieng
  Serial.print(">tho:");
  Serial.println(tho);
  Serial.print(">loc:");
  Serial.println(loc);

  delay(20);   // ~50Hz lay mau, du nhanh de thay muot tren do thi
}
