int t_powi(int n, int x)
{
    int r = 1;
    while (x) {
        r *= n;
        x--;
    };

    return r;
}


