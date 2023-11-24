bool equationsPossible(char ** equations, int equationsSize){
    int  len;
    bool ret  = true;
    char sym[26][26];
    
    for (int i = 0; i < 26; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            sym[i][j] = 0;
        }
    }
    
    
    for (int i = 0; i < equationsSize; i++)
    {
        if (equations[i][1] == '=')
        {
            for (int j = 0; j < len; j++)
            {
                if (/* condition */)
                {
                    /* code */
                }
                
            }
            sym[i][j] = 0;
        }
    }
}
