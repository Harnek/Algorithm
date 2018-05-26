using R = long double;
using P = complex<R>;

const R EPS = 1e-10;
const R PI = acos(R(-1));

int sgn(R a) {
    if (abs(a) <= EPS) return 0;
    return (a < 0) ? -1 : 1;
}

int sgn(R a, R b) {
    return sgn(b-a);
}

bool near(P a, P b) {
    return !sgn(abs(a-b));
}

bool lessP(P l, P r) {
    if (sgn(l.real(), r.real())) return l.real() < r.real();
    if (sgn(l.imag(), r.imag())) return l.imag() < r.imag();
    return false;
}

R cross(P a, P b) { return a.real()*b.imag() - a.imag()*b.real(); }
R dot(P a, P b) { return a.real()*b.real() + a.imag()*b.imag(); }

/*
  1->cclock
 -1->clock
  0->on
  2->back
 -2->front
 */
int ccw(P a, P b, P c) {
    assert(!near(a, b));
    if (near(a, c) || near(b, c)) return 0;
    int s = sgn(cross(b-a, c-a));
    if (s) return s;
    if (dot(b-a, c-a) < 0) return 2;
    if (dot(a-b, c-b) < 0) return -2;
    return 0;
}

struct L {
    P x, y;
    L() {}
    L(P x, P y) :x(x), y(y) {}
    P vec() { return y-x; }
    R abs() { return abs(vec()); }
    R arg() { return arg(vec()); }
};

//度をラジアンに
R deg2rad(R x) {
    return x/180*PI;
}

//ラジアンを度に
R rad2deg(R x) {
    return x/PI*180;
}

//角度を[0, 2*PI)に
R radNorP(R x) {
    return fmod(fmod(x, 2*PI) + 2*PI, 2*PI);
}

//角度を[-PI, PI)に
R radNorN(R x) {
    x = radNorP(x);
    if (x >= PI) x -= 2*PI;
    return x;
}

/**
 * radianで、xが[l, r]に入っているかを判別する
 * 0:OFF
 * 1:IN
 * 2:ON
 */
bool inR(R l, R r, R x) {
    l = radNorP(l);
    r = radNorP(r);
    x = radNorP(x);
    if (!sgn(l, x) || !sgn(r, x)) return 2;
    if (!sgn(l, r)) return 0;
    if (sgn(l, r) == 1) {
        if (sgn(l, x) == 1 && sgn(x, r) == 1) return 1;
    } else {
        if (sgn(x, r) == 1 || sgn(l, x) == 1) return 1;
    }
    return 0;
}

R ssqrt(R d) {
    d = max<R>(0, d);
    return sqrt(d);
}

R sacos(R d) {
    d = max<R>(-1, d);
    d = min<R>(1, d);
    return acos(d);
}

