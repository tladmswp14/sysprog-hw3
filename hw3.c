#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE 1000000000  
#define BASE_DIGITS 9    


typedef struct {
    int* digits;    
    int size;        
    int capacity;    
} BigInt;


BigInt* init_bigint(long long num) {
    BigInt* n = (BigInt*)malloc(sizeof(BigInt));
    n->capacity = 10;
    n->digits = (int*)calloc(n->capacity, sizeof(int));
    n->size = 0;

    if (num == 0) {
        n->size = 1;
        n->digits[0] = 0;
        return n;
    }

    while (num > 0) {
        n->digits[n->size++] = num % BASE;
        num /= BASE;
    }

    return n;
}


void free_bigint(BigInt* n) {
    free(n->digits);
    free(n);
}


void resize_bigint(BigInt* n, int new_capacity) {
    if (new_capacity > n->capacity) {
        int* new_digits = (int*)calloc(new_capacity, sizeof(int));
        memcpy(new_digits, n->digits, n->size * sizeof(int));
        free(n->digits);
        n->digits = new_digits;
        n->capacity = new_capacity;
    }
}


BigInt* big_mult(BigInt* a, BigInt* b) {
    BigInt* result = (BigInt*)malloc(sizeof(BigInt));
    result->capacity = a->size + b->size + 1;
    result->digits = (int*)calloc(result->capacity, sizeof(int));
    result->size = a->size + b->size;


    for (int i = 0; i < a->size; i++) {
        long long carry = 0;
        for (int j = 0; j < b->size || carry > 0; j++) {
            long long cur = result->digits[i + j] +
                (long long)a->digits[i] * (j < b->size ? b->digits[j] : 0) +
                carry;
            result->digits[i + j] = cur % BASE;
            carry = cur / BASE;
        }
    }


    while (result->size > 1 && result->digits[result->size - 1] == 0) {
        result->size--;
    }

    return result;
}


char* bigint_to_hex(BigInt* n) {
    int total_digits = (n->size - 1) * BASE_DIGITS + 1;
    long long temp = n->digits[n->size - 1];
    while (temp >= 10) {
        total_digits++;
        temp /= 10;
    }

   
    char* decimal = (char*)calloc(total_digits + 1, sizeof(char));

   
    int* temp_digits = (int*)calloc(n->size, sizeof(int));
    memcpy(temp_digits, n->digits, n->size * sizeof(int));
    int temp_size = n->size;

    int pos = 0;
    while (temp_size > 1 || temp_digits[0] > 0) {
        long long carry = 0;
        for (int i = temp_size - 1; i >= 0; i--) {
            long long cur = carry * BASE + temp_digits[i];
            temp_digits[i] = cur / 10;
            carry = cur % 10;
        }
        decimal[pos++] = '0' + carry;

        while (temp_size > 1 && temp_digits[temp_size - 1] == 0) {
            temp_size--;
        }
    }

    free(temp_digits);

   
    for (int i = 0; i < pos / 2; i++) {
        char t = decimal[i];
        decimal[i] = decimal[pos - 1 - i];
        decimal[pos - 1 - i] = t;
    }
    decimal[pos] = '\0';

    int dec_len = strlen(decimal);
    char* hex = (char*)calloc(dec_len + 10, sizeof(char));

 
    int hex_digits[1000];
    int hex_size = 0;

   
    int* dec_array = (int*)calloc(dec_len, sizeof(int));
    for (int i = 0; i < dec_len; i++) {
        dec_array[i] = decimal[i] - '0';
    }

    int dec_size = dec_len;
    while (dec_size > 0 && !(dec_size == 1 && dec_array[0] == 0)) {
        int remainder = 0;
        for (int i = 0; i < dec_size; i++) {
            int cur = remainder * 10 + dec_array[i];
            dec_array[i] = cur / 16;
            remainder = cur % 16;
        }
        hex_digits[hex_size++] = remainder;

        while (dec_size > 1 && dec_array[0] == 0) {
            for (int i = 0; i < dec_size - 1; i++) {
                dec_array[i] = dec_array[i + 1];
            }
            dec_size--;
        }
    }

    free(dec_array);
    free(decimal);


    if (hex_size == 0) {
        strcpy(hex, "0x0");
    }
    else {
        strcpy(hex, "0x");
        for (int i = hex_size - 1; i >= 0; i--) {
            char digit = hex_digits[i] < 10 ? '0' + hex_digits[i] : 'A' + (hex_digits[i] - 10);
            int len = strlen(hex);
            hex[len] = digit;
            hex[len + 1] = '\0';
        }
    }

    return hex;
}


void print_bigint_decimal(BigInt* n) {
    printf("%d", n->digits[n->size - 1]);
    for (int i = n->size - 2; i >= 0; i--) {
        printf("%09d", n->digits[i]);
    }
}

int main() {
    printf("테스트 결과\n\n");

    // Test 100,000 x 100,000
    printf("Test Case 1: 100,000 x 100,000\n");
    

    BigInt* num1 = init_bigint(100000);
    BigInt* num2 = init_bigint(100000);

    printf("Input 1: 100,000 (0x186A0)\n");
    printf("Input 2: 100,000 (0x186A0)\n\n");

    BigInt* result1 = big_mult(num1, num2);

    printf("Result in Decimal: ");
    print_bigint_decimal(result1);
    printf("\n");

    char* hex1 = bigint_to_hex(result1);
    printf("Result in Hexadecimal: %s\n", hex1);
    printf("Expected: 0x2540BE400 (10,000,000,000)\n\n");

    free(hex1);
    free_bigint(num1);
    free_bigint(num2);
    free_bigint(result1);

    // Test 2 
    printf("\nTest Case 2: 999,999 x 999,999\n");
    

    BigInt* num3 = init_bigint(999999);
    BigInt* num4 = init_bigint(999999);

    printf("Input 1: 999,999 (0xF423F)\n");
    printf("Input 2: 999,999 (0xF423F)\n\n");

    BigInt* result2 = big_mult(num3, num4);

    printf("Result in Decimal: ");
    print_bigint_decimal(result2);
    printf("\n");

    char* hex2 = bigint_to_hex(result2);
    printf("Result in Hexadecimal: %s\n", hex2);
    printf("Expected: 0xEE6B27FF0001 (999,998,000,001)\n\n");

    free(hex2);
    free_bigint(num3);
    free_bigint(num4);
    free_bigint(result2);

    // Test 3
    printf("\nTest Case 3: 123,456,789 x 987,654,321\n");
    

    BigInt* num5 = init_bigint(123456789);
    BigInt* num6 = init_bigint(987654321);

    printf("Input 1: 123,456,789 (0x75BCD15)\n");
    printf("Input 2: 987,654,321 (0x3ADE68B1)\n\n");

    BigInt* result3 = big_mult(num5, num6);

    printf("Result in Decimal: ");
    print_bigint_decimal(result3);
    printf("\n");

    char* hex3 = bigint_to_hex(result3);
    printf("Result in Hexadecimal: %s\n", hex3);

    free(hex3);
    free_bigint(num5);
    free_bigint(num6);
    free_bigint(result3);

    printf("끝\n");

    return 0;
}