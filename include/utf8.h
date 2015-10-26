
#ifndef FAB_UTF8_H
#define FAB_UTF8_H

#include <stdlib.h>
#include <stdint.h>

typedef void *fab_utf8_t;

fab_utf8_t fab_utf8_create(void (*callback)(uint32_t, void *), void *data);

int fab_utf8_process(fab_utf8_t fab_utf8, uint8_t c);

void fab_utf8_destroy(fab_utf8_t fab_utf8);

#endif /* FAB_UTF8_H */
