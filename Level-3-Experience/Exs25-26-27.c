#define PROMOTE(XT, A)                              \
_Generic(0+(XT)+0,                                  \
         signed: (signed)A,                         \
         unsigned: (unsigned)A,                     \
         long: (long)A,                             \
         unsigned long: (unsigned long)A,           \
         long long: (long long)A,                   \
         unsigned long long: (unsigned long long)A)

#define SIGNEDNESS(XT) (PROMOTE(XT, -1) < PROMOTE(XT, 0))

signed (maxs)(signed a, signed b); 
unsigned (maxu)(unsigned a, unsigned b);
unsigned (maxus)(unsigned a, signed b);
unsigned (maxsu)(signed a, unsigned b);
signed long (maxsl)(signed long a, signed long b);
unsigned long (maxul)(unsigned long a, unsigned long b);
unsigned long (maxusl)(unsigned long a, signed long b);
unsigned long (maxsul)(signed long a, unsigned long b);
signed long long (maxsll)(signed long long a, signed long long b);
unsigned long long (maxull)(unsigned long long a, unsigned long long b);
unsigned long long (maxusll)(unsigned long long a, signed long long b);
unsigned long long (maxsull)(signed long long a, unsigned long long b);

inline
signed maxs(signed a, signed b) {
  return a < b ? b : a;
}

inline
unsigned maxu(unsigned a, unsigned b) {
  return a < b ? b : a;
}

inline
unsigned maxus(unsigned a, signed b) {
  return b < 0 ? a : maxu(a, b);
}

inline
unsigned maxsu(signed a, unsigned b) {
  return a < 0 ? b : maxu(a, b);
}

inline
signed long maxsl(signed long a, signed long b) {
  return a < b ? b : a;
}

inline
unsigned long maxul(unsigned long a, unsigned long b) {
  return a < b ? b : a;
}

inline
unsigned long maxusl(unsigned long a, signed long b) {
  return b < 0 ? a : maxul(a, b);
}

inline
unsigned long maxsul(signed long a, unsigned long b) {
  return a < 0 ? b : maxul(a, b);
}

inline
signed long long maxsll(signed long long a, signed long long b) {
  return a < b ? b : a;
}

inline
unsigned long long maxull(unsigned long long a, unsigned long long b) {
  return a < b ? b : a;
}

inline
unsigned long long maxusll(unsigned long long a, signed long long b) {
  return b < 0 ? a : maxull(a, b);
}

inline
unsigned long long maxsull(signed long long a, unsigned long long b) {
  return a < 0 ? b : maxull(a, b);
}

#define  MAX(X, Y)                              \
_Generic((X)+(Y),                               \
         signed: maxs,                          \
         unsigned: maxu,                        \
         signed long: maxsl,                    \
         unsigned long: maxul,                  \
         signed long long: maxsll,              \
         unsigned long long: maxull)((X), (Y))

#define MAXUS(X, Y)                             \
_Generic((X)+(Y),                               \
         signed: maxus,                         \
         unsigned: maxus,                       \
         signed long: maxusl,                   \
         unsigned long: maxusl,                 \
         signed long long: maxusll,             \
         unsigned long long: maxusll)((X), (Y))

#define MAXSU(X, Y)                             \
_Generic((X)+(Y),                               \
         signed: maxsu,                         \
         unsigned: maxsu,                       \
         signed long: maxsul,                   \
         unsigned long: maxsul,                 \
         signed long long: maxsull,             \
         unsigned long long: maxsull)((X), (Y))

#define mix(X, Y)                                       \
_Generic(&(char[SIGNEDNESS(X)-SIGNEDNESS(Y)+2]){ },     \
         char(*)[1]: MAXUS((X), (Y)),                   \
         char(*)[2]: MAX((X), (Y)),                     \
         char(*)[3]: MAXSU((X), (Y)))

int main(void) {
  long val = PROMOTE(1000L, 5);
  long val2 = PROMOTE(long, 5);
  unsigned val3 = PROMOTE(short, 5);

  bool val4 = SIGNEDNESS(val);

	long val5 = mix(-1, 2);
  
  return 0;
}
