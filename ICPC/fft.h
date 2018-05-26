using R = double;
const R PI = 4*atan(R(1));
using Pc = complex<R>;

void fft(bool type, V<Pc> &c) {
    int N = int(c.size());
    if (N <= 1) return;
    int s = 0;
    while ((1<<s) < N) s++;
    assert(1<<s == N);

    static V<Pc> ep[30];
    if (!ep[s].size()) {
        for (int i = 0; i < N; i++) {
            ep[s].push_back(polar<R>(1, i*2*PI/N));
        }
    }
    V<Pc> a = c, b(N);
    for (int i = 1; i <= s; i++) {
        int W = 1<<(s-i);
        for (int y = 0; y < N/2; y += W) {
            Pc now = ep[s][y]; if (type) now = conj(now);
            for (int x = 0; x < W; x++) {
                auto l = a[y<<1 | x];
                auto u = now, v = a[y<<1 | x | W];
                auto r = Pc(u.real() * v.real() - u.imag() * v.imag(),
                    u.real() * v.imag() + u.imag() * v.real());
                b[y | x]        = l+r;
                b[y | x | N>>1] = l-r;
            }
        }
        swap(a, b);
    }
    c = a;
}

V<Pc> multiply(const V<Pc> &a, const V<Pc> &b) {
    auto A = a.size(), B = b.size();
    assert(A && B);
    int lg = 0;
    while ((1<<lg) < A+B-1) lg++;
    int N = 1<<lg;
    V<Pc> ac(N, Pc(0, 0)), bc(N, Pc(0, 0));
    for (int i = 0; i < A; i++) ac[i] = a[i];
    for (int i = 0; i < B; i++) bc[i] = b[i];
    fft(false, ac);
    fft(false, bc);
    for (int i = 0; i < N; i++) {
        ac[i] *= bc[i];
    }
    fft(true, ac);
    V<Pc> c(A+B-1);
    for (int i = 0; i < A+B-1; i++) {
        c[i] = ac[i] / R(N);
    }
    return c;
}

V<R> multiply_fast(const V<R> &a, const V<R> &b) {
    auto A = a.size(), B = b.size();
    assert(A && B);
    int lg = 0;
    while ((1<<lg) < A+B-1) lg++;
    int N = 1<<lg;
    V<Pc> d(N);
    for (int i = 0; i < N; i++) d[i] = Pc(i<A ? a[i] : 0, i<B ? b[i] : 0);
    fft(false, d);
    for (int i = 0; i < N/2+1; i++) {
        auto j = i ? (N-i) : 0;
        Pc x = Pc(d[i].real()+d[j].real(), d[i].imag()-d[j].imag());
        Pc y = Pc(d[i].imag()+d[j].imag(), -d[i].real()+d[j].real());
        d[i] = x * y / R(4);
        if (i != j) d[j] = conj(d[i]);
    }
    fft(true, d);
    V<R> c(A+B-1);
    for (int i = 0; i < A+B-1; i++) {
        c[i] = d[i].real() / N;
    }
    return c;
}

template<int B, uint MD>
void nft(bool type, V<ModInt<MD>> &c) {
    using Mint = ModInt<MD>;
    int N = int(c.size());
    int s = bsr(uint(N));
    assert(1<<s == N);
    V<Mint> a = c, b(N);
    for (int i = 1; i <= s; i++) {
        int W = 1<<(s-i); //変更後の幅W
        Mint base = pow(Mint(B), (MD-1)/(1<<i));
        if (type) base = Mint::inv(base);
        Mint now = 1;
        for (int y = 0; y < N/2; y += W) {
            for (int x = 0; x < W; x++) {
                auto l =       a[y<<1 | x];
                auto r = now * a[y<<1 | x | W];
                b[y | x]        = l+r;
                b[y | x | N>>1] = l-r;
            }
            now *= base;
        }
        swap(a, b);
    }
    c = a;
}