#define _GNU_SOURCE

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "estring.h"

#define min(a, b) (a) < (b) ? (a) : (b)
#define max(a, b) (a) > (b) ? (a) : (b)

char *String_to_cstr(String str) {
	return strndup(str.data, str.len);
}

String String_copy(String str) {
	String ret = {
		.data = memcpy(malloc(str.len), str.data, str.len),
		.len = str.len
	};

	return ret;
}

String String_concat(String s1, String s2) {
	char *new_string = malloc(s1.len + s2.len);
	String ret = {
		.data = new_string,
		.len = s1.len + s2.len
	};

	new_string = mempcpy(new_string, s1.data, s1.len);
	memcpy(new_string, s2.data, s2.len);

	return ret;
}

String String_repeat(String str, size_t count) {
	char *new_string = malloc(str.len * count);
	String ret = {
		.data = new_string,
		.len = str.len * count
	};

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

int StringView_contains_rchr(const StringView str, const char c) {
	return memrchr(str.data, c, str.len) != NULL;
}

int String_contains_rchr(const String str, const char c) {
	return StringView_contains_rchr(str, c);
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

	if (result == NULL) {
		StringView ret = {
			.data = NULL,
			.len = 0
		};
		return ret;
	} else {
		StringView ret = {
			.data = result,
			.len = str.len - (result - str.data)
		};
		return ret;
	}
}

String String_search_chr(String str, const char c) {
	return String_copy(StringView_search_chr(str, c));
}

StringView StringView_search_rchr(StringView str, const char c) {
	char *result = memrchr(str.data, c, str.len);

	if (result == NULL) {
		StringView ret = {
			.data = NULL,
			.len = 0
		};
		return ret;
	} else {
		StringView ret = {
			.data = result,
			.len = str.len - (result - str.data)
		};
		return ret;
	}
}

String String_search_rchr(String str, const char c) {
	return String_copy(StringView_search_rchr(str, c));
}

StringView StringView_search_str(StringView str, const StringView substr) {
	char *result = memmem(str.data, str.len, substr.data, substr.len);

	if (result == NULL) {
		StringView ret = {
			.data = NULL,
			.len = 0
		};
		return ret;
	} else {
		StringView ret = {
			.data = result,
			.len = str.len - (result - str.data)
		};
		return ret;
	}
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
