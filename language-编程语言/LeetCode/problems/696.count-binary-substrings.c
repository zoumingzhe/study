#define min(a,b) (a)<(b)?(a):(b)
#define flag s[off]

int countBinarySubstrings(char * s){
    int stat1 = 0;
    int stat2 = 0;
    int off = 0;
    int len = 0;
    int ret = 0;

    while (flag != '\0')
    {
        len = 0;

        for (int i = 0;  s[off+i] != '\0'; i++)
        {
            if (s[off+i] == flag)
            {
                len++;
            }  
            else
            {
                break;
            };  
        }
        
        stat2 = stat1;
        stat1 = len;
        off += len;
        ret += min(stat1, stat2);
    }

    return ret;
}