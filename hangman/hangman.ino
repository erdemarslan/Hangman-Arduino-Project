/*
 * Adam Asmaca
 * Erdem ARSLAN
 */

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <PS2Keyboard.h>
#include <Adafruit_NeoPixel.h>

// Pinler
#define K_DATA 4
#define K_IRQ 3

#define BUZZERPIN 7

#define LED_ADAM 5
#define LED_HARF 6
#define NUM_LED_ADAM 14
#define NUM_LED_HARF 30
#define LED_ADAM_PARLAKLIK 100
#define LED_HARF_PARLAKLIK 255

#define DOGRU_MAX 16
#define HATA_MAX 10

//buzzer için notalar
#define LOWNOTE 100
#define ALOW 440
#define CLOW 261
#define ELOW 329
#define FLOW 349
#define CHIGH 523
#define EHIGH 659
#define GHIGH 784
#define FSHIGH 740
#define AHIGH 880

// I2C LCD Ekran Olacak

// Tüm giriş çıkışlar klavyeden sağlanacak!

// Resetleme fonksiyonu
void(* resetFunc) (void) = 0;             //declare reset function @ address 0

// Veriler ve Değişkenler
/*
 * Harfler ve Karşılıkları
 * ı -> i
 * ğ -> [
 * ü -> ]
 * ş -> ;
 * i -> '
 * ö -> ,
 * ç -> .
 */
/*
 * Her kageoride 10 kelime olacak!
 */
const char* kelimeler[] = {"samanyolu", "galaks'", "andromeda", "'kl'm", "meteorolog", "meteoroloj'", "kl'matolog", "kasirga", "g]ne;", "yildiz",
                           "h]cre", "m'toz b,l]nme", "mayoz b,l]nme", "memel'ler", "kurba[alar", "mantarlar", "m'tekondr'", "sentrozom", "golg' c's'mc'['", "n]kleot'd",
                           "k'net'k enerj'", "h'drojen", "s]rt]nme kuvvet'", "oks'jen", "atom", "molek]l", "saf madde", "kari;imlar", "homojen kari;im", "basin.", "element",
                           "elektr'k", "d'ren.", "akim", "ger'l'm", "d]z ayna", ".ukur ayna", "t]msek ayna", "yansima a.isi", "gelme a.isi", "beyaz renk",
                           "'nce ba[irsak", "s'nd'r'm", "a v'tam'n'", "adrenal'n", "b]y]me hormonu", "n,ron", "bey'nc'k", "yumurtal'k", "]reme s'stem'", "bo;altim",
                           "aslan", "jaguar", "su aygiri", "ser.e", "kirlangi.", "f'lam'ngo", "bal'na", "levrek", "kaplan", "penguen"
};
const byte kategoriler[] = {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5};
//const char kategori_adlari[] = {"Gökyüzü ve Uzay", "Canlilar", "Madde ve Enerji", "Işık Ses Elektrik", "Sistemler" ,"Hayvanlar"};
const String harfler = "abc.defg[hp,onmlkj'irs;tu]vyz";
int kelimeSayisi;
String tahminler = "";
//char tahminEdilenHarf;
char harf;
int dogru = 0;
int yanlis = 0;
bool oyunBasladi = false;
char* kelime;
String metinKelime;
int kelimeUzunlugu;


// adam asmaca grafik karakterleri
byte adam_sol_alt[8] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x1F, 0x1F};
byte adam_sol_ust[8] = {0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10};
byte adam_sag_ust[8] = {0x1C, 0x04, 0x0E, 0x0A, 0x0E, 0x04, 0x0E, 0x15};
byte adam_sag_alt[8] = {0x15, 0x04, 0x0A, 0x0A, 0x0A, 0x1B, 0x00, 0x00};


// Türkçe Karakterler
// ığüşöç
byte h_i[8] = {0x00,0x00,0x0C,0x04,0x04,0x04,0x0E,0x00};
byte h_g[8] = {0x0E,0x00,0x0F,0x11,0x11,0x0F,0x01,0x0E};
byte h_u[8] = {0x0A,0x00,0x11,0x11,0x11,0x13,0x0D,0x00};
byte h_s[8] = {0x00,0x0E,0x10,0x0E,0x01,0x1E,0x04,0x00};
byte h_o[8] = {0x0A,0x00,0x0E,0x11,0x11,0x11,0x0E,0x00};
byte h_c[8] = {0x00,0x00,0x0E,0x10,0x10,0x11,0x0E,0x04};

// Sınıflar
PS2Keyboard klavye;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_NeoPixel ledAdam(NUM_LED_ADAM, LED_ADAM, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ledHarf(NUM_LED_HARF, LED_HARF, NEO_GRB + NEO_KHZ800);

void setup() {
  // Tanımlamalar
  pinMode(BUZZERPIN, OUTPUT);

  // Rasgele modunu aç ve toplam kelimeyi belirle!
  randomSeed(analogRead(0));
  kelimeSayisi = sizeof(kelimeler)/sizeof(char*);
  
  // Klavye
  klavye.begin(K_DATA, K_IRQ, PS2Keymap_US);
  
  // Ledleri başlat!
  ledAdam.begin();
  ledHarf.begin();

  // Parlaklık ayarı yapalım. Duruma göre ayarlayacağız bunları
  ledAdam.setBrightness(LED_ADAM_PARLAKLIK);
  ledHarf.setBrightness(LED_HARF_PARLAKLIK);

  // Başlangıçta rasgele renk atayalım ledlere ki görsel olsun!
  LEDBaslangic();
  
  // Ekran
  lcd.init();
  lcd.backlight();
  anaEkran();

  // Seri Monitör
  Serial.begin(9600);
  Serial.println("Adam asmaca oyunu için program başlatıldı.");
}


// [TAB] a basılırsa oyunda bir kelime ver ve adamı biraz daha as
void loop() {
  if (klavye.available()) {
    harf = klavye.read();
    
    Serial.println(harf);

    // Oyun Başladı mı?
    
    if(!oyunBasladi) {
      // Oyun daha başlamamış! Oyunun başlamasını bekle!
      // Oyun başlamamışsa ENTER tuşuna basılmasını bekle
      if(harf == PS2_ENTER) {
               
        Serial.println("Oyun için ayarlamalar yapılıyor...");
        // Oyunu Başlat
        oyunuBaslat();
        Serial.println("Oyun başladı...");
      }
      // Enter dışında bir tuşa basılmışsa birşey yapma!
    } else {
      // Oyun başlamışsa burası çalışacak!

      // [ESC] tuşuna basılırsa, oyundan çık ve resetle
      if (harf == PS2_ESC) {
        // Oyunu resetleme ile bitir!
        oyunBitti(2);
        
      } else {
        // gelen harfi kontrol et! Varsa ekranda göster, yoksa hata ver!
        harfiKontrolEt(harf);

        Serial.print("Doğru: ");
        Serial.println(dogru);
        Serial.print("Yanlış: ");
        Serial.println(yanlis);
        
        // harf kontrolleri tamam
        // oyun bitiyor mu onu da kontrol edelim

        // önce hatalarla bitiyor mu ona bakalım!
        if(yanlis >= 10) {
          // ekrana yazı basalım ve oyunu da bitirelim
          oyunBitti(0);
        }

        if(dogru >= kelimeUzunlugu) {
          oyunBitti(1);
        }
      }
    }
  }
}
