#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 1024

// ===================== プロトタイプ宣言 =====================
void caesar_encrypt(char *text, int shift);
void caesar_decrypt(char *text, int shift);
void substitution_encrypt(char *text);
void substitution_decrypt(char *text);
void xor_cipher(char *data, const char *key);
void print_hex(const char *title, const char *data, int len);
void print_menu(void);

// ===================== 定数定義 =====================
const char *plain_alph = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *cipher_alph = "QWERTYUIOPASDFGHJKLZXCVBNM";

// ===================== メイン処理 =====================
int main(void) {
    int choice, shift, len;
    char plain[MAX_LEN], key[MAX_LEN];
    char buffer[MAX_LEN];

    while (1) {
        print_menu();
        printf("\n実行する暗号を選択してください (0～4): ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // 入力バッファクリア
            printf("無効な入力です。\n");
            continue;
        }

        if (choice == 0) {
            printf("終了します。\n");
            break;
        }

        switch (choice) {
        case 1:
            // シーザー暗号
            printf("【シーザー暗号】\n平文を入力: ");
            while (getchar() != '\n');
            fgets(plain, MAX_LEN, stdin);
            len = strcspn(plain, "\n");
            plain[len] = 0;

            printf("シフト数 (1～25): ");
            scanf("%d", &shift);

            strcpy(buffer, plain);
            caesar_encrypt(buffer, shift);
            printf("→ 暗号文: %s\n", buffer);

            caesar_decrypt(buffer, shift);
            printf("→ 復号文: %s\n\n", buffer);
            break;

        case 2:
            // 単一換字式暗号
            printf("【単一換字式暗号】\n平文を入力: ");
            while (getchar() != '\n');
            fgets(plain, MAX_LEN, stdin);
            len = strcspn(plain, "\n");
            plain[len] = 0;

            strcpy(buffer, plain);
            substitution_encrypt(buffer);
            printf("→ 暗号文: %s\n", buffer);

            substitution_decrypt(buffer);
            printf("→ 復号文: %s\n\n", buffer);
            break;

        case 3:
            // XOR暗号
            printf("【XOR暗号】\n平文を入力: ");
            while (getchar() != '\n');
            fgets(plain, MAX_LEN, stdin);
            len = strcspn(plain, "\n");
            plain[len] = 0;

            printf("共通キーを入力: ");
            fgets(key, MAX_LEN, stdin);
            key[strcspn(key, "\n")] = 0;

            strcpy(buffer, plain);
            xor_cipher(buffer, key);
            print_hex("→ 暗号文(16進)", buffer, len);

            xor_cipher(buffer, key);
            printf("→ 復号文: %s\n\n", buffer);
            break;

        case 4:
            // 一括実行
            printf("===== 一括実行 =====\n共通の平文を入力: ");
            while (getchar() != '\n');
            fgets(plain, MAX_LEN, stdin);
            len = strcspn(plain, "\n");
            plain[len] = 0;

            printf("\n--- 1. シーザー暗号 ---\nシフト数: 3\n");
            strcpy(buffer, plain);
            caesar_encrypt(buffer, 3);
            printf("暗号文: %s → 復号: ", buffer);
            caesar_decrypt(buffer, 3);
            printf("%s\n", buffer);

            printf("\n--- 2. 単一換字式暗号 ---\n");
            strcpy(buffer, plain);
            substitution_encrypt(buffer);
            printf("暗号文: %s → 復号: ", buffer);
            substitution_decrypt(buffer);
            printf("%s\n", buffer);

            printf("\n--- 3. XOR暗号 ---\nキー: secret123\n");
            strcpy(buffer, plain);
            xor_cipher(buffer, "secret123");
            print_hex("暗号文(16進)", buffer, len);
            xor_cipher(buffer, "secret123");
            printf("復号文: %s\n\n", buffer);
            break;

        default:
            printf("1～4の番号を選択してください。\n\n");
            break;
        }
    }
    return 0;
}

// ===================== 関数定義 =====================

// メニュー表示
void print_menu(void) {
    printf("==============================\n");
    printf("       暗号基礎メニュー       \n");
    printf("==============================\n");
    printf("  1. シーザー暗号\n");
    printf("  2. 単一換字式暗号\n");
    printf("  3. XOR暗号\n");
    printf("  4. 一括実行（全種類）\n");
    printf("  0. 終了\n");
    printf("==============================\n");
}

// シーザー暗号: 暗号化
void caesar_encrypt(char *text, int shift) {
    shift = shift % 26;
    for (int i = 0; text[i] != '\0'; i++) {
        if (isupper(text[i])) {
            text[i] = ((text[i] - 'A' + shift) % 26) + 'A';
        } else if (islower(text[i])) {
            text[i] = ((text[i] - 'a' + shift) % 26) + 'a';
        }
    }
}

// シーザー暗号: 復号化
void caesar_decrypt(char *text, int shift) {
    caesar_encrypt(text, 26 - (shift % 26));
}

// 単一換字式暗号: 暗号化
void substitution_encrypt(char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        char c = toupper(text[i]);
        for (int j = 0; j < 26; j++) {
            if (c == plain_alph[j]) {
                text[i] = isupper(text[i]) ? cipher_alph[j] : tolower(cipher_alph[j]);
                break;
            }
        }
    }
}

// 単一換字式暗号: 復号化
void substitution_decrypt(char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        char c = toupper(text[i]);
        for (int j = 0; j < 26; j++) {
            if (c == cipher_alph[j]) {
                text[i] = isupper(text[i]) ? plain_alph[j] : tolower(plain_alph[j]);
                break;
            }
        }
    }
}

// XOR暗号: 暗号化/復号化
void xor_cipher(char *data, const char *key) {
    int keylen = strlen(key);
    for (int i = 0; data[i] != '\0'; i++) {
        data[i] ^= key[i % keylen];
    }
}

// 16進数で表示
void print_hex(const char *title, const char *data, int len) {
    printf("%s: ", title);
    for (int i = 0; i < len; i++) {
        printf("%02X ", (unsigned char)data[i]);
    }
    printf("\n");
}
