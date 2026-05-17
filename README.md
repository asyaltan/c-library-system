# 📚 Kütüphane Envanter Yönetim Sistemi


## 🛠️ Arka Plandaki Teknik Yapı ve Mantık

Sistem mimarisi kurulurken veri bütünlüğü, bellek optimizasyonu ve temel yazılım güvenliği için şu teknik çözümler uygulanmıştır:

### 1. Dinamik Bağlı Listeler (Linked List)
Kitaplar, kullanıcılar ve ödünç kayıtları için sabit boyutlu diziler (array) yerine RAM üzerinde **tek yönlü bağlı listeler** kurulmuştur. `malloc` ve `free` fonksiyonları sayesinde bellek dinamik olarak yönetilir. Yeni veri geldikçe liste doğrusal olarak genişler, veri silindiğinde ise ilgili düğüm (`node`) RAM'den tamamen temizlenerek bellek sisteme iade edilir.

### 2. XOR Kriptografi (Veri Güvenliği)
Kullanıcı şifrelerinin veri tabanında düz metin olarak saklanmasını engellemek için bir kriptografi mekanizması entegre edilmiştir. Belirlenen `0x5A` anahtarı (maske) ile şifreler bit düzeyinde **XOR (`^`) işlemine** sokulur. XOR algoritmasının simetrik yapısı sayesinde, dosyaya yazılırken şifrelenen veriler, program açılıp dosyadan okunurken aynı fonksiyonun tekrar çalıştırılmasıyla orijinal haline geri döndürülür.

### 3. Girdi Güvenliği (Secure Coding)
Klasik `scanf` fonksiyonunun boşluk karakterlerinde kırılması ve arabellek taşması (**Buffer Overflow**) yaratma riskine karşı tüm metin girdileri `fgets` ile sınırlandırılarak alınır. Satır sonlarına yapışan Enter (`\n`) karakterleri `strcspn` ile temizlenir. Menüler arasında geçiş yaparken komutların kendi kendine atlamasını önlemek için klavye belleğinde kalan çöpler `tamponTemizle` fonksiyonuyla her adımda yutturulur.

### 4. Zaman Simülasyonu (`time.h`)
Kitap ödünç alma ve iade işlemlerindeki teslim süresi takibi ve ceza hesaplamaları için UNIX zaman damgası (`timestamp`) kullanılmıştır. Sistem canlı test edilirken günlerce beklemeyi önlemek adına **geçen her 1 saniye, sistemde 1 gün olarak simüle edilir**. 2 günlük ödünç alınan bir kitap 4 saniye sonra iade edilirse sistem otomatik olarak 2 gün gecikme ve ceza hesaplar.

---

## 📦 Sistem Nasıl Çalışıyor? (İş Paketleri)

### 👤 Kullanıcı Girişi ve Yetkilendirme (İP-1)
Sisteme giriş yapan kullanıcının rolüne (Yönetici, Personel, Öğrenci) göre ana menü ve fonksiyonlar otomatik olarak kısıtlanır. Öğrenci rolündeki bir hesap, kitap silme veya kullanıcı hesabı yönetme ekranlarına erişemez; bu koruma hem arayüzde hem de fonksiyonların kendi içinde çift katmanlı olarak kontrol edilir.

### 📖 Kitap Envanter İşlemleri (İP-2)
Kitapların ISBN, ad, yazar, yayınevi, yayın yılı, stok sayısı ve raf konumu bilgileriyle tam entegre çalışır. Güncelleme menüsünden stok ve konumlar dinamik olarak değiştirilebilir. Silme işleminde ise aranan düğüm listeden kopartılarak bellekten temizlenir.

### 👥 Kullanıcı Hesabı Yönetimi (İP-3)
Yalnızca Yöneticinin (Admin) erişebildiği bu panel üzerinden sisteme dinamik olarak yeni Personel veya Öğrenci hesapları eklenebilir, mevcut hesapların şifre/rol bilgileri güncellenebilir ya da sistemden tamamen silinebilir.

### ⏱️ Süreli Ödünç Alma ve İade Süreçleri (İP-4)
Öğrenciler giriş yaptıktan sonra istedikleri kitabı kendi adlarına tescil ettirebilirler. Veri bütünlüğünü korumak adına, bir öğrenci iade etmediği aktif bir kitap varken aynı kitabı tekrar ödünç alamaz. İade sırasında simüle edilen gün sınırı aşıldıysa sistem ekrana gecikme gününü ve ceza uyarısını basar.

### 🔍 Çok Kriterli Filtreleme ve Arama (İP-5)
Çok kriterli filtreleme isterine uygun olarak; kullanıcının girdiği tek bir kelime üzerinden `strstr` fonksiyonu yardımıyla hem **Kitap Adı**, hem **Yazar**, hem de **ISBN** alanları aynı anda taranır. Eşleşen kitapların tüm künyesi detaylı sonuç ekranı olarak listelenir.

### 📊 Sistem Raporları ve Analiz (İP-6)
Yetkili rollerin görebildiği bu ekranda şu 3 kritik istatistik anlık üretilir:
* **Envanter Özeti:** Raflardaki toplam farklı kitap çeşidi sayısı ve toplam stok miktarı.
* **Popüler Kitaplar:** Sistemde en çok ödünç alınan ve stoğu kritik seviyenin (3'ün altına) altına düşen, yüksek talep gören kitaplar.
* **Gecikmiş İadeler:** Kitabı teslim etme süresi geçmiş aktif borçlu kullanıcıların adı, kitap ISBN'i ve kaç gün geciktirdikleri dinamik olarak listelenir.

---

## 💾 Kalıcı Hafıza Yapısı (Metin Dosyaları)

Sistemden çıkış yapıldığı an RAM'deki tüm dinamik bağlı listeler taranarak veriler 3 farklı metin dosyasına kalıcı olarak işlenir ve sonraki açılışta otomatik geri yüklenir:
* `kutuphane.txt` -> Güncel kitap envanteri, stoklar, raf konumları ve ödünç sayacı.
* `kullanicilar.txt` -> XOR ile şifrelenmiş dinamik kullanıcı hesapları ve rol tanımları.
* `odunc.txt` -> Kimin hangi kitabı ne zaman aldığına dair UNIX zaman damgalı ödünç logları.

---

## 🚀 Derleme ve Çalıştırma

Kodu standart bir C derleyicisi (GCC) ile derleyip çalıştırabilirsiniz:

```bash
gcc main.c -o kütüphane_sistemi
./kütüphane_sistemi
