char * intToRoman(int num){
    int i;
    int len = 0;
    char *buff = (char*)malloc(50);

    while  (num) {
        while  (num / 1000) {
            buff[len++] = 'M';
            num -= 1000;
        }

        while (num / 900) {
            buff[len++] = 'C';
            buff[len++] = 'M';
            num -= 900;
        }

        while (num / 500) {
            buff[len++] = 'D';
            num -= 500;
        }

        while (num / 400) {
            buff[len++] = 'C';
            buff[len++] = 'D';
            num -= 400;
        }

        while (num / 100) {
            buff[len++] = 'C';
            num -= 100;
        }

        while (num / 90) {
            buff[len++] = 'X';
            buff[len++] = 'C';
            num -= 90;
        }

        while (num / 50) {
            buff[len++] = 'L';
            num -= 50;
        }

        while (num / 40) {
            buff[len++] = 'X';
            buff[len++] = 'L';
            num -= 40;
        }

        while (num / 10) {
            buff[len++] = 'X';
            num -= 10;
        }

        while (num / 9) {
            buff[len++] = 'I';
            buff[len++] = 'X';
            num -= 9;
        }

        while (num / 5) {
            buff[len++] = 'V';
            num -= 5;
        }

        while (num / 4) {
            buff[len++] = 'I';
            buff[len++] = 'V';
            num -= 4;
        }

        while (num ) {
            buff[len++] = 'I';
            num--;
        }
    }

    buff[len++] = '\0';
    return buff;
}
