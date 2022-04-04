#define _GNU_SOURCE

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <regex.h>

#include "estring.h"

#define min(a, b) (a) < (b) ? (a) : (b)
#define max(a, b) (a) > (b) ? (a) : (b)

char *String_to_cstr(String str) {
	if (str.data == NULL || str.data[str.len] != '\0') return NULL;
	else return str.data;
}

// INTERNAL FUNCTION (for now) to make allocating Strings slightly easier
static String String_alloc(size_t size) {
	return (String) {
		.data = calloc(size + 1, sizeof(char)),
		.len = size + 1
	};
}

// INTERNAL MACRO, COULD RUIN THE STACK IF NOT USED CAREFULLY
#define String_alloca(size) (String) {.data = alloca(size + 1), .len = size}

void String_free(String str) {
	free(str.data);
}

// INTERNAL FUNCTION
static String String_copy_prealloc(String dest, String src) {
	strncpy(dest.data, src.data, dest.len);
	return dest;
}

String String_copy(String str) {
	return (String) {
		.data = strndup(str.data, str.len),
		.len = str.len
	};
}

// INTERNAL MACRO, COULD RUIN THE STACK IF NOT USED CAREFULLY
#define String_copya(str) (String) {.data = strndupa(str.data, str.len), .len = str.len}

long String_to_long(String str, int base) {
	return strtol(String_to_cstr(str), NULL, base);
}

long StringView_to_long(StringView str, int base) {
	String temp = String_copy(str);
	long res = String_to_long(temp, base);
	String_free(temp);
	return res;
}

long long String_to_llong(String str, int base) {
	return strtoll(String_to_cstr(str), NULL, base);
}

long long StringView_to_llong(StringView str, int base) {
	String temp = String_copy(str);
	long long res = String_to_llong(temp, base);
	String_free(temp);
	return res;
}

unsigned long String_to_ulong(String str, int base) {
	return strtoul(String_to_cstr(str), NULL, base);
}

unsigned long StringView_to_ulong(StringView str, int base) {
	String temp = String_copy(str);
	unsigned long res = String_to_ulong(temp, base);
	String_free(temp);
	return res;
}

unsigned long long String_to_ullong(String str, int base) {
	return strtoull(String_to_cstr(str), NULL, base);
}

unsigned long long StringView_to_ullong(StringView str, int base) {
	String temp = String_copy(str);
	unsigned long long res = String_to_ullong(temp, base);
	String_free(temp);
	return res;
}

double String_to_double(String str) {
	return strtod(String_to_cstr(str), NULL);
}

double StringView_to_double(StringView str) {
	String temp = String_copy(str);
	double res = String_to_double(temp);
	String_free(temp);
	return res;
}

float String_to_float(String str) {
	return strtof(String_to_cstr(str), NULL);
}

float StringView_to_float(StringView str) {
	String temp = String_copy(str);
	float res = String_to_float(temp);
	String_free(temp);
	return res;
}

long double String_to_ldouble(String str) {
	return strtold(String_to_cstr(str), NULL);
}

long double StringView_to_ldouble(StringView str) {
	String temp = String_copy(str);
	long double res = String_to_ldouble(temp);
	String_free(temp);
	return res;
}

int String_matches(String str, const char *regex) {
	regex_t preg;
	int compres = regcomp(&preg, regex, REG_EXTENDED | REG_NOSUB);
	if (compres) return compres;
	int execres = regexec(&preg, String_to_cstr(str), 0, NULL, 0);
	regfree(&preg);
	return execres;
}

int StringView_matches(StringView str, const char *regex) {
	String temp = String_copy(str);
	int res = String_matches(temp, regex);
	String_free(temp);
	return res;
}

String String_concat(String s1, String s2) {
	String ret = String_alloc(s1.len + s2.len);
	char *tmp = mempcpy(ret.data, s1.data, s1.len);
	memcpy(tmp, s2.data, s2.len);

	return ret;
}

String String_repeat(String str, size_t count) {
	if (++count < 1) return (String) {0};
	String ret = String_alloc(str.len * count);
	char *new_string = ret.data;

	for (size_t i = 0; i < count; ++i) {
		new_string = mempcpy(new_string, str.data, str.len);
		memcpy(new_string, str.data, str.len);
	}

	return ret;
}

String String_toupper(String str) {
	String ret = String_copy(str);
	for (size_t i = 0; i < ret.len; ++i) ret.data[i] = toupper(ret.data[i]);
	return ret;
}

String String_tolower(String str) {
	String ret = String_copy(str);
	for (size_t i = 0; i < ret.len; ++i) ret.data[i] = tolower(ret.data[i]);
	return ret;
}

String String_replace(String str, const char old, const char new) {
	String ret = String_copy(str);
	for (size_t i = 0; i < ret.len; ++i) if (ret.data[i] == old) ret.data[i] = new;
	return ret;
}

StringView StringView_from_cstr(char *str) {
	StringView ret = {
		.data = str,
		.len = strlen(str)
	};

	return ret;
}

String String_from_cstr(char *str) {
	return String_copy(StringView_from_cstr(str));
}

StringView StringView_substring(StringView str, ssize_t begin, ssize_t end) {
	if (begin < 0) begin = str.len + begin;
	if (end <= 0) end = str.len + end;
	str.data += begin;
	str.len = end - begin;
	return str;
}

String String_substring(StringView str, ssize_t begin, ssize_t end) {
	return String_copy(StringView_substring(str, begin, end));
}

int StringView_compare(StringView s1, StringView s2) {
	ssize_t len_diff = s1.len - s2.len;
	int result = strncmp(s1.data, s2.data, min(s1.len, s2.len));

	if (len_diff > 0) {
		s1.data += s2.len;
		s1.len -= s2.len;
		while (s1.len > 0) {
			result += *s1.data++;
			--s1.len;
		}
	} else if (len_diff < 0) {
		s2.data += s1.len;
		s2.len -= s1.len;
		while (s2.len > 0) {
			result -= *s2.data++;
			--s2.len;
		}
	}

	return result;
}

int String_compare(String s1, String s2) {
	return StringView_compare(s1, s2);
}

int StringView_compare_nocase(StringView s1, StringView s2) {
	ssize_t len_diff = s1.len - s2.len;
	int result = strncasecmp(s1.data, s2.data, min(s1.len, s2.len));

	if (len_diff > 0) {
		s1.data += s2.len;
		s1.len -= s2.len;
		while (s1.len > 0) {
			result += *s1.data++ | 32;
			--s1.len;
		}
	} else if (len_diff < 0) {
		s2.data += s1.len;
		s2.len -= s1.len;
		while (s2.len > 0) {
			result -= *s2.data++ | 32;
			--s2.len;
		}
	}

	return result;
}

int String_compare_nocase(String s1, String s2) {
	return StringView_compare_nocase(s1, s2);
}

int StringView_contains_chr(const StringView str, const char c) {
	return memchr(str.data, c, str.len) != NULL;
}

int String_contains_chr(const String str, const char c) {
	return StringView_contains_chr(str, c);
}

int StringView_contains_str(const StringView str, const StringView substr) {
	return memmem(str.data, str.len, substr.data, substr.len) != NULL;
}

int String_contains_str(const String str, const String substr) {
	return StringView_contains_str(str, substr);
}

int StringView_indexof_chr(const StringView str, const char c) {
	char *result = memchr(str.data, c, str.len);
	if (result == NULL) return -1;
	else return result - str.data;
}

int String_indexof_chr(const String str, const char c) {
	return StringView_indexof_chr(str, c);
}

int StringView_indexof_rchr(const StringView str, const char c) {
	char *result = memrchr(str.data, c, str.len);
	if (result == NULL) return -1;
	else return result - str.data;
}

int String_indexof_rchr(const String str, const char c) {
	return StringView_indexof_rchr(str, c);
}

int StringView_indexof_str(const StringView str, const StringView substr) {
	char *result = memmem(str.data, str.len, substr.data, substr.len);
	if (result == NULL) return -1;
	else return result - str.data;
}

int String_indexof_str(const String str, const String substr) {
	return StringView_indexof_str(str, substr);
}

StringView StringView_search_chr(StringView str, const char c) {
	char *result = memchr(str.data, c, str.len);

	if (result == NULL) return (StringView) {0};
	else return (StringView) {
		.data = result,
		.len = str.len - (result - str.data)
	};
}

String String_search_chr(String str, const char c) {
	return String_copy(StringView_search_chr(str, c));
}

StringView StringView_search_rchr(StringView str, const char c) {
	char *result = memrchr(str.data, c, str.len);

	if (result == NULL) return (StringView) {0};
	else return (StringView) {
		.data = result,
		.len = str.len - (result - str.data)
	};
}

String String_search_rchr(String str, const char c) {
	return String_copy(StringView_search_rchr(str, c));
}

StringView StringView_search_str(StringView str, const StringView substr) {
	char *result = memmem(str.data, str.len, substr.data, substr.len);

	if (result == NULL) return (StringView) {0};
	else return (StringView) {
		.data = result,
		.len = str.len - (result - str.data)
	};
}

String String_search_str(String str, String substr) {
	return String_copy(StringView_search_str(str, substr));
}

int StringView_startswith(StringView str, const StringView substr) {
	if (substr.len > str.len) return 0;
	str.len = substr.len;
	return StringView_compare(str, substr);
}

int String_startswith(String str, const String substr) {
	return StringView_startswith(str, substr);
}

int StringView_endswith(StringView str, const StringView substr) {
	if (substr.len > str.len) return 0;
	str.data += str.len - substr.len;
	str.len = substr.len;
	return StringView_compare(str, substr);
}

int String_endswith(String str, const String substr) {
	return StringView_endswith(str, substr);
}

StringView StringView_strip_leading(StringView str) {
	while (isblank(*str.data)) {
		++str.data;
		--str.len;
	}

	return str;
}

String String_strip_leading(String str) {
	return String_copy(StringView_strip_leading(str));
}

StringView StringView_strip_trailing(StringView str) {
	char *s = str.data + str.len - 1;
	while (isblank(*s--)) --str.len;
	return str;
}

String String_strip_trailing(String str) {
	return String_copy(StringView_strip_trailing(str));
}

StringView StringView_strip(StringView str) {
	return StringView_strip_trailing(StringView_strip_leading(str));
}

String String_strip(String str) {
	return String_copy(StringView_strip(str));
}
