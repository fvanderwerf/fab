
#include <utf8.h> 

#include <ge.h>

#include <stdlib.h>

struct fab_utf8 {
    void (*callback)(uint32_t, void *);
    void *data;
};

fab_utf8_t fab_utf8_create(void (*callback)(uint32_t, void *), void *data)
{
    struct fab_utf8 *utf8_data = NULL;
   
    FAB_CGE_NULL(utf8_data = malloc(sizeof(struct fab_utf8)));

    utf8_data->callback = callback;
    utf8_data->data = data;

    return utf8_data;
error:
    return NULL;
}

int fab_utf8_process(fab_utf8_t utf8, uint8_t c)
{
    struct fab_utf8 *utf8_data = utf8;

    FAB_CGE_ERRNO(c & 0x80, EINVAL);

    utf8_data->callback(c, utf8_data->data);

    return 0;
error:
    return 1;
}

void fab_utf8_destroy(fab_utf8_t utf8)
{
    free(utf8);
}



