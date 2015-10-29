
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <json.h>
#include <ge.h>

struct fab_json {
    int x;
};

fab_json_t fab_json_create()
{
    struct fab_json *json;

    FAB_CGE_NULL(json = malloc(sizeof(struct fab_json)));

    return json;
error:
    return NULL;
}


void fab_json_destroy(fab_json_t fab_json)
{
    free(fab_json);
}
