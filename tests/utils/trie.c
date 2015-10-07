#include <stic.h>

#include <stddef.h> /* NULL */
#include <string.h> /* strdup() */

#include "../../src/utils/trie.h"

TEST(freeing_new_trie_is_ok)
{
	const trie_t trie = trie_create();
	assert_non_null(trie);
	trie_free(trie);
}

TEST(freeing_null_trie_is_ok)
{
	trie_free(NULL_TRIE);
}

TEST(put_returns_zero_for_new_string)
{
	const trie_t trie = trie_create();

	assert_int_equal(0, trie_put(trie, "str"));

	trie_free(trie);
}

TEST(put_returns_positive_number_for_existing_string)
{
	const trie_t trie = trie_create();

	assert_int_equal(0, trie_put(trie, "str"));
	assert_true(trie_put(trie, "str") > 0);

	trie_free(trie);
}

TEST(multiple_puts)
{
	const trie_t trie = trie_create();

	assert_int_equal(0, trie_put(trie, "str"));
	assert_int_equal(0, trie_put(trie, "astr"));
	assert_int_equal(0, trie_put(trie, "string"));
	assert_int_equal(0, trie_put(trie, "strong"));
	assert_int_equal(0, trie_put(trie, "xxx"));

	assert_true(trie_put(trie, "str") > 0);
	assert_int_equal(0, trie_put(trie, "st"));
	assert_int_equal(0, trie_put(trie, "s"));

	assert_true(trie_put(trie, "astr") > 0);
	assert_int_equal(0, trie_put(trie, "ast"));
	assert_int_equal(0, trie_put(trie, "as"));
	assert_int_equal(0, trie_put(trie, "a"));

	assert_true(trie_put(trie, "string") > 0);
	assert_int_equal(0, trie_put(trie, "strin"));
	assert_int_equal(0, trie_put(trie, "stri"));

	assert_true(trie_put(trie, "strong") > 0);
	assert_int_equal(0, trie_put(trie, "stron"));
	assert_int_equal(0, trie_put(trie, "stro"));

	assert_true(trie_put(trie, "xxx") > 0);
	assert_int_equal(0, trie_put(trie, "xx"));
	assert_int_equal(0, trie_put(trie, "x"));

	trie_free(trie);
}

TEST(empty_string_does_not_exists_after_trie_creation)
{
	const trie_t trie = trie_create();

	assert_int_equal(0, trie_put(trie, ""));

	trie_free(trie);
}

TEST(utf8)
{
	const trie_t trie = trie_create();

	assert_int_equal(0, trie_put(trie, "строка"));
	assert_int_equal(0, trie_put(trie, "string"));
	assert_true(trie_put(trie, "строка") > 0);
	assert_true(trie_put(trie, "string") > 0);

	trie_free(trie);
}

TEST(put_sets_data_to_null)
{
	const trie_t trie = trie_create();
	void *data = trie;

	assert_int_equal(0, trie_put(trie, "str"));
	assert_success(trie_get(trie, "str", &data));
	assert_null(data);

	trie_free(trie);
}

TEST(get_returns_previously_set_data)
{
	const trie_t trie = trie_create();
	void *data;

	assert_int_equal(0, trie_set(trie, "str", trie));
	assert_success(trie_get(trie, "str", &data));
	assert_true(data == trie);

	trie_free(trie);
}

TEST(set_overwrites_previous_data)
{
	const trie_t trie = trie_create();
	void *data;

	assert_int_equal(0, trie_set(trie, "str", trie));
	assert_true(trie_set(trie, "str", &data) > 0);
	assert_success(trie_get(trie, "str", &data));
	assert_true(data == &data);

	trie_free(trie);
}

TEST(free_with_data_removes_data)
{
	/* The check is implicit, external tool should check that no memory leak is
	 * created. */

	const trie_t trie = trie_create();

	assert_true(trie_set(trie, "str", strdup("str")) == 0);
	assert_true(trie_set(trie, "something", strdup("something")) == 0);

	trie_free_with_data(trie);
}

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab cinoptions-=(0 : */
/* vim: set cinoptions+=t0 filetype=c : */
