#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define Max 310

typedef enum {FAILURE, SUCCESS} Status_Code;

typedef struct {
    char num[Max];
    int length;
    char sign;
} BigInt;
BigInt addition(const BigInt , const BigInt );

Status_Code initialise(const char *str, BigInt* n) {
    BigInt result = {{0}, 0, '+'};
    int i = 0, j;

    if (str[i] == '-') {
        result.sign = '-';
        i++;
    }

    while (str[i] == '0') {
        i++;
    }

    j = i;
    while (j < strlen(str)) {
        if (!isdigit(str[j])) {
            printf( "Invalid input: %s\n", str);
            exit(1);
        }
        j++;
    }

    j = 0;
    while (j < strlen(str) - i) {
        if (result.length >= Max) {
            printf("Error: Number exceeds maximum size limit (%d digits)\n", Max);
            exit(1);
        }
        result.num[result.length] = str[strlen(str) - j - 1] - '0';
        j++;
        result.length++;
    }

    if (result.length == 0) {
        result.length = 1;
        result.num[0] = 0;
        result.sign = '+';
    }

    *n = result;
    return SUCCESS;
}

int compareAbsolute(const BigInt a, const BigInt b) {
    if (a.length != b.length) return a.length - b.length;
    for (int i = a.length - 1; i >= 0; i--) {
        if (a.num[i] != b.num[i]) return a.num[i] - b.num[i];
    }
    return 0;
}
BigInt subtraction(const BigInt a, const BigInt b) {
    BigInt res = {{0}, 0, '+'};
    int borrow = 0, i = 0;

    if (a.sign != b.sign) {
        BigInt b_copy = b;
        b_copy.sign = (b.sign == '+') ? '-' : '+';
        return addition(a, b_copy);
    }

    int cmp = compareAbsolute(a, b);
    if (cmp == 0) {
        res.length = 1;
        res.num[0] = 0;
        res.sign = '+';
        return res;
    }

    BigInt larger = a, smaller = b;
    if (cmp < 0) {
        larger = b;
        smaller = a;
        res.sign = (b.sign == '+') ? '-' : '+';
    } else {
        res.sign = a.sign;
    }

    for (i = 0; i < larger.length || borrow; i++) {
        int diff = larger.num[i] - borrow;
        if (i < smaller.length) diff -= smaller.num[i];
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        res.num[i] = diff;
    }

    res.length = i;
    while (res.length > 1 && res.num[res.length - 1] == 0) {
        res.length--;
    }

    return res;
}

BigInt addition(const BigInt a, const BigInt b) {
    BigInt res = {{0}, 0, '+'};
    int carry = 0, i = 0;

    if (a.sign == b.sign) {
        for (; i < a.length || i < b.length || carry; i++) {
            if (i >= Max) {
                printf("Error: Overflow in addition\n");
                exit(1);
            }
            int sum = carry;
            if (i < a.length) sum += a.num[i];
            if (i < b.length) sum += b.num[i];
            res.num[i] = sum % 10;
            carry = sum / 10;
        }
        res.length = i;
        res.sign = a.sign;
    } else {
        BigInt b_copy = b;
        b_copy.sign = '+';
        res = (a.sign == '+') ? subtraction(a, b_copy) : subtraction(b, a);
    }

    return res;
}



BigInt multiplication(const BigInt a, const BigInt b) {
    BigInt res = {{0}, 0, '+'};
    int i, j;

    if (a.length + b.length > Max) {
        printf("Error: Overflow in multiplication\n");
        exit(1);
    }

    for (i = 0; i < a.length; i++) {
        int carry = 0;
        for (j = 0; j < b.length || carry; j++) {
            if (i + j >= Max) {
                printf("Error: Overflow in multiplication\n");
                exit(1);
            }
            int product = res.num[i + j] + carry;
            if (j < b.length) product += a.num[i] * b.num[j];
            res.num[i + j] = product % 10;
            carry = product / 10;
        }
    }

    res.length = i + j - 1;
    while (res.length > 1 && res.num[res.length - 1] == 0) {
        res.length--;
    }

    res.sign = (a.sign == b.sign) ? '+' : '-';
    return res;
}

void printBigInt(const BigInt n) {
    if (n.sign == '-') printf("-");
    if (n.length == 0 || (n.length == 1 && n.num[0] == 0)) {
        printf("0");
        return;
    }
    for (int i = n.length - 1; i >= 0; i--) {
        printf("%d", n.num[i]);
    }
    
}

int main() {
    char a[Max], b[Max];
    Status_Code sc;
    BigInt n1, n2;
    int n=1;
    while(n){
    
    printf("Enter the first number: ");
    scanf("%s", a);
    printf("Enter the second number: ");
    scanf("%s", b);

    sc = initialise(a, &n1);
    sc = initialise(b, &n2);

    printf("First number: ");
    printBigInt(n1);
    printf("\n");
    printf("Second number: ");
    printBigInt(n2);
    printf("\n");

    BigInt sum = addition(n1, n2);
    printf("Sum: ");
    printBigInt(sum);
    printf("\n");

    BigInt diff = subtraction(n1, n2);
    printf("Difference: ");
    printBigInt(diff);
    printf("\n");

    BigInt prod = multiplication(n1, n2);
    printf("Product: ");
    printBigInt(prod);
    printf("\n");
    printf("enter 1 to continue or 0 to exit:");
    scanf("%d",&n);
    }

    return 0;
}
