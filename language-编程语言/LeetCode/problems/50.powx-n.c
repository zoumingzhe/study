double myPow(double x, int n) {
    if (0 == n) {
        return 1.0;
    }

    if (n < 0) {
        return 1 / (x * myPow(x, -(n + 1)));
    }

    if (0 == n % 2) {
        //偶数
        return myPow(x * x, n / 2);
    } else {
        //奇数
        return x * myPow(x * x, (n - 1) / 2);
    }
}
