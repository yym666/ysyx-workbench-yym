#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  char out[1024];
  int len = vsprintf(out, fmt, ap);
  for (int i = 0; i < len; ++i)
    putch(out[i]);
  va_end(ap);
  return len;
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return vsnprintf(out, -1, fmt, ap);
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int len = vsprintf(out, fmt, ap);
  va_end(ap);
  return len;
  panic("Not implemented");
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  int pos = 0, flags, width, precise;
  for (; *fmt != '\0'; ++fmt){
    while (*fmt != '%' && *fmt != '\0'){
      out[pos++] = *fmt, fmt++;
      if (pos > n) return n;
    }
    if (*fmt == '\0') break;
    if (*fmt != '%') assert(0);
    fmt++;

    flags = ' ';
    if (*fmt == '0' || *fmt == '-' || *fmt == '+') 
      flags = *fmt, fmt++;
    
    width = 0;
    while (*fmt >= '0' && *fmt <= '9'){
      width = width * 10 + *fmt - '0';
      fmt++;
    }
    // if (flags != ' ') putch('');

    precise = 0;
    if (*fmt == '.'){
      fmt++;
      while (*fmt >= '0' && *fmt <= '9'){
        precise = precise * 10 + *fmt - '0';
        fmt++;
      }
    }
    else precise = 1;
    char specifier = *fmt;
    switch (specifier){
      case 's':{ 
        char* cur = va_arg(ap, char*);
        while (*cur != '\0'){
          out[pos++] = *cur++;
          if (pos > n) return n;
        } 
        break;
      }
      case 'c':{ 
        char cur = va_arg(ap, int);
        out[pos++] = cur;
        break;
      }
      case 'd': case 'i': case 'l': {
        int cur = va_arg(ap, int);
        int sig = cur >= 0 ? 1 : -1;
        cur = cur * sig;

        char num[25];
        int len = 0;
        do{
          num[len++] = (char)((cur % 10) + '0');
          cur /= 10;
        }while (cur > 0);

        if (sig < 0) num[len++] = '-';
        else if (flags == '+') num[len++] = '+';
        
        if (flags == '-') {
          for (int i = 0; i < len; ++i){
            out[pos++] = num[len - i - 1];
            if (pos > n) return n;
          }
          for (int i = 0; i < width - len; ++i){
            out[pos++] = flags;
            if (pos > n) return n;
          }
        }
        else{
          for (int i = 0; i < width - len; ++i){
            out[pos++] = flags;
            if (pos > n) return n;
          }
          for (int i = 0; i < len; ++i){
            out[pos++] = num[len - i - 1];
            if (pos > n) return n;
          }
        }
        break;
      }
      case 'x': case 'p': {
         unsigned long d = va_arg(ap, unsigned long);
          char num[20] = {0};
          int rem = 0;
          int len = 0;

          do {
            rem = d % 16;
            d = d / 16;
            if (rem <= 9) num[len++] = rem + '0';
            else num[len++] = rem - 10 + 'a';
            // putch(num[len-1]);
          } while (d > 0);

        if (flags == '+') assert(0);

        if (flags == '-') {
          for (int i = 0; i < len; ++i){
            out[pos++] = num[len - i - 1];
            if (pos > n) return n;
          }
          for (int i = 0; i < width - len; ++i){
            out[pos++] = flags;
            if (pos > n) return n;
          }
        }
        else{
          for (int i = 0; i < width - len; ++i){
            out[pos++] = flags;
            if (pos > n) return n;
          }
          for (int i = 0; i < len; ++i){
            out[pos++] = num[len - i - 1];
            if (pos > n) return n;
          }
        }
        break;
      }
      default: putch(specifier); assert(0); break;
    }
  }
  out[pos] = '\0';
  return pos;
  panic("Not implemented");
}

#endif
