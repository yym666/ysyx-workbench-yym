#include <memory/paddr.h>
#include <common.h>
#include <device/map.h>

#ifdef CONFIG_DTRACE
void dtrace_read(paddr_t addr, int len, IOMap *map) {
	printf("dtrace: read %12s at " FMT_PADDR ",%d\n", map->name, addr, len);
}

void dtrace_write(paddr_t addr, int len, word_t data, IOMap *map) {
	printf("dtrace: write %12s at " FMT_PADDR ",%d with " FMT_WORD "\n", map->name, addr, len, data);
}
#else
void dtrace_read(paddr_t addr, int len, IOMap *map) {}
void dtrace_write(paddr_t addr, int len, word_t data, IOMap *map) {}
#endif