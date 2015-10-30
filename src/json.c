
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <json.h>
#include <ge.h>

struct fab_json;

typedef void (*fab_json_state)(struct fab_json *, uint8_t);


struct fab_json {
    void (*callback)(const struct fab_json_token *, void *);
    void *callback_data;

    fab_json_state state;
};

/* states */
void fab_json_state_start(struct fab_json *json, uint8_t c);
void fab_json_state_null1(struct fab_json *json, uint8_t c);
void fab_json_state_null2(struct fab_json *json, uint8_t c);
void fab_json_state_null3(struct fab_json *json, uint8_t c);

fab_json_t fab_json_create(void (*callback)(const struct fab_json_token *, void *), void *data)
{
    struct fab_json *json;

    FAB_CGE_NULL(json = malloc(sizeof(struct fab_json)));
    
    json->callback = callback;
    json->callback_data = data;
    json->state = fab_json_state_start;

    return json;
error:
    return NULL;
}

void fab_json_state_start(struct fab_json *json, uint8_t c)
{
    switch(c) {
        case 'n':
            json->state = fab_json_state_null1;
            break;
        default:
            break;
    }
}

void fab_json_state_null1(struct fab_json *json, uint8_t c)
{
    switch(c) {
        case 'u':
            json->state = fab_json_state_null2;
            break;
        default:
            break;
    }
}

void fab_json_state_null2(struct fab_json *json, uint8_t c)
{
    switch(c) {
        case 'l':
            json->state = fab_json_state_null3;
            break;
        default:
            break;
    }
}

void fab_json_state_null3(struct fab_json *json, uint8_t c)
{
    struct fab_json_token token;
    switch(c) {
        case 'l':
            json->state = fab_json_state_start;
            token.type = FAB_JSON_NULL;
            json->callback(&token, json->callback_data);
            break;
        default:
            break;
    }
}

void fab_json_process(fab_json_t fab_json, unicode_t c)
{
    struct fab_json *data = fab_json;
    data->state(data, c);
}

void fab_json_destroy(fab_json_t fab_json)
{
    free(fab_json);
}
