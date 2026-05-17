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

if(strcmp(kullaniciadi, "admin") == 0 && strcmp(sifre, "1234") == 0 ){
    return 1;
}
if(strcmp(kullaniciadi, "ogrenci") == 0 && strcmp(sifre, "1111") == 0){
    return 2;
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
    
    printf("Stok Sayisi: ");
    scanf("%d", &yeni->stok);
    
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
        printf("Stok: %d\n", gecici->stok);
        printf("--------------------------\n");
        
        gecici = gecici->sonraki;
        sira++;
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
        fprintf(dosya, "%s|%s|%s|%d\n", gecici->isbn, gecici->ad, gecici->yazar, gecici->stok);
        
        gecici = gecici->sonraki;
    }
    
    fclose(dosya);
    printf("Tum kitaplar kutuphane.txt dosyasina guvenle kaydedildi!\n");
}

int main(){
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