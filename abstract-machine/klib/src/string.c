#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  int cnt = 0;
  while (1) {
    if (s[cnt] == '\0') return cnt;
    ++cnt;
  }
  panic("Not implemented");
}

char *strcpy(char *dst, const char *src) {
  int cnt = 0;
  while (1){
    dst[cnt] = src[cnt];
    if (src[cnt] == '\0')
      return dst;
    ++cnt;
  }
  panic("Not implemented");
}

char *strncpy(char *dst, const char *src, size_t n) {
  int i;
  for (i = 0; i < n && src[i] != '\0'; ++i)
    dst[i] = src[i];
  for ( ; i < n; ++i)
    dst[i] = '\0';
  return dst;
  panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
    char *ret = dst;
    while (*dst) dst++;
    while ((*dst++ = *src++) != '\0');
    return ret;
//mine
  int cnt = 0, len = 0;
  while (1) {
    if (dst[len] == '\0')
      break;
    ++len;
  }
  while (1) {
    dst[len + cnt] = src[cnt];
    if (src[cnt] == '\0')
      break;
    ++cnt;
  }
  return dst;
  panic("Not implemented");
}

int strcmp(const char *s1, const char *s2) {
  int l1 = strlen(s1), l2 = strlen(s2);
  if (l1 > l2) return 1;
  if (l1 < l2) return -1;
  for (int i = 0; i < l1; ++i){
    if (s1[i] != s2[i]){
      if (s1[i] > s2[i]) return 1;
      else return -1;
    }
  }
  return 0;
  panic("Not implemented");
}

int strncmp(const char *s1, const char *s2, size_t n) {
  int l1 = strlen(s1), l2 = strlen(s2);
  if (l1 > n) l1 = n;
  if (l2 > n) l2 = n;
  if (l1 > l2) return 1;
  if (l1 < l2) return -1;
  for (int i = 0; i < l1; ++i){
    if (s1[i] != s2[i]){
      if (s1[i] > s2[i]) return 1;
      else return -1;
    }
  }
  return 0;
  
  panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
  void* ret = s;
  while (n--){
    *(char*)s = (char)c;
    s++;
  }
  return ret;
  panic("Not implemented");
}

void *memmove(void *dst, const void *src, size_t n) {
  
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  char* dst = (char*)out;
  const char* src = (const char*)in;
  for (int i = 0; i < n; ++i){
    dst[i] = src[i];
  }
  panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
  int res = 0;
  char* dst = (char* )s1;
  const char* src = (const char*)s2;
  for (int i = 0; i < n; ++i){
    res = dst[i] - src[i];
    if (res != 0){
      break;
    }
  }
  return res;
  panic("Not implemented");
}

#endif
