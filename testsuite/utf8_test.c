
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "utf8_test.h"

#include <utf8.h>
#include <stdlib.h>
#include <errno.h>



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
    int ret;
    uint32_t codepoints[] = { '\x00', 'F', 'a', 'b', 'i', 'a', 'n', '\x7F' };

    struct output output = {
        .head = NULL,
        .tail = NULL
    };

    fab_utf8_t utf8 = fab_utf8_create(test_utf8_callback, &output);

    ck_assert(utf8 != NULL);

    ret = 0;
    ret |= fab_utf8_process(utf8, '\x00');
    ret |= fab_utf8_process(utf8, 'F');
    ret |= fab_utf8_process(utf8, 'a');
    ret |= fab_utf8_process(utf8, 'b');
    ret |= fab_utf8_process(utf8, 'i');
    ret |= fab_utf8_process(utf8, 'a');
    ret |= fab_utf8_process(utf8, 'n');
    ret |= fab_utf8_process(utf8, '\x7F');

    ck_assert(!ret);

    fab_utf8_destroy(utf8);
    
    ck_assert_int_eq(0, test_utf8_compare(output.head, codepoints, 8));

    while (output.head != NULL) {
        struct character *next = output.head->next;
        free(output.head);
        output.head = next;
    }
}

void test_utf8_invalid_byte()
{
    int ret;
    struct output output = {
        .head = NULL,
        .tail = NULL
    };

    fab_utf8_t utf8 = fab_utf8_create(test_utf8_callback, &output);

    ck_assert(utf8 != NULL);

    ret = fab_utf8_process(utf8, '\xFF');
    ck_assert(ret);
    ck_assert_int_eq(EINVAL, errno);

    fab_utf8_destroy(utf8);

    ck_assert_ptr_eq(NULL, output.head);

    while (output.head != NULL) {
        struct character *next = output.head->next;
        free(output.head);
        output.head = next;
    }
}

Suite *utf8_test_suite()
{
    Suite *s;
    TCase *tc_single_byte, *tc_invalid;;

    s = suite_create("utf8");

    /* single byte test case */
    tc_single_byte = tcase_create("single_byte");
    tcase_add_test(tc_single_byte, test_utf8_single_byte);
    suite_add_tcase(s, tc_single_byte);

    /* invalid input */
    tc_invalid = tcase_create("invalid");
    tcase_add_test(tc_invalid, test_utf8_invalid_byte);
    suite_add_tcase(s, tc_invalid);

    return s;
}
