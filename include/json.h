
#ifndef FAB_JSON_H
#define FAB_JSON_H

#include <types.h>

typedef void *fab_json_t;

struct fab_json_token {
    enum {
        FAB_JSON_NULL,
        FAB_JSON_FALSE,
        FAB_JSON_TRUE,
        FAB_JSON_ARRAY_START,
        FAB_JSON_ARRAY_SEP,
        FAB_JSON_ARRAY_END
    } type;
};

fab_json_t fab_json_create(void (*callback)(const struct fab_json_token *, void *), void *data);
void fab_json_process(fab_json_t fab_json, unicode_t c);
void fab_json_destroy(fab_json_t fab_json);


#endif /* FAB_JSON_H */
