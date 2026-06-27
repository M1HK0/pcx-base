class CFormatter {
    string buf;

    CFormatter(const string &in fmt) {buf = fmt;}
    
    CFormatter@ replace(const string &in token, const string &in val) {
        int p = buf.findFirst(token);
        if (p >= 0)
            buf = buf.substr(0, p) + val + buf.substr(p + token.length());
        return this;
    }

    CFormatter@ s(const string &in val) {return replace("%s", val);}
    CFormatter@ d(int val) {return replace("%d", formatInt(val));}
    CFormatter@ u(int val) {return replace("%u", formatUInt(val));}
    CFormatter@ x(int val) {return replace("%x", "0x" + formatInt(val, "H"));}
    CFormatter@ f(float val, int precision = 2) {return replace("%f", formatFloat(val, "", 0, precision));}

    string str() {return buf;}
}

namespace string {
    CFormatter format(const string &in fmt) {
        return CFormatter(fmt);
    }
}