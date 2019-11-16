
// Başlangıç ekranını basar
void anaEkran() {
  // başlangıç için özel karakterleri tanımla!
  lcd.createChar(0, adam_sol_ust);
  lcd.createChar(1, adam_sag_ust);
  lcd.createChar(2, adam_sol_alt);
  lcd.createChar(3, adam_sag_alt);
  lcd.createChar(4, h_i);
  
  // Hadi asılmış adam çizelim
  lcd.clear();
  lcd.home();
  lcd.write(0);
  lcd.write(1);
  lcd.print(" ADAM ASMACA");
  lcd.setCursor(0,1);
  lcd.write(2);
  lcd.write(3);
  lcd.print(" ENTER'a bas");
  lcd.write(4);
  lcd.print("n");
}

// Oyunu başlatır
void oyunuBaslat() {
  // Kelime Seç!
  int secilenKelimeNo = random(kelimeSayisi);
  kelime = kelimeler[secilenKelimeNo];
  metinKelime = String(kelime);
  kelimeUzunlugu = strlen(kelime);

  // Oyunu başlat!
  klavyeyiAyarla();

  // kategoriyi bul ve ekrana yazdır!
  kategoriBulveEkranaYaz(secilenKelimeNo, kelimeUzunlugu);
  
  
  lcd.clear();
  lcd.home();
  lcd.print("  Harf se");
  lcd.write(5);
  lcd.print("iniz");
  
  // DEBUG
  Serial.print("Kelime: ");
  Serial.println(kelime);
  Serial.print("Kelime Uzunluğu: ");
  Serial.println(kelimeUzunlugu);

  //Kelimeyi Ekrana Yazdır
  lcd.setCursor(0,1);
  for(int i = 0; i < kelimeUzunlugu; i++) {
    lcd.print("_");
  }

  // Boşluklar varsa kaldır ve boşluk sayısı kadar doğru arttır!
  kelimedekiBosluklariKaldir(0);

  // Ledleri sıfırlayalım
  LEDSifirla();

  // Oyunun başladığını bildir!
  oyunBasladi = true;
}

void kategoriBulveEkranaYaz(int sira, int uzunluk) {
  int kategori = kategoriler[sira];
  Serial.print("Kategori No: ");
  Serial.println(kategori);

  lcd.clear();
  lcd.home();
  //const char kategori_adlari[] = {"Gökyüzü ve Uzay", "Canlilar", "Madde ve Enerji", "Işık Ses Elektrik", "Sistemler" ,"Hayvanlar"};
  lcd.print("Toplam ");
  lcd.print(String(uzunluk));
  lcd.print(" Harf");
  lcd.setCursor(0,1);
  switch(kategori) {
    case 0:
      // ığüşöç
      lcd.print("G");
      lcd.write(4);
      lcd.print("ky");
      lcd.write(2);
      lcd.print("z");
      lcd.write(2);
      lcd.print(" ve Uzay");
    break;

    case 1:
      // ığüşöç
      lcd.print("Canl");
      lcd.write(0);
      lcd.print("lar ve Hayat");
    break;

    case 2:
      // ığüşöç
      lcd.print("Madde ve Enerji");
    break;

    case 3:
      // ığüşöç
      lcd.print("I");
      lcd.write(3);
      lcd.write(0);
      lcd.print("kSes Elektrik");
    break;

    case 4:
      // ığüşöç
      lcd.print("Sistemler");
    break;

    case 5:
      lcd.print("Hayvanlar");
    break;
  }

  delay(2000);
}

// Kelimede yer alan boşlukları oyundan çıkarır
void kelimedekiBosluklariKaldir(int pozisyon) {

  int pos = metinKelime.indexOf(" ", pozisyon);
  int sonPos = metinKelime.lastIndexOf(" ");

  if (pos != -1) {
    if (pos == sonPos) {
      // sorun yok! Boşluktan bir tane varmış! O yüzden göster gitsin!
      lcd.setCursor(pos, 1);
      lcd.print(" ");
      // boşluğu da karakterden sayacağı için oyunun bitebilmesi için boşluk kadar doğru cevap işaretlesin.
      dogru += 1;
      // doğru bilinenleri say!
    } else {
      // önce şu andakini bir çevirelim.
      lcd.setCursor(pos, 1);
      lcd.print(" ");
      // boşluğu da karakterden sayacağı için oyunun bitebilmesi için boşluk kadar doğru cevap işaretlesin.
      //dogru += 1;
      // Şimdi diğer boşlukları aramaya devam edelim
      kelimedekiBosluklariKaldir(pos+1);
    }
  }
}

// Klavyedeki Türkçe karakterleri ayarlar
void klavyeyiAyarla() {
  // Ana menüdeki karakterler silinecek!
  // ığüşöç
  lcd.createChar(0, h_i);
  lcd.createChar(1, h_g);
  lcd.createChar(2, h_u);
  lcd.createChar(3, h_s);
  lcd.createChar(4, h_o);
  lcd.createChar(5, h_c);
}

void adamAnimasyon(bool ledYesil) {
  int sira = 0;
  int k,y;
  if (ledYesil) {
    k = 0;
    y = 255;
  } else {
    k = 255;
    y = 0;
  }
  
  while(sira < 8) {
    for(int a = 0; a < NUM_LED_ADAM; a++) {
      ledAdam.setPixelColor(a, ledAdam.Color(0, 0, 0));
      ledAdam.show();
    }
    delay(250);

    for(int a = 0; a < NUM_LED_ADAM; a++) {
      ledAdam.setPixelColor(a, ledAdam.Color(k, y, 0));
      ledAdam.show();
    }
    delay(250);
    sira++;
  }
  
}

// Oyunu bitirir
void oyunBitti(byte neYapayim) {
  switch(neYapayim) {
    case 0: // Oyun KAYBEDİLDİ
      // önce adamın son ledini yakalım!
      // sonra ekrana yazı basalım
      lcd.clear();
      lcd.home();
      lcd.print("     Oyunu");
      // kaybettiniz
      lcd.setCursor(0,1);
      lcd.print("  Kaybettiniz!");

      // müzik çalalım
      buzz(ELOW, 500);
      buzz(CLOW, 1000);

      // adama animasyon verelim
      adamAnimasyon(false);
      break;

    case 1: // Oyun KAZANILDI
      // son harf de ekranda gözüksün diye azcik bekle
      delay(800);
      // Tebrikler Kazandınız
      lcd.clear();
      lcd.home();
      lcd.print("    Tebrikler");
      // kaybettiniz
      lcd.setCursor(0,1);
      lcd.print("   Kazand");
      lcd.write(0);
      lcd.print("n");
      lcd.write(0);
      lcd.print("z!");

      // müzik çalalım
      buzz(ALOW, 150);
      buzz(CHIGH, 150);
      buzz(EHIGH, 150);
      buzz(AHIGH, 150);
      delay(150);
      buzz(GHIGH, 150);
      buzz(AHIGH, 500);
      
      // adama animasyon verelim
      adamAnimasyon(true);
      break;

    case 2: // OYUN RESETLENİYOR!
      // oyun sıfırlanıyor
      lcd.clear();
      lcd.home();
      lcd.print("Oyun yeniden");
      lcd.setCursor(0,1);
      lcd.print("ba");
      lcd.write(3);
      lcd.print("lat");
      lcd.write(0);
      lcd.print("l");
      lcd.write(0);
      lcd.print("yor...");
      delay(1000);
      break;
  }
  resetFunc(); // Arduinoyu resetle...
}

// Oyun içinde girilen harfleri kontrol eder. kelimenin içinde yer alıp almadıklarına bakar.
void harfiKontrolEt(char harf) {
  // Önce gelen bir harf mi ona bakmak lazım. Sayı noktalama işareti vb girilirse tepki vermemek lazım.
    if(harfler.indexOf(harf) == -1) {
      // giriş yapılabilecek karakter gelmemiş, döngüden çık!
      return;
    }
    // gelen harf daha önce kullanılmış mı ona bakmak lazım. kullanılmışsa geçmek lazım
    if(tahminler.indexOf(harf) != -1) {
      // harf daha önce kullanılmış, döngüden çıkalım!
      return;
    }

    // harfi tahmin listesine ekle!
    tahminler += String(harf);
    //Serial.println(tahminler);

    // harf kelime içinde var mı?
    int pos = metinKelime.indexOf(harf);

    // kelimenin içinde harf bulunamadı. Hataya ekleme yap ve buradan çık!
    if(pos == -1) {
      // hatayı arttıralım!
      yanlis += 1;
      // Kullanılan harfin ledini yak
      LEDHarfYak(harfler.indexOf(harf), false);
      // Yanlış olduğundan adamı as
      adamiAsalim();
    } else {
      // Ledi Yakalım
      LEDHarfYak(harfler.indexOf(harf), true);
      //Ekrandaki bulunan tüm harfleri göster!
      // Doğru sayısı fonksiyonun içinde belirlenecek!
      bulunanHarfleriGoster(harf, 0);
    }
}

void bulunanHarfleriGoster(char harf, int pozisyon) {
  int poz = metinKelime.indexOf(harf, pozisyon);
  int sonPoz = metinKelime.lastIndexOf(harf);

  Serial.print("İlk Pozisyon: ");
  Serial.println(poz);
  Serial.print("Son Pozisyon: ");
  Serial.println(sonPoz);

  if (poz == sonPoz) {
    // tek bir harf bulduk. Sorun yok. Ekranda gösterelim, doğru sayısını da bir arttıralım.
    // Doğru sayısını arttır.
    dogru += 1;
    // harfi lcd ekrana basalım!
    harfiEkranaBas(harf, poz);
  } else {
    // önce ilk harfi ekrana basalım! Sonra diğerlerine bakarız.
    // doğruyu bir arttıralım
    dogru += 1;
    harfiEkranaBas(harf, poz);

    // diğer harfleri de arayalım
    bulunanHarfleriGoster(harf, poz+1);
  }
}

// adamı asar
void adamiAsalim() {
  
  switch(yanlis) {
    case 1:
      ledAdam.setPixelColor(0, ledAdam.Color(255,0,0));
      ledAdam.setPixelColor(1, ledAdam.Color(255,0,0));
      break;

    case 2:
      ledAdam.setPixelColor(2, ledAdam.Color(255,0,0));
      ledAdam.setPixelColor(3, ledAdam.Color(255,0,0));
      ledAdam.setPixelColor(4, ledAdam.Color(255,0,0));
      break;


    case 3:
      ledAdam.setPixelColor(5, ledAdam.Color(255,0,0));
      ledAdam.setPixelColor(6, ledAdam.Color(255,0,0));
      break;

    case 4:
      ledAdam.setPixelColor(7, ledAdam.Color(255,0,0));
      break;

    case 10:
      ledAdam.setPixelColor(8, ledAdam.Color(255,0,0));
      break;


    case 5:
      ledAdam.setPixelColor(9, ledAdam.Color(255,0,0));
      break;


    case 6:
      ledAdam.setPixelColor(10, ledAdam.Color(255,0,0));
      break;


    case 7:
      ledAdam.setPixelColor(11, ledAdam.Color(255,0,0));
      break;


    case 8:
      ledAdam.setPixelColor(12, ledAdam.Color(255,0,0));
      break;

    case 9:
      ledAdam.setPixelColor(13, ledAdam.Color(255,0,0));
      break;
  }

  ledAdam.show();
}

// Başlangıçta rasgele bir renk belirle
void LEDBaslangic() {
  // Yanık kalan tüm ledleri söndür!
  ledAdam.clear();
  ledHarf.clear();

  // Tüm ledleri beyaz yak!
  for(int a = 0; a < NUM_LED_ADAM; a++) {
    int k = random(255);
    int y = random(255);
    int b = random(255);
    ledAdam.setPixelColor(a, ledAdam.Color(k, y, b));
    ledAdam.show();
  }
  for(int a = 0; a < NUM_LED_HARF; a++) {
    int k = random(255);
    int y = random(255);
    int b = random(255);
    ledHarf.setPixelColor(a, ledHarf.Color(k, y, b));
    ledHarf.show();
  }
}

void LEDSifirla() {
  // Yanık kalan tüm ledleri söndür!
  ledAdam.clear();
  ledHarf.clear();

  // Tüm ledleri beyaz yak!
  for(int a = 0; a < NUM_LED_ADAM; a++) {
    ledAdam.setPixelColor(a, ledAdam.Color(150, 150, 150));
    ledAdam.show();
  }
  for(int a = 0; a < NUM_LED_HARF; a++) {
    ledHarf.setPixelColor(a, ledHarf.Color(150, 150, 150));
    ledHarf.show();
  }
}

// Kullanılan harfin ledini yakar
void LEDHarfYak(int ledNo, bool dogru) {
  // doğruysa yeşil yakalım, değilse kırmızı yak!
  if(dogru) {
    ledHarf.setPixelColor(ledNo, ledHarf.Color(0,255,0));
  } else {
    ledHarf.setPixelColor(ledNo, ledHarf.Color(255,50,50));
  }
  // yaktığın ledi göster.
  ledHarf.show();
}

// Kelimenin harflerini ekrana basar.
void harfiEkranaBas(char c, int pos) {
  lcd.setCursor(pos, 1);
  lcd.print(" ");
  lcd.setCursor(pos, 1);
  if(c == '\'') {
    lcd.print("i");
  }
  else if(c == '[') {
    lcd.write(1);
  }
  else if(c == ']') {
    lcd.write(2);
  }
  else if(c == ';') {
    lcd.write(3);
  }
  else if(c == ',') {
    lcd.write(4);
  }
  else if(c == '.') {
    lcd.write(5);
  }
  else if(c == 'i') {
    lcd.write(0);
  }
  else {
    lcd.print(c);
  }
}

void buzz (int frequencyInHertz, long timeInMilliseconds){  
  //Serial.println(frequencyInHertz);   
    long delayAmount = (long)(1000000/frequencyInHertz);
    long loopTime = (long)((timeInMilliseconds*1000)/(delayAmount*2));
    for (int x=0; x < loopTime; x++){    
      digitalWrite(BUZZERPIN, HIGH);
      delayMicroseconds(delayAmount);
      digitalWrite(BUZZERPIN, LOW);
      delayMicroseconds(delayAmount);
    } 
    delay(20);
}
