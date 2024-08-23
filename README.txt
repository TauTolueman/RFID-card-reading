	Espressif firmasının ESP32 C6 DEV-KIT1 kartıyla geliştirdiğimiz bu projede 
Bir adet ESP32 C6 DEV-KIT1 kartı,
Bir adet MFRC522 rfid kart okuyucu,
İki adet LED,
Bir adet buzzer kullanılmıştır.
	
	Projenin amacı MFRC522 rfid kart okuyucumuza okuttuğumuz kartların seri numarasını ve 
gerçek saate göre ne zaman okuttuğumuz bilgisini kaydetmektir.
Gerçek saati ayarlarken, ESP32 kartının WIFI kütüphanesi sayesinde istediğimiz wifi ağına bağlanıp istediğimiz konumun uydu saatini kullanıyoruz.
Kart okuturken çeşitli kurallarımız var. 
Örneğin: 
Okuttuğumuz bir kartı 5 dakika içinde bir daha okutursak o kartın verisi kaydedilmiyor ve kullanıcı uyarılıyor.(İstediğimiz şekilde bu süreyi konfigüre edebiliyoruz.)
Özel olarak tanımladığımız kartı (Beyaz kart seri numarası: 977716643) okuttuğumuzda ise bütün kayıtlı verileri silip sistemi yeniden başlatıyor.
(İstediğimiz şekilde özel kartı konfigüre edebiliyoruz)

Ayrıca buzzer ve LED'lerle özel çıkışları anlamlandırdık.
Her kart okuttuğumuzda buzzer ve LED'lerle kullanıcıya mesaj veriyoruz.


/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////

				     Pin layouts

//////////////////////////////////////MFRC522//////////////////////////////////////

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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////*/



