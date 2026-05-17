#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define ANAHTAR 0x5A

void sifrele(char *metin) {
    for(int i = 0; metin[i] != '\0'; i++)
        metin[i] ^= ANAHTAR;
}

void tamponTemizle() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

typedef struct Kitap {
    char isbn[20];
    char ad[100];
    char yazar[100];
    char yayinevi[100];
    int yayinYili;
    int stok;
    char konum[20];
    int oduncSayisi;
    struct Kitap *sonraki;
} Kitap;

typedef struct Kullanici {
    char kullaniciAdi[50];
    char sifre[50];
    int rol;
    struct Kullanici *sonraki;
} Kullanici;

typedef struct Odunc {
    char isbn[20];
    char kullaniciAdi[50];
    long int oduncTarihi;
    int sureGun;
    int iadeEdildi;
    struct Odunc *sonraki;
} Odunc;

Kitap *bas = NULL;
Kullanici *kullaniciBas = NULL;
Odunc *oduncBas = NULL;

int aktifRol = 0;
char aktifKullanici[50];

int girisYap() {
    char kullaniciadi[50];
    char sifre[50];

    printf("\n--- KULLANICI GIRISI ---\n");
    printf("Kullanici Adi: ");
    tamponTemizle();
    fgets(kullaniciadi, 50, stdin);
    kullaniciadi[strcspn(kullaniciadi, "\n")] = '\0';

    printf("Sifre: ");
    tamponTemizle();
    fgets(sifre, 50, stdin);
    sifre[strcspn(sifre, "\n")] = '\0';

    if(strcmp(kullaniciadi, "yonetici") == 0 && strcmp(sifre, "1234") == 0 ){
        strcpy(aktifKullanici, kullaniciadi);
        return 1;
    }
    if(strcmp(kullaniciadi, "personel") == 0 && strcmp(sifre, "2222") == 0 ){
        strcpy(aktifKullanici, kullaniciadi);
        return 2;
    }
    if(strcmp(kullaniciadi, "ogrenci") == 0 && strcmp(sifre, "1111") == 0){
        strcpy(aktifKullanici, kullaniciadi);
        return 3;
    }
    Kullanici *gecici = kullaniciBas;
    while (gecici != NULL) {
        if (strcmp(gecici->kullaniciAdi, kullaniciadi) == 0 && strcmp(gecici->sifre, sifre) == 0) {
            strcpy(aktifKullanici, kullaniciadi);
            return gecici->rol;
        }
        gecici = gecici->sonraki;
    }
    return 0;
}

void kitapEkle() {
    if (aktifRol != 1 && aktifRol != 2) {
        printf("Bu islem icin yetkiniz bulunmamaktadir.\n");
        return;
    }

    Kitap *yeni = (Kitap*)malloc(sizeof(Kitap));

    printf("\n--- YENI KITAP EKLE ---\n");
    printf("ISBN: ");
    tamponTemizle();
    fgets(yeni->isbn, 20, stdin);
    yeni->isbn[strcspn(yeni->isbn, "\n")] = '\0';

    printf("Kitap Adi: ");
    tamponTemizle();
    fgets(yeni->ad, 100, stdin);
    yeni->ad[strcspn(yeni->ad, "\n")] = '\0';

    printf("Yazar: ");
    tamponTemizle();
    fgets(yeni->yazar, 100, stdin);
    yeni->yazar[strcspn(yeni->yazar, "\n")] = '\0';

    printf("Yayinevi: ");
    tamponTemizle();
    fgets(yeni->yayinevi, 100, stdin);
    yeni->yayinevi[strcspn(yeni->yayinevi, "\n")] = '\0';

    printf("Yayin Yili: ");
    scanf("%d", &yeni->yayinYili);

    printf("Stok Sayisi: ");
    scanf("%d", &yeni->stok);

    printf("Konum (Orn: Raf-A1): ");
    tamponTemizle();
    fgets(yeni->konum, 20, stdin);
    yeni->konum[strcspn(yeni->konum, "\n")] = '\0';

    yeni->oduncSayisi = 0;
    yeni->sonraki = NULL;

    if (bas == NULL) {
        bas = yeni;
    }
    else {
        Kitap *gecici = bas;
        while (gecici->sonraki != NULL) {
            gecici = gecici->sonraki;
        }
        gecici->sonraki = yeni;
    }
    printf("\nKitap basariyla eklendi!\n");
}

void kitapListele() {
    printf("\n--- KUTUPHANEDEKI KITAPLAR ---\n");

    if (bas == NULL) {
        printf("Kutuphanede su an hic kitap bulunmamaktadir.\n");
        return;
    }

    Kitap *gecici = bas;
    int sira = 1;

    while (gecici != NULL) {
        printf("\n%d. Kitap Bilgileri:\n", sira);
        printf("ISBN: %s\n", gecici->isbn);
        printf("Ad: %s\n", gecici->ad);
        printf("Yazar: %s\n", gecici->yazar);
        printf("Yayinevi: %s\n", gecici->yayinevi);
        printf("Yayin Yili: %d\n", gecici->yayinYili);
        printf("Stok: %d\n", gecici->stok);
        printf("Konum: %s\n", gecici->konum);
        printf("Toplam Odunc Sayisi: %d\n", gecici->oduncSayisi);
        printf("--------------------------\n");

        gecici = gecici->sonraki;
        sira++;
    }
}

void kitapAra() {
    printf("\n--- KITAP ARAMA VE COK KRITERLI FILTRELEME ---\n");
    if (bas == NULL) {
        printf("Kutuphane bos oldugu icin arama yapilamaz.\n");
        return;
    }

    char arananTerim[100];
    printf("Aranacak kelimeyi girin (Ad, Yazar veya ISBN): ");
    tamponTemizle();
    fgets(arananTerim, 100, stdin);
    arananTerim[strcspn(arananTerim, "\n")] = '\0';

    Kitap *gecici = bas;
    int bulundu = 0;

    while (gecici != NULL) {
        if (strstr(gecici->ad, arananTerim) != NULL ||
            strstr(gecici->yazar, arananTerim) != NULL ||
            strcmp(gecici->isbn, arananTerim) == 0) {

            printf("\n====================================\n");
            printf("        DETAYLI SONUC GOSTERIMI     \n");
            printf("====================================\n");
            printf("ISBN No     : %s\n", gecici->isbn);
            printf("Kitap Adi   : %s\n", gecici->ad);
            printf("Yazari      : %s\n", gecici->yazar);
            printf("Yayinevi    : %s\n", gecici->yayinevi);
            printf("Yayin Yili  : %d\n", gecici->yayinYili);
            printf("Stok Durumu : %d adet mevcut\n", gecici->stok);
            printf("Raf Konumu  : %s\n", gecici->konum);
            printf("------------------------------------\n");
            bulundu = 1;
        }
        gecici = gecici->sonraki;
    }

    if (bulundu == 0) {
        printf("Aradiginiz kriterlere uygun bir kitap bulunamadi.\n");
    }
}

void kitapSil() {
    if (aktifRol != 1) {
        printf("Bu islem icin yetkiniz bulunmamaktadir.\n");
        return;
    }

    printf("\n--- KITAP SILME ---\n");
    if (bas == NULL) {
        printf("Kutuphane bos oldugu icin silme islemi yapilamaz.\n");
        return;
    }

    char silinecekISBN[20];
    printf("Silmek istediginiz kitabin ISBN numarasini girin: ");
    tamponTemizle();
    fgets(silinecekISBN, 20, stdin);
    silinecekISBN[strcspn(silinecekISBN, "\n")] = '\0';

    Kitap *gecici = bas;
    Kitap *onceki = NULL;
    int bulundu = 0;

    if (strcmp(bas->isbn, silinecekISBN) == 0) {
        bas = bas->sonraki;
        free(gecici);
        printf("Kitap sistemden basariyla silindi!\n");
        return;
    }

    while (gecici != NULL) {
        if (strcmp(gecici->isbn, silinecekISBN) == 0) {
            onceki->sonraki = gecici->sonraki;
            free(gecici);
            bulundu = 1;
            printf("Kitap sistemden basariyla silindi!\n");
            break;
        }
        onceki = gecici;
        gecici = gecici->sonraki;
    }

    if (bulundu == 0) {
        printf("Girilen ISBN numarasina ait bir kitap bulunamadi.\n");
    }
}

void kitapGuncelle() {
    if (aktifRol != 1 && aktifRol != 2) {
        printf("Bu islem icin yetkiniz bulunmamaktadir.\n");
        return;
    }

    printf("\n--- KITAP GUNCELLEME ---\n");
    if (bas == NULL) {
        printf("Kutuphane bos oldugu icin guncelleme islemi yapilamaz.\n");
        return;
    }

    char arananISBN[20];
    printf("Guncellemek istediginiz kitabin ISBN numarasini girin: ");
    tamponTemizle();
    fgets(arananISBN, 20, stdin);
    arananISBN[strcspn(arananISBN, "\n")] = '\0';

    Kitap *gecici = bas;
    int bulundu = 0;

    while (gecici != NULL) {

        if (strcmp(gecici->isbn, arananISBN) == 0) {
            bulundu = 1;
            printf("\nKitap Mevcut Bilgileri:\n");
            printf("Ad: %s | Yazar: %s | Stok: %d | Konum: %s\n", gecici->ad, gecici->yazar, gecici->stok, gecici->konum);

            printf("\nYeni Stok Sayisini Girin: ");
            scanf("%d", &gecici->stok);

            printf("Yeni Konumu Girin (Orn: Raf-B2): ");
            tamponTemizle();
            fgets(gecici->konum, 20, stdin);
            gecici->konum[strcspn(gecici->konum, "\n")] = '\0';

            printf("\nKitap bilgileri basariyla guncellendi!\n");
            break;
        }
        gecici = gecici->sonraki;
    }

    if (bulundu == 0) {
        printf("Girilen ISBN numarasina ait bir kitap bulunamadi.\n");
    }
}

void raporAl() {
    if (aktifRol != 1 && aktifRol != 2) {
        printf("Bu islem icin yetkiniz bulunmamaktadir.\n");
        return;
    }

    printf("\n=== KUTUPHANE SISTEM RAPORU ===\n");
    if (bas == NULL) {
        printf("Kutuphane bos oldugu icin rapor hazirlanamadi.\n");
        return;
    }

    int farkliKitapSayisi = 0;
    int toplamStok = 0;
    Kitap *gecici = bas;

    while (gecici != NULL) {
        farkliKitapSayisi++;
        toplamStok += gecici->stok;
        gecici = gecici->sonraki;
    }

    printf("[1] Envanter Ozeti:\n");
    printf("    - Farkli Kitap Cesidi: %d\n", farkliKitapSayisi);
    printf("    - Raflardaki Toplam Kitap Stogu: %d\n", toplamStok);

    printf("\n[2] Populer Kitaplar (En Cok Odunc Alinanlar):\n");
    gecici = bas;
    int populerBulundu = 0;
    while(gecici != NULL) {
        if(gecici->oduncSayisi > 0) {
            printf("    - %s | Toplam Odunc: %d kez", gecici->ad, gecici->oduncSayisi);
            if(gecici->stok < 3) printf(" (Stok Azaliyor! Kalan: %d)", gecici->stok);
            printf("\n");
            populerBulundu = 1;
        }
        gecici = gecici->sonraki;
    }
    if(!populerBulundu) printf("    - Henuz odunc alinmis kitap bulunmamaktadir.\n");

    printf("\n[3] Gecikmis Iadeler Durumu:\n");
    Odunc *go = oduncBas;
    int gecikmeBulundu = 0;
    while (go != NULL) {
        if (go->iadeEdildi == 0) {
            long int gecenGun = (time(NULL) - go->oduncTarihi) / 86400;
            if (gecenGun > go->sureGun) {
                printf("    - Kullanici: %s | ISBN: %s | %ld gun gecikmis\n", go->kullaniciAdi, go->isbn, gecenGun - go->sureGun);
                gecikmeBulundu = 1;
            }
        }
        go = go->sonraki;
    }
    if (!gecikmeBulundu)
        printf("    - Su an aktif gecikmede olan odunc kitap bulunmamaktadir.\n");

    printf("--------------------------------\n");
}

void dosyayaKaydet() {
    FILE *dosya = fopen("kutuphane.txt", "w");
    if (dosya != NULL) {
        Kitap *gecici = bas;
        while (gecici != NULL) {
            fprintf(dosya, "%s , %s , %s , %s , %d , %d , %s , %d\n",
                    gecici->isbn, gecici->ad, gecici->yazar, gecici->yayinevi,
                    gecici->yayinYili, gecici->stok, gecici->konum, gecici->oduncSayisi);
            gecici = gecici->sonraki;
        }
        fclose(dosya);
    }

    FILE *fKullanici = fopen("kullanicilar.txt", "w");
    if (fKullanici != NULL) {
        Kullanici *gk = kullaniciBas;
        while (gk != NULL) {
            char geciciSifre[50];
            strcpy(geciciSifre, gk->sifre);
            sifrele(geciciSifre);
            fprintf(fKullanici, "%s , %s , %d\n", gk->kullaniciAdi, geciciSifre, gk->rol);
            gk = gk->sonraki;
        }
        fclose(fKullanici);
    }

    FILE *fOdunc = fopen("odunc.txt", "w");
    if (fOdunc != NULL) {
        Odunc *go = oduncBas;
        while (go != NULL) {
            fprintf(fOdunc, "%s , %s , %ld , %d , %d\n", go->isbn, go->kullaniciAdi, go->oduncTarihi, go->sureGun, go->iadeEdildi);
            go = go->sonraki;
        }
        fclose(fOdunc);
    }
    printf("Tum veriler basariyla kaydedildi!\n");
}

void kitapOduncAl() {
    printf("\n--- KITAP ODUNC ALMA ---\n");
    if (bas == NULL) {
        printf("Kutuphane bos oldugu icin odunc verilemez.\n");
        return;
    }

    char arananISBN[20];
    printf("Odunc almak istediginiz kitabin ISBN numarasini girin: ");
    tamponTemizle();
    fgets(arananISBN, 20, stdin);
    arananISBN[strcspn(arananISBN, "\n")] = '\0';

    Odunc *kontrol = oduncBas;
    while (kontrol != NULL) {
        if (strcmp(kontrol->isbn, arananISBN) == 0 && strcmp(kontrol->kullaniciAdi, aktifKullanici) == 0 && kontrol->iadeEdildi == 0) {
            printf("Bu kitabi zaten odunc aldiniz. Once iade etmeniz gerekmektedir.\n");
            return;
        }
        kontrol = kontrol->sonraki;
    }

    Kitap *gecici = bas;
    while (gecici != NULL) {
        if (strcmp(gecici->isbn, arananISBN) == 0) {
            if (gecici->stok > 0) {
                gecici->stok--;
                gecici->oduncSayisi++;

                Odunc *yeni = (Odunc*)malloc(sizeof(Odunc));
                strcpy(yeni->isbn, arananISBN);
                strcpy(yeni->kullaniciAdi, aktifKullanici);
                yeni->oduncTarihi = time(NULL);

                printf("Kac gunluk odunc aliyorsunuz?: ");
                scanf("%d", &yeni->sureGun);

                yeni->iadeEdildi = 0;
                yeni->sonraki = oduncBas;
                oduncBas = yeni;

                printf("'%s' adli kitap adiniza basariyla tescil edildi.\n", gecici->ad);
                return;
            } else {
                printf("Uzgunuz, bu kitabin stogu tukenmistir.\n");
                return;
            }
        }
        gecici = gecici->sonraki;
    }
    printf("Girilen ISBN'e ait kitap bulunamadi.\n");
}

void kitapIadeEt() {
    printf("\n--- KITAP IADE ETME ---\n");
    char arananISBN[20];
    printf("Iade etmek istediginiz kitabin ISBN numarasini girin: ");
    tamponTemizle();
    fgets(arananISBN, 20, stdin);
    arananISBN[strcspn(arananISBN, "\n")] = '\0';

    Odunc *go = oduncBas;
    while (go != NULL) {
        if (strcmp(go->isbn, arananISBN) == 0 && strcmp(go->kullaniciAdi, aktifKullanici) == 0 && go->iadeEdildi == 0) {
            go->iadeEdildi = 1;

            Kitap *gk = bas;
            while (gk != NULL) {
                if (strcmp(gk->isbn, arananISBN) == 0) { gk->stok++; break; }
                gk = gk->sonraki;
            }

            long int gecenGun = (time(NULL) - go->oduncTarihi) / 86400;
            if (gecenGun > go->sureGun) {
                printf("Iade basarili! Ancak %ld gun GECIKME tespit edildi. Ceza uygulandi.\n", gecenGun - go->sureGun);
            } else {
                printf("Kitap zamaninda basariyla iade edildi. Tesekkurler!\n");
            }
            return;
        }
        go = go->sonraki;
    }
    printf("Aktif bir odunc kaydi bulunamadi.\n");
}

void dosyadanYukle() {
    FILE *dosya = fopen("kutuphane.txt", "r");
    if (dosya != NULL) {
        char isbn[20], ad[100], yazar[100], yayinevi[100], konum[20];
        int yayinYili, stok, oduncSayisi;
        while (fscanf(dosya, "%19s , %99s , %99s , %99s , %d , %d , %19s , %d\n",
                      isbn, ad, yazar, yayinevi, &yayinYili, &stok, konum, &oduncSayisi) == 8) {
            Kitap *yeni = (Kitap*)malloc(sizeof(Kitap));
            strcpy(yeni->isbn, isbn); strcpy(yeni->ad, ad); strcpy(yeni->yazar, yazar);
            strcpy(yeni->yayinevi, yayinevi); yeni->yayinYili = yayinYili; yeni->stok = stok;
            strcpy(yeni->konum, konum); yeni->oduncSayisi = oduncSayisi; yeni->sonraki = NULL;
            if (bas == NULL) { bas = yeni; }
            else { Kitap *gecici = bas; while (gecici->sonraki != NULL) { gecici = gecici->sonraki; } gecici->sonraki = yeni; }
        }
        fclose(dosya);
    }

    FILE *fKullanici = fopen("kullanicilar.txt", "r");
    if (fKullanici != NULL) {
        char kadi[50], sifre[50]; int rol;
        while (fscanf(fKullanici, "%49s , %49s , %d\n", kadi, sifre, &rol) == 3) {
            Kullanici *yeni = (Kullanici*)malloc(sizeof(Kullanici));
            sifrele(sifre);
            strcpy(yeni->kullaniciAdi, kadi); strcpy(yeni->sifre, sifre); yeni->rol = rol; yeni->sonraki = NULL;
            if (kullaniciBas == NULL) { kullaniciBas = yeni; }
            else { Kullanici *g = kullaniciBas; while (g->sonraki != NULL) { g = g->sonraki; } g->sonraki = yeni; }
        }
        fclose(fKullanici);
    }

    FILE *fOdunc = fopen("odunc.txt", "r");
    if (fOdunc != NULL) {
        char oIsbn[20], oKadi[50]; long int oTarih; int oSure, oIade;
        while (fscanf(fOdunc, "%19s , %49s , %ld , %d , %d\n", oIsbn, oKadi, &oTarih, &oSure, &oIade) == 5) {
            Odunc *yeni = (Odunc*)malloc(sizeof(Odunc));
            strcpy(yeni->isbn, oIsbn); strcpy(yeni->kullaniciAdi, oKadi); yeni->oduncTarihi = oTarih; yeni->sureGun = oSure; yeni->iadeEdildi = oIade; yeni->sonraki = NULL;
            if (oduncBas == NULL) { oduncBas = yeni; }
            else { Odunc *g = oduncBas; while (g->sonraki != NULL) { g = g->sonraki; } g->sonraki = yeni; }
        }
        fclose(fOdunc);
    }
}

void kullaniciEkle() {
    Kullanici *yeni = (Kullanici*)malloc(sizeof(Kullanici));
    printf("\n--- YENI KULLANICI OLUSTUR ---\n");
    printf("Kullanici Adi: ");
    tamponTemizle();
    fgets(yeni->kullaniciAdi, 50, stdin);
    yeni->kullaniciAdi[strcspn(yeni->kullaniciAdi, "\n")] = '\0';
    printf("Sifre: ");
    tamponTemizle();
    fgets(yeni->sifre, 50, stdin);
    yeni->sifre[strcspn(yeni->sifre, "\n")] = '\0';
    printf("Rol Secin (1: Yonetici, 2: Personel, 3: Ogrenci): "); scanf("%d", &yeni->rol);
    yeni->sonraki = NULL;

    if (kullaniciBas == NULL) { kullaniciBas = yeni; }
    else {
        Kullanici *gecici = kullaniciBas;
        while (gecici->sonraki != NULL) { gecici = gecici->sonraki; }
        gecici->sonraki = yeni;
    }
    printf("Kullanici hesabi basariyla olusturuldu!\n");
}

void kullaniciGuncelle() {
    printf("\n--- KULLANICI GUNCELLEME ---\n");
    if (kullaniciBas == NULL) { printf("Sistemde kayitli kullanici bulunmamaktadir.\n"); return; }

    char arananAd[50];
    printf("Guncellemek istediginiz kullanici adini girin: ");
    tamponTemizle();
    fgets(arananAd, 50, stdin);
    arananAd[strcspn(arananAd, "\n")] = '\0';

    Kullanici *gecici = kullaniciBas;
    int bulundu = 0;

    while (gecici != NULL) {
        if (strcmp(gecici->kullaniciAdi, arananAd) == 0) {
            bulundu = 1;
            printf("\nMevcut Bilgiler -> Sifre: %s | Rol: %d\n", gecici->sifre, gecici->rol);
            printf("Yeni Sifre Girin: ");
            tamponTemizle();
            fgets(gecici->sifre, 50, stdin);
            gecici->sifre[strcspn(gecici->sifre, "\n")] = '\0';
            printf("Yeni Rol Girin (1: Yonetici, 2: Personel, 3: Ogrenci): ");
            scanf("%d", &gecici->rol);
            printf("Kullanici bilgileri basariyla guncellendi!\n");
            break;
        }
        gecici = gecici->sonraki;
    }

    if (bulundu == 0) { printf("Kullanici bulunamadi.\n"); }
}

void kullaniciSil() {
    printf("\n--- KULLANICI SILME ---\n");
    if (kullaniciBas == NULL) { 
        printf("Sistemde kayitli kullanici yok.\n"); 
        return; }
    char silinecekAd[50]; 
    printf("Silmek istediginiz kullanici adini girin: ");
    tamponTemizle();
    fgets(silinecekAd, 50, stdin);
    silinecekAd[strcspn(silinecekAd, "\n")] = '\0';

    Kullanici *gecici = kullaniciBas, *onceki = NULL;
    if (strcmp(kullaniciBas->kullaniciAdi, silinecekAd) == 0) { 
        kullaniciBas = kullaniciBas->sonraki; 
        free(gecici); 
        printf("Kullanici silindi.\n"); 
        return; }
    while (gecici != NULL && strcmp(gecici->kullaniciAdi, silinecekAd) != 0) { 
        onceki = gecici; 
        gecici = gecici->sonraki; }
    if (gecici == NULL) { 
        printf("Kullanici bulunamadi.\n"); 
        return; }
    onceki->sonraki = gecici->sonraki; 
    free(gecici);
    printf("Kullanici sistemden silindi.\n");
}

int main(){
    dosyadanYukle();

    aktifRol = girisYap();
    if(aktifRol == 0){
        printf("Hatali giris!");
    }
    else{
        printf("Giris basarili\n");
        int secim = -1;

        while (secim != 0) {
            printf("\n=== KUTUPHANE SISTEMI ===\n");
            if (aktifRol == 1 || aktifRol == 2) printf("1. Kitap Ekle\n");
            printf("2. Kitaplari Listele\n");
            printf("3. Kitap Ara\n");
            if (aktifRol == 1) printf("4. Kitap Sil\n");
            if (aktifRol == 1 || aktifRol == 2) printf("5. Kitap Guncelle\n");
            printf("6. Kitap Odunc Al\n");
            printf("7. Kitap Iade Et\n");
            if (aktifRol == 1 || aktifRol == 2) printf("8. Sistem Raporu\n");
            if (aktifRol == 1) printf("9. Kullanici Yonetimi\n");
            printf("0. Cikis\n");
            printf("Seciminiz: ");
            scanf("%d", &secim);

            if (secim == 1) {
                kitapEkle();
            }
            else if (secim == 2) {
                kitapListele();
            }
            else if (secim == 3) {
                kitapAra();
            }
            else if (secim == 4) {
                kitapSil();
            }
            else if (secim == 5) {
                kitapGuncelle();
            }
            else if (secim == 6) {
                kitapOduncAl();
            }
            else if (secim == 7) {
                kitapIadeEt();
            }
            else if (secim == 8) {
                raporAl();
            }
            else if (secim == 9) {
                printf("\n1. Yeni Kullanici Olustur\n2. Kullanici Guncelle\n3. Kayitli Kullaniciyi Sil\nSeciminiz: ");
                int kSecim; 
                scanf("%d", &kSecim);
                if(kSecim == 1) kullaniciEkle();
                else if(kSecim == 2) kullaniciGuncelle();
                else if(kSecim == 3) kullaniciSil();
            }
            else if (secim == 0) {
                dosyayaKaydet();
                printf("Sistemden cikiliyor. Iyi gunler!\n");
            }
            else {
                printf("Gecersiz secim. Lutfen tekrar deneyin.\n");
            }
        }
    }
}