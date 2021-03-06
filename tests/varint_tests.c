#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "varint.h"
#include "mu_unit.h"

int varfmt_cmp(const unsigned char *buf, const char *rhs, size_t len);

char *test_varint(void)
{
    size_t i = 0;
    varint_t v1 = 123;
    varint_t v3 = 515;
    varint_t v5 = 0x234567ab;
    varint_t v9 = 0x12345678a1a2a3a4;
    varint_t val;
    unsigned char buf[9];

    i = kyk_pack_varint(buf, v1);
    mu_assert(i == 1, "pack failed");
    mu_assert(varfmt_cmp(buf, "0x7b", 1), "pack failed");

    i = kyk_unpack_varint(buf, &val);
    mu_assert(i == 1, "unpack failed");
    mu_assert(val == v1, "unpack failed");
    
    i = kyk_pack_varint(buf, v3);
    mu_assert(i == 3, "pack failed");
    mu_assert(varfmt_cmp(buf, "0xfd0302", 3), "pack failed");

    i = kyk_unpack_varint(buf, &val);
    mu_assert(i == 3, "unpack failed");
    mu_assert(val == v3, "unpack failed");

    i = kyk_pack_varint(buf, v5);
    mu_assert(i == 5, "pack failed");
    mu_assert(varfmt_cmp(buf, "0xfeab674523", 5), "pack failed");

    i = kyk_unpack_varint(buf, &val);
    mu_assert(i == 5, "unpack failed");
    mu_assert(val == v5, "unpack failed");

    i = kyk_pack_varint(buf, v9);
    mu_assert(i == 9, "pack failed");
    mu_assert(varfmt_cmp(buf, "0xffa4a3a2a178563412", 9), "pack failed");

    i = kyk_unpack_varint(buf, &val);
    mu_assert(i == 9, "unpack failed");
    mu_assert(val == v9, "unpack failed");

    return NULL;
    
}

char* test_kyk_new_var_str()
{
    var_str* vstr = NULL;
    const char* str = "hello bitcoin";

    vstr = kyk_new_var_str(str);

    mu_assert(vstr, "Failed to test_kyk_new_var_str");
    mu_assert(vstr -> len == strlen(str), "Failed to test_kyk_new_var_str");

    kyk_free_var_str(vstr);
    
    return NULL;
}

char* test_kyk_pack_var_str()
{
    var_str* vstr = NULL;
    uint8_t buf[100];
    const char* str = "hello bitcoin";
    size_t len = 0;

    vstr = kyk_new_var_str(str);

    len = kyk_pack_var_str(buf, vstr);
    mu_assert(len = 1 + strlen(str), "Failed to test_kyk_pack_var_str");

    return NULL;

}

char* test_kyk_unpack_var_str()
{
    var_str* vstr = NULL;
    uint8_t buf[] = {
	0x0d, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x20,
	0x62, 0x69, 0x74, 0x63, 0x6f, 0x69, 0x6e
    };
    const char* str = "hello bitcoin";
    int res = -1;
    size_t len = 0;

    res = kyk_unpack_var_str(buf, &vstr, &len);
    mu_assert(res == 0, "Failed to test_kyk_unpack_var_str");
    mu_assert(len == 1 + strlen(str), "Failed to test_kyk_unpack_var_str");
    mu_assert(strcmp(str, vstr -> data) == 0, "Failed to test_kyk_unpack_var_str");
    
    return NULL;
}

int varfmt_cmp(const unsigned char *buf, const char *rhs, size_t len)
{
    unsigned char hex_buf[100] = {0};
    unsigned char *p = hex_buf;
    int res;
    size_t i = 0;

    p += sprintf((char *)p, "0x");
    for(i = 0; i < len; i++){
	p += sprintf((char *)p, "%02x", buf[i]);
    }

    res = strncmp((char *)hex_buf, rhs, strlen(rhs)) == 0 ? 1 : 0;


    return res;
}


char *all_tests()
{
    mu_suite_start();
    
    mu_run_test(test_varint);
    mu_run_test(test_kyk_new_var_str);
    mu_run_test(test_kyk_pack_var_str);
    mu_run_test(test_kyk_unpack_var_str);
    
    return NULL;
}

MU_RUN_TESTS(all_tests);
