#ifndef ENHANCED_CSTRINGS_H
#define ENHANCED_CSTRINGS_H

#define String(str) _Generic((str), \
		char*: String_from_cstr, \
		String: String_to_cstr \
	)(str)

#define StringView(str) _Generic((str), \
		char*: StringView_from_cstr, \
		StringView: StringView_to_cstr \
	)(str)

typedef struct String {
	char *data;
	size_t len;
} String;

/* This differentiation exists purely so that you can be discrete about whether
 * the data buffer is owned by the String object or not. */
typedef String StringView;

char *String_to_cstr(String str);

String String_copy(String str);

String String_concat(String s1, String s2);

String String_repeat(String str, size_t count);

String String_toupper(String str);

String String_tolower(String str);

String String_replace(String str, const char old, const char new);

StringView StringView_from_cstr(char *str);

String String_from_cstr(char *str);

StringView StringView_substring(StringView str, ssize_t begin, ssize_t end);
String String_substring(StringView str, ssize_t begin, ssize_t end);

int StringView_compare(StringView s1, StringView s2);
int String_compare(String s1, String s2);

int StringView_compare_nocase(StringView s1, StringView s2);
int String_compare_nocase(String s1, String s2);

int StringView_contains_chr(const StringView str, const char c);
int String_contains_chr(const String str, const char c);

int StringView_contains_rchr(const StringView str, const char c);
int String_contains_rchr(const String str, const char c);

int StringView_contains_str(const StringView str, const StringView substr);
int String_contains_str(const String str, const String substr);

int StringView_indexof_chr(const StringView str, const char c);
int String_indexof_chr(const String str, const char c);

int StringView_indexof_rchr(const StringView str, const char c);
int String_indexof_rchr(const String str, const char c);

int StringView_indexof_str(const StringView str, const StringView substr);
int String_indexof_str(const String str, const String substr);

StringView StringView_search_chr(StringView str, const char c);
String String_search_chr(String str, const char c);

StringView StringView_search_rchr(StringView str, const char c);
String String_search_rchr(String str, const char c);

StringView StringView_search_str(StringView str, const StringView substr);
String String_search_str(String str, String substr);

int StringView_startswith(StringView str, const StringView substr);
int String_startswith(String str, const String substr);

int StringView_endswith(StringView str, const StringView substr);
int String_endswith(String str, const String substr);

StringView StringView_strip_leading(StringView str);
String String_strip_leading(String str);

StringView StringView_strip_trailing(StringView str);
String String_strip_trailing(String str);

StringView StringView_strip(StringView str);
String String_strip(String str);

#endif
