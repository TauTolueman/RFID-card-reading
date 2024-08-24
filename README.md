# 🚀 ESP32 C6 RFID Kart Okuyucu Projesi

Bu proje, ESP32 C6 DEV-KIT1 kartı kullanılarak MFRC522 RFID kart okuyucu, LED'ler ve bir buzzer ile kartların seri numaralarını ve okutulma zamanlarını kaydetmek amacıyla geliştirilmiştir.

## 📦 Proje Bileşenleri

- **ESP32 C6 DEV-KIT1**
- **MFRC522 RFID Kart Okuyucu**
- **2 adet LED**
- **1 adet Buzzer**

## 🎯 Proje Amacı

Bu proje, MFRC522 RFID kart okuyucusu ile okutulan kartların seri numaralarını ve gerçek saat bilgilerini kaydetmek için tasarlanmıştır. Gerçek saat bilgisi, ESP32'nin Wi-Fi bağlantısı üzerinden konumun uydu saatine erişilerek ayarlanmaktadır.

### 🔒 Kart Okuma Kuralları

- Bir kart 5 dakika içerisinde tekrar okutulursa, bu kartın verisi kaydedilmez ve kullanıcı uyarılır. (Bu süre konfigüre edilebilir.)
- 🛡️ Özel olarak tanımlanan bir kart (Beyaz kart seri numarası: 977716643) okutulduğunda, bütün kayıtlı veriler silinir ve sistem yeniden başlatılır. (Özel kart tanımlaması konfigüre edilebilir.)

### 💡 Buzzer ve LED Kullanımı

Kart okutulduğunda, kullanıcıya LED'ler ve buzzer ile görsel ve işitsel geri bildirimler sağlanır.



  ![Screensho](https://github.com/user-attachments/assets/938a5af7-d2cf-4ad5-aeaf-e53c35e3b4b9)

## 🛠️ Pin Düzeni

	MFRC522                ESP32C6             LED's and BUZZER

	SDA                    GPIO_5              //////////////////
	SCK                    GPIO_18		   //////////////////
	MOSI                   GPIO_23		   //////////////////
	MISO                   GPIO_19		   //////////////////
	IRQ                    NOT USED		   //////////////////
	GND                    GND		   //////////////////
	RST                    GPIO_22		   //////////////////
	3V3                    3V3		   //////////////////
	///////		       GPIO_2              RED LED
	///////		       GPIO_3              GREEN LED
	///////		       GPIO_15             BUZZER


## 🧩 Kurulum

1. **ESP-IDF Kurulumu:** ESP32 C6 için ESP-IDF ortamını kurun.
2. **Kodun Yüklenmesi:** Proje dosyalarını ESP32 C6 kartına yükleyin.
3. **Wi-Fi Ayarları:** Projedeki Wi-Fi ayarlarını kendi ağınıza göre yapılandırın.
4. **Çalıştırma:** Kartı çalıştırın ve RFID kartları okutun.

## ⚙️ Kullanım

- Projeyi başlattıktan sonra, RFID kartlarınızı MFRC522 okuyucuya okutabilirsiniz.
- 💡 LED'ler ve 🔊 buzzer, başarılı kart okuma işlemlerinde size geri bildirim sağlayacaktır.
- 🛑 Özel kart (seri numarası: 977716643) okutulduğunda, tüm kayıtlar silinecek ve sistem yeniden başlatılacaktır.


## 🎦 YouTube video linki
https://youtu.be/3Y2tAWSvxYA?feature=shared
