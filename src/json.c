
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

    double number;
};

/* states */
void fab_json_state_start(struct fab_json *json, uint8_t c);
void fab_json_state_null1(struct fab_json *json, uint8_t c);
void fab_json_state_null2(struct fab_json *json, uint8_t c);
void fab_json_state_null3(struct fab_json *json, uint8_t c);

void fab_json_state_false1(struct fab_json *json, uint8_t c);
void fab_json_state_false2(struct fab_json *json, uint8_t c);
void fab_json_state_false3(struct fab_json *json, uint8_t c);
void fab_json_state_false4(struct fab_json *json, uint8_t c);

void fab_json_state_true1(struct fab_json *json, uint8_t c);
void fab_json_state_true2(struct fab_json *json, uint8_t c);
void fab_json_state_true3(struct fab_json *json, uint8_t c);

void fab_json_state_number(struct fab_json *json, uint8_t c);


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
    struct fab_json_token token;
    switch(c) {
        case 'n':
            json->state = fab_json_state_null1;
            break;
        case 'f':
            json->state = fab_json_state_false1;
            break;
        case 't':
            json->state = fab_json_state_true1;
            break;
        case '[':
            token.type = FAB_JSON_ARRAY_START;
            json->callback(&token, json->callback_data);
            break;
        case ']':
            token.type = FAB_JSON_ARRAY_END;
            json->callback(&token, json->callback_data);
            break;
        case ',':
            token.type = FAB_JSON_ENTRY_SEP;
            json->callback(&token, json->callback_data);
            break;
        case '{':
            token.type = FAB_JSON_OBJECT_START;
            json->callback(&token, json->callback_data);
            break;
        case '}':
            token.type = FAB_JSON_OBJECT_END;
            json->callback(&token, json->callback_data);
            break;
        case ':':
            token.type = FAB_JSON_KEYVAL_SEP;
            json->callback(&token, json->callback_data);
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            json->number = c - '0';
            json->state = fab_json_state_number;
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

void fab_json_state_false1(struct fab_json *json, uint8_t c)
{
    switch(c) {
        case 'a':
            json->state = fab_json_state_false2;
            break;
        default:
            break;
    }
}

void fab_json_state_false2(struct fab_json *json, uint8_t c)
{
    switch(c) {
        case 'l':
            json->state = fab_json_state_false3;
            break;
        default:
            break;
    }
}

void fab_json_state_false3(struct fab_json *json, uint8_t c)
{
    switch(c) {
        case 's':
            json->state = fab_json_state_false4;
            break;
        default:
            break;
    }
}


void fab_json_state_false4(struct fab_json *json, uint8_t c)
{
    struct fab_json_token token;
    switch(c) {
        case 'e':
            json->state = fab_json_state_start;
            token.type = FAB_JSON_FALSE;
            json->callback(&token, json->callback_data);
            break;
        default:
            break;
    }
}

void fab_json_state_true1(struct fab_json *json, uint8_t c)
{
    switch(c) {
        case 'r':
            json->state = fab_json_state_true2;
            break;
        default:
            break;
    }
}

void fab_json_state_true2(struct fab_json *json, uint8_t c)
{
    switch(c) {
        case 'u':
            json->state = fab_json_state_true3;
            break;
        default:
            break;
    }
}

void fab_json_state_true3(struct fab_json *json, uint8_t c)
{
    struct fab_json_token token;
    switch(c) {
        case 'e':
            json->state = fab_json_state_start;
            token.type = FAB_JSON_TRUE;
            json->callback(&token, json->callback_data);
            break;
        default:
            break;
    }
}


void fab_json_state_number(struct fab_json *json, uint8_t c)
{
    struct fab_json_token token;

    switch (c) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            json->number *= 10;
            json->number += c - '0';
            break;
        case ' ':
        case '\t':
        case '\x0a':
        case '\x0d':
            json->state = fab_json_state_start;
            token.type = FAB_JSON_NUMBER;
            token.value.number = json->number;
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
