#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct Kitap {
    char isbn[20];
    char ad[100];
    char yazar[100];
    char yayinevi[100];
    int yayinYili;
    int stok;
    char konum[20];
    struct Kitap *sonraki;
} Kitap;

Kitap *bas = NULL;
int aktifRol = 0;

int girisYap() {
    char kullaniciadi[20];
    char sifre[20];

    printf("\n--- KULLANICI GIRISI ---\n");
    printf("Kullanici Adi: ");
    scanf("%s", kullaniciadi);
    
    printf("\nSifre: ");
    scanf("%s", sifre);

if(strcmp(kullaniciadi, "yonetici") == 0 && strcmp(sifre, "1234") == 0 ){
    return 1;
}
if(strcmp(kullaniciadi, "personel") == 0 && strcmp(sifre, "2222") == 0 ){
    return 2;
}
if(strcmp(kullaniciadi, "ogrenci") == 0 && strcmp(sifre, "1111") == 0){
    return 3;
}
return 0;
}

void kitapEkle() {
    Kitap *yeni = (Kitap*)malloc(sizeof(Kitap));
    
    printf("\n--- YENI KITAP EKLE ---\n");
    printf("ISBN: ");
    scanf("%s", yeni->isbn);
    
    printf("Kitap Adi: ");
    scanf("%s", yeni->ad);
    
    printf("Yazar: ");
    scanf("%s", yeni->yazar);

    printf("Yayinevi: ");
    scanf("%s", yeni->yayinevi);
    
    printf("Yayin Yili: ");
    scanf("%d", &yeni->yayinYili);
    
    printf("Stok Sayisi: ");
    scanf("%d", &yeni->stok);

    printf("Konum (Orn: Raf-A1): ");
    scanf("%s", yeni->konum);
    
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
        printf("--------------------------\n");
        
        gecici = gecici->sonraki;
        sira++;
    }
}

void kitapAra() {
    printf("\n--- KITAP ARAMA ---\n");
    if (bas == NULL) {
        printf("Kutuphane bos oldugu icin arama yapilamaz.\n");
        return;
    }

    int aramaSecim;
    printf("1. Kitap Adina Gore Ara\n");
    printf("2. ISBN Numarasina Gore Ara\n");
    printf("Seciminiz: ");
    scanf("%d", &aramaSecim);

    char aranan[100];
    printf("Aranacak kelimeyi girin: ");
    scanf("%s", aranan);

    Kitap *gecici = bas;
    int bulundu = 0;

    while (gecici != NULL) {
        int eslesme = 0;
        
        if (aramaSecim == 1 && strcmp(gecici->ad, aranan) == 0) {
            eslesme = 1;
        } 
        else if (aramaSecim == 2 && strcmp(gecici->isbn, aranan) == 0) {
            eslesme = 1;
        }

        if (eslesme == 1) {
            printf("\nKitap Bulundu!\n");
            printf("ISBN: %s\n", gecici->isbn);
            printf("Ad: %s\n", gecici->ad);
            printf("Yazar: %s\n", gecici->yazar);
            printf("Yayinevi: %s\n", gecici->yayinevi);
            printf("Yayin Yili: %d\n", gecici->yayinYili);
            printf("Stok: %d\n", gecici->stok);
            printf("Konum: %s\n", gecici->konum);
            printf("--------------------------\n");
            bulundu = 1;
        }
        gecici = gecici->sonraki;
    }

    if (bulundu == 0) {
        printf("Aradiginiz kriterlere uygun bir kitap bulunamadi.\n");
    }
}

void kitapSil() {
    printf("\n--- KITAP SILME ---\n");
    if (bas == NULL) {
        printf("Kutuphane bos oldugu icin silme islemi yapilamaz.\n");
        return;
    }

    char silinecekISBN[20];
    printf("Silmek istediginiz kitabin ISBN numarasini girin: ");
    scanf("%s", silinecekISBN);

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
    printf("\n--- KITAP GUNCELLEME ---\n");
    if (bas == NULL) {
        printf("Kutuphane bos oldugu icin guncelleme islemi yapilamaz.\n");
        return;
    }

    char arananISBN[20];
    printf("Guncellemek istediginiz kitabin ISBN numarasini girin: ");
    scanf("%s", arananISBN);

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
            scanf("%s", gecici->konum);
            
            printf("\nKitap bilgileri basariyla guncellendi!\n");
            break;
        }
        gecici = gecici->sonraki;
    }

    if (bulundu == 0) {
        printf("Girilen ISBN numarasina ait bir kitap bulunamadi.\n");
    }
}

void dosyayaKaydet() {
    FILE *dosya = fopen("kutuphane.txt", "w");
    
    if (dosya == NULL) {
        printf("Hata: Dosya acilamadi!\n");
        return;
    }

    Kitap *gecici = bas;
    
    while (gecici != NULL) {
        fprintf(dosya, "%s , %s , %s , %s , %d , %d , %s\n", 
                gecici->isbn, gecici->ad, gecici->yazar, gecici->yayinevi, 
                gecici->yayinYili, gecici->stok, gecici->konum);
        
        gecici = gecici->sonraki;
    }
    
    fclose(dosya);
    printf("Tum kitaplar kutuphane.txt dosyasina guvenle kaydedildi!\n");
}

void dosyadanYukle() {
    FILE *dosya = fopen("kutuphane.txt", "r");
    
    if (dosya == NULL) {
        return; 
    }

    char isbn[20], ad[100], yazar[100], yayinevi[100], konum[20];
    int yayinYili, stok;

    while (fscanf(dosya, "%s , %s , %s , %s , %d , %d , %s\n", 
                  isbn, ad, yazar, yayinevi, &yayinYili, &stok, konum) != EOF) {
        
        Kitap *yeni = (Kitap*)malloc(sizeof(Kitap));
        
        strcpy(yeni->isbn, isbn);
        strcpy(yeni->ad, ad);
        strcpy(yeni->yazar, yazar);
        strcpy(yeni->yayinevi, yayinevi);
        yeni->yayinYili = yayinYili;
        yeni->stok = stok;
        strcpy(yeni->konum, konum);
        yeni->sonraki = NULL;

        if (bas == NULL) {
            bas = yeni;
        } else {
            Kitap *gecici = bas;
            while (gecici->sonraki != NULL) {
                gecici = gecici->sonraki;
            }
            gecici->sonraki = yeni;
        }
    }
    fclose(dosya);
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
            printf("1. Kitap Ekle\n");
            printf("2. Kitaplari Listele\n");
            printf("3. Kitap Ara\n");
            printf("4. Kitap Sil\n");
            printf("5. Kitap Guncelle\n");
            printf("0. Cikis\n");
            printf("Seciminiz: ");
            scanf("%d", &secim);
            
            if (secim == 1) {
                printf("Kitap ekleme menusu calisti...\n");
                kitapEkle();
            }
            else if (secim == 2) {
                printf("Listeleme menusu calisti...\n");
                kitapListele();
                
            }
            else if (secim == 3) {
                printf("Kitap arama menusu calisti...\n");
                kitapAra();
            }
            else if (secim == 4) {
                printf("Kitap silme menusu calisti...\n");
                kitapSil();
            }
            else if (secim == 5) {
                printf("Kitap guncelleme menusu calisti...\n");
                kitapGuncelle();
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