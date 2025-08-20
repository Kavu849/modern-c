#include "rationals.h"

size_t gcd(size_t a, size_t b) [[__unsequenced__]];
size_t gcd2(size_t a, size_t b);

rat rat_get(signed sign, size_t num, size_t denom) [[__unsequenced__]] {
  rat ret = {
    .sign = (sign < 0),
    .num = num,
    .denom = denom,
  };
  return ret;
}

rat rat_get_normal(rat x) [[__unsequenced__]] {
  size_t c = gcd(x.num, x.denom);
  x.num /= c;
  x.denom /= c;
  return x;
}

rat rat_get_extended(rat x, size_t f) [[__unsequenced__]] {
  x.num *= f;
  x.denom *= f;
  return x;
}

rat rat_get_prod(rat x, rat y) [[__unsequenced__]] {
  rat ret = {
    .sign = (x.sign != y.sign),
    .num = x.num * y.num,
    .denom = x.denom * y.denom,
  };
  return rat_get_normal(ret);
}

rat rat_get_prod2(rat x, rat y) [[__unsequenced__]] {
  size_t tmp = x.denom;
  x.denom = y.denom;
  y.denom = tmp;
  x = rat_get_normal(x);
  y = rat_get_normal(y);
  rat ret = {
    .sign = (x.sign != y.sign),
    .num = x.num * y.num,
    .denom = x.denom * y.denom,
  };
  return ret;
}

rat rat_get_sum(rat x, rat y) [[__unsequenced__]] {
  size_t c = gcd(x.denom, y.denom);
  size_t ax = y.denom / c;
  size_t bx = x.denom / c;
  x = rat_get_extended(x, ax);
  y = rat_get_extended(y, bx);
  assert(x.denom == y.denom);

  if (x.sign == y.sign) {
    x.num += y.num;
  } else if (x.num > y.num) {
    x.num -= y.num;
  } else {
    x.num = y.num - x.num;
    x.sign = !x.sign;
  }
  return rat_get_normal(x);
}

void rat_destroy(rat* rp) {
  if (rp) *rp = (rat) {};
}

rat* rat_init(rat* rp, signed sign, size_t num, size_t denom) {
  if (rp) *rp = rat_get(sign, num, denom);
  return rp;
}

rat* rat_normalize(rat* rp) {
  if (rp) *rp = rat_get_normal(*rp);
  return rp;
}

rat* rat_extend(rat* rp, size_t f) {
  if (rp) *rp = rat_get_extended(*rp, f);
  return rp;
}

char const* rat_print(size_t len, char tmp[len], rat const* x) {
  if (x) {
    if (x->sign)
      snprintf(tmp, len, "-%zu/%zu", x->num, x->denom);
    else
      snprintf(tmp, len, "+%zu/%zu", x->num, x->denom);
  } else
    tmp[0] = 0;
  return tmp;
}

int main(int argc, char* argv[argc + 1]) {
  return 0;
}
