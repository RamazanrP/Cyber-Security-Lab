#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(char *program_name) {
    printf("Kullanim: %s <giris_dosyasi> <cikis_dosyasi> <anahtar>\n", program_name);
    printf("Ornek: %s gizli.txt sifreli.dat mysecretkey\n", program_name);
}

int main(int argc, char *argv[]) {
    // 1. Argüman Kontrolü: Terminalden doğru girilmiş mi?
    if (argc != 4) { //argc değişkeni terminalde(shell)de komut satırını girerken arttı, özel olarak saymadı, direkt 4 ten başladı
        print_usage(argv[0]);// eğer argc 4 değilse yani program + input + output + şifre yani 4 adet string yoksa,  direkt argv[0]
        return 1;            // argv[0] da programımızı(xor_tool mesela) çalıştırır, kullanma kılavuzu gibi kullanıcıyı yönlendirir
    }

    char *input_filename = argv[1];  //char veya int = 5 veya 'A' demek yerine *... dersek değişkenin tamamını değil,
    char *output_filename = argv[2]; // sadece ADRESini vermiş oluruz, git, şurada bulacaksın demiş olduk böylece
    char *key = argv[3];
    int key_len = strlen(key);

    // 2. Dosyaları Binary Açma, r=read, w=write, b=binary
    FILE *fin = fopen(input_filename, "rb"); // Okumak için atadık
    FILE *fout = fopen(output_filename, "wb"); // Nereye yazdıracağımıza bakıyoruz
    // sadece r deseydik iki karakterli \n \r veya EOF a benzer dizimler yüzünden orijinal bozulabilirdi. Binary, kesinliktir!
    if (fin == NULL) {
        printf("Hata: Giris dosyasi '%s' acilamadi!\n", input_filename);
        return 1; //Eğer kullanıcı bir şey vermemişse NULL döner, NULL döndüyse uyarı versin
    }
    if (fout == NULL) {
        printf("Hata: Cikis dosyasi '%s' olusturulamadi!\n", output_filename);
        fclose(fin); // Çıkış yerini girmediyse uyarı versin
        return 1;
    }

    printf("[*] Islem baslatiliyor...\n");
    printf("[*] Anahtar: %s\n", key);

    // 3. Şifreleme / Deşifreleme Döngüsü
    int ch; //char değil int yaptık çünkü karakterler (unsigned char) 0-255 arasındadır
    int i = 0;
    
    // Dosyadan byte byte oku (EOF= End of File)
    while ((ch = fgetc(fin)) != EOF) { //Önce ch'yi atadık sonra bu ch EOF a eşit mi değil mi kontrole soktuk
        char cipher_byte = ch ^ key[i % key_len];// i, binlerce harf olabilir, ama biz am key'in indexi kadar giderbiliriz
        // key= "gizli" olsun, key[1]= i olacak, i=6 olduğunda key[6]'ya değil [6%5]'e yani 1 e yani key[1]=i 'ye göre şifre oluşacak
        
        fputc(cipher_byte, fout); //Sonucu(şifreli metini) cipher'den aldık, çıkış dosyasına yerleştirdik,
                                  //printf deseydik sadece terminalde gözükecekti,bunu yaparak direkt dosya içinde kaydedildi.
        i++;
    }

    // 4. Temizlik ve Kapanış
    printf("[+] Islem basariyla tamamlandi! (%d bytes islendi)\n", i);
    
    fclose(fin);
    fclose(fout);

    return 0;
}