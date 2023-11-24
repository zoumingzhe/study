bool isPalindrome(int x){
    int  i, j;
    int  z = x;
    int  len = 0;
    char str[100];

    if (x > 0)
    {
        while (z)
        {
            str[len++] = z % 10;
            z /= 10;
        }
        for (i = 0, j = len-1; i < j; i++, j--)
        {
            if (str[i] != str[j])
            {
                return false;
            }
        }
        return true;
    }
    else if (x < 0)
    {
        return false;
    }
    else
    {
        return true;
    }

    return true;
}
