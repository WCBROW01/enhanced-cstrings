#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define String(str) _Generic((str), \
		const char*: String_from_cstr, \
		String: String_to_cstr \
	)(str)

//#define Stringa(str) strndupa((str).data, (str).len)

typedef struct String {
	char *data;
	size_t len;
} String;

typedef String StringView;

StringView StringView_from_cstr(const char *str) {
	return {
		.data = str,
		.len = strlen(str)
	};
}

String StringView_truncate(StringView str, size_t begin, size_t end) {
	size_t len = end - begin;
	char *sdata = str.data;
	sdata += begin;
	return {
		.data = sdata,
		.len = len
	};
}

int StringView_cmp(StringView s1, StringView s2) {
	size_t len_diff = s1.len - s2.len
	int result = memcmp(s1, s2, s1.len - len_diff);

	if (len_diff > 0) {
		s1.data += s2.len;
		s1.len -= s2.len;
		while (s1.len > 0) {
			result += *s1.data++;
			s1.len--;
		}
	} else if (len_diff < 0) {
		s2.data += s1.len;
		s2.len -= s1.len;
		while (s2.len > 0) {
			result -= *s2.data++;
			s2.len--;
		}
	}

	return result;
}

StringView StringView_searchchr(StringView str, char c) {
	char *result = memchr(str.data, c, str.len);
	if (result == NULL) return {
		.data = NULL,
		.len = 0
	};
	else return {
		.data = result,
		.len = str.len - result - str
	};
}

StringView StringView_searchrchr(StringView str, char c) {
	char *result = memrchr(str.data, c, str.len);
	if (result == NULL) return {
		.data = NULL,
		.len = 0
	};
	else return {
		.data = result,
		.len = str.len - result - str
	};
}

StringView StringView_searchstr(StringView str, StringView substr) {
	char *result = memmem(str.data, str.len, substr.data, substr.len);
	if (result == NULL) return {
		.data = NULL,
		.len = 0
	};
	else return {
		.data = result,
		.len = str.len - result - str.data
	};
}

StringView StringView_tokenize(StringView str,

char *String_to_cstr(String str) {
	return strndup(str.data, str.len);
}

String String_copy(String str) {
	return {
		.data = memcpy(malloc(str.len), str.data, str.len),
		.len = str.len
	};
}

String String_cat(String s1, String s2) {
	char *new_string = malloc(s1.len + s2.len);
	String ret = {
		.data = new_string,
		.len = s1.len + s2.len
	};

	new_string = mempcpy(new_string, s1.data, s1.len);
	memcpy(new_string, s2.data, s2.len);

	return ret;
}
