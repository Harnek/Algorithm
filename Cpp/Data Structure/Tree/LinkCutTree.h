/**
 * 木のパスへの捜査なら最強と名高いLink-Cut Tree
 *
 * 機能としては、link、cut、evert、rootを実装済み
 * 辺に値を持たせたい場合は頂点倍加を推奨する
 * コード長はおよそ3kで、ICPCでの使用も十分可能
 */
struct LCNode {
    typedef LCNode* NP;

    typedef int D;
    struct Node {
        D d, sm;
    } n;
    LCNode() : l(nullptr), r(nullptr), sz(0) {}
    LCNode(D v) : p(nullptr), l(last), r(last), sz(1) {
        n.d = v;
        n.sm = v;
    }

    void revdata() {
        assert(this != last);
        swap(l, r);
        rev ^= true;
    }
    NP update() {
        assert(this != last);
        sz = 1+l->sz+r->sz;
        n.sm = n.d;
        if (l->sz) {
            n.sm += l->n.sm;
        }
        if (r->sz) {
            n.sm += r->n.sm;
        }
        return this;
    }

    D get() {
        expose();
        return n.sm;
    }
    void set(D d) {
        expose();
        n.d = d;
        update();
    }
    
    //ここからテンプレ
    static LCNode last_d;
    static NP last;
    NP p, l, r;
    int sz;
    bool rev;
    inline int pos() {
        if (p) {
            if (p->l == this) return -1;
            if (p->r == this) return 1;
        }
        return 0;
    }
    void rot() {
        NP qq = p->p;
        int pps = p->pos();
        if (p->l == this) {
            p->l = r; r->p = p;
            r = p; p->p = this;
        } else {
            p->r = l; l->p = p;
            l = p; p->p = this;
        }
        p->update(); update();
        p = qq;
        if (!pps) return;
        if (pps == -1) {
            qq->l = this;
        } else {
            qq->r = this;
        }
        qq->update();
    }
    void splay() {
        assert(this != last);
        supush();
        int ps;
        while ((ps = pos())) {
            int pps = p->pos();
            if (!pps) {
                rot();
            } else if (ps == pps) {
                p->rot(); rot();
            } else {
                rot(); rot();
            }
        }
    }
    void expose() {
        assert(this != last);
        NP u = this, ur = last;
        do {
            u->splay();
            u->r = ur;
            u->update();
            ur = u;
        } while ((u = u->p));
        splay();
    }
    /**
     * splayする前に一括でその頂点のパス木の根までをpushする
     * 唯一stack overflowしうる関数なので注意すること
     */
    void supush() {
        if (pos()) {
            p->supush();
        }
        push();
    }
    void push() {
        int lsz = l->sz, rsz = r->sz;
        if (rev) {
            if (lsz) {
                l->revdata();
            }
            if (rsz) {
                r->revdata();
            }
            rev = false;
        }
    }

    void link(NP r) {
        assert(this != r);
        expose();
        r->expose();
        assert(l == last);
        p = r;
    }

    void cut() {
        expose();
        l->p = NULL;
        l = last;
        update();
    }
    NP root() {
        expose();
        NP u = this;
        while (u->l != last) {
            u = u->l;
        }
        u->splay();
        return u;
    }

    void evert() {
        expose();
        revdata();
    }
};
LCNode LCNode::last_d = LCNode();
LCNode::NP LCNode::last = &last_d;
