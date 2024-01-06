int linear(int t, int b, int c) {
    return (c*t)/100 + b;
}

int easeInQuad(int t, int b, int c) {
    t = (t * t) / 100;
    return (c*t)/100 + b;
}

int easeOutQuad(int t, int b, int c) {
    t = 100 - t;
    t = (t * t) / 100;
    return c - (c * t) / 100 + b;
}

int easeInOutQuad(int t, int b, int c) {
    if (t < 50) return easeInQuad(t*2, b, c/2);
    return easeOutQuad(t*2 - 100, b + c/2, c/2);
}