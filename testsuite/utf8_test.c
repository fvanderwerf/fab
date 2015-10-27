
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <utf8.h>
#include <stdlib.h>
#include <errno.h>

#include "fab_test.h"


struct character {
    uint32_t codepoint;
    struct character *next;
};

struct output {
    struct character *head;
    struct character *tail;
};


void test_utf8_callback(uint32_t character, void *data)
{
    struct output *output = data;

    struct character *c = malloc(sizeof(struct character));
    c->codepoint = character;
    c->next = NULL;

    if (output->head == NULL) {
        output->head = c;
        output->tail = c;
    } else {
        output->tail->next = c;
        output->tail = c;
    }
}

int test_utf8_compare(const struct character *cs, uint32_t *codepoints, int length)
{
    uint32_t *end = codepoints + length;
    while (cs != NULL && codepoints < end) {
        if (cs->codepoint != *codepoints)
            return cs->codepoint - *codepoints;

        cs = cs->next;
        codepoints++;
    }

    if (cs)
        return 1;
    else if (codepoints < end)
        return -1;
    else
        return 0;
}

void test_utf8_single_byte()
{
    int pass = 1;
    int ret;
    uint32_t codepoints[] = { '\x00', 'F', 'a', 'b', 'i', 'a', 'n', '\x7F' };

    struct output output = {
        .head = NULL,
        .tail = NULL
    };

    fab_utf8_t utf8 = fab_utf8_create(test_utf8_callback, &output);

    if (utf8 == NULL) {
        fab_fail(__func__);
        return;
    }

    ret = 0;
    ret |= fab_utf8_process(utf8, '\x00');
    ret |= fab_utf8_process(utf8, 'F');
    ret |= fab_utf8_process(utf8, 'a');
    ret |= fab_utf8_process(utf8, 'b');
    ret |= fab_utf8_process(utf8, 'i');
    ret |= fab_utf8_process(utf8, 'a');
    ret |= fab_utf8_process(utf8, 'n');
    ret |= fab_utf8_process(utf8, '\x7F');

    if (ret)
        pass = 0;

    fab_utf8_destroy(utf8);
    
    if (test_utf8_compare(output.head, codepoints, 8) != 0)
        pass = 0;

    while (output.head != NULL) {
        struct character *next = output.head->next;
        free(output.head);
        output.head = next;
    }

    if (pass)
        fab_pass(__func__);
    else
        fab_fail(__func__);
}

void test_utf8_invalid_byte()
{
    int ret;
    int pass = 1;
    struct output output = {
        .head = NULL,
        .tail = NULL
    };

    fab_utf8_t utf8 = fab_utf8_create(test_utf8_callback, &output);

    if (utf8 == NULL) {
        fab_fail(__func__);
        return;
    }

    ret = fab_utf8_process(utf8, '\xFF');
    if (ret == 0 || errno != EINVAL)
        pass = 0;

    fab_utf8_destroy(utf8);

    pass = output.head == NULL;

    while (output.head != NULL) {
        struct character *next = output.head->next;
        free(output.head);
        output.head = next;
    }

    if (pass)
        fab_pass(__func__);
    else
        fab_fail(__func__);
}
