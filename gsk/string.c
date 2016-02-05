/* STRING.C
 * eduarcastrillo@gmail.com
 */
#include "common.h"
#include "string.h"

#define NODE_SZ sizeof(mem_node_t)

const uint64 mem_start = 0x100000;
const uint64 mem_end = 0x500000;

struct __mem_node_t {
  struct __mem_node_t *prev;
  struct __mem_node_t *next;
  uint64 length;
  void *ptr;
};

typedef struct __mem_node_t mem_node_t;

mem_node_t *root = (mem_node_t *)mem_start;
mem_node_t *tail = (mem_node_t *)mem_end;

void init_mem() {

  root->prev = NULL;
  root->length = 0;
  root->ptr = NULL;
  root->next = tail;
}

void *malloc(int nbytes) {
  
  if (nbytes < 0) {
    return NULL;
  }
  mem_node_t *curr = root;
  while (curr != tail) {
      // memory to asign
    uint32 needed = NODE_SZ+nbytes;
      // target start position for the new memory segment
    uint32 start_pos = curr+NODE_SZ+curr->length;
      
    if (start_pos+needed < curr->next) {
      mem_node_t *new_mem = (mem_node_t *)start_pos;
      new_mem->length = nbytes;
      new_mem->prev = curr;
      new_mem->next = curr->next;
      new_mem->ptr = (void *)(new_mem+NODE_SZ);
      curr->next = new_mem;
      return new_mem->ptr;
    }
  }
  return NULL;
}

void free(void *ptr) {
  
  mem_node_t *to_free = (mem_node_t *)(ptr-NODE_SZ);
  to_free->prev->next = to_free->next;
  to_free->next->prev = to_free->prev;
}

void memcpy(void *dest, void *src, int n) {

  uint8 *d = dest;
  uint8 *s = src;
  while (n--) *d++ = *s++;
}

void memset(void *ptr, int v, int n) {

  uint8 *p = ptr;
  while (n--) *p++ = v;
}

void memsetw(void *ptr, int v, int n) {

  uint16 *p = ptr;
  while (n--) *p++ = v;
}

void strcpy(char *dest, const char *src) {

  while (*dest++ = *src++);
}

void strcat(char *dest, const char *src) {

  dest += strlen(dest);
  while (*dest++ = *src++);
}

void substr(char *dest, const char *src, int s, int e) {

  int k = 0;
  int i = s;
  for (;i <= e; ++i, ++k) dest[k] = src[i];
  dest[k] = '\0';
}

int strlen(const char *str) {

  int len = -1;
  while (str[++len]);
  return len;
}

int strncmp(const char *s1, const char *s2, int n) {

  while (n--) {
    if (*s1 != *s2) return *s1 - *s2;
    else if(!*s1)  return 0;
    ++s1;
    ++s2;
  }
  return 0;
}

