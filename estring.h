#ifndef ENHANCED_CSTRINGS_H
#define ENHANCED_CSTRINGS_H

#define String(str) _Generic((str), \
		char*: String_from_cstr, \
		StringView: String_copy \
	)(str)

#define StringView(str) StringView_from_cstr(str)

typedef struct String {
	char *data;
	size_t len;
} String;

/* The differentiation between String and StringView exists purely so that you
 * can be discrete about whether the data buffer is owned by the returned String
 * object or not. Any methods pertaining to String allocate memory,
 * while StringView methods will not touch the contents of the data buffer at all,
 * at most just shifting the position of the data pointer and changing the stored length.
 * This allows for any method that operates on a StringView to be reused for a
 * String method, just by making this the body:
 *
 * return String_copy(StringView_method(params)); */
typedef String StringView;

/* Returns a pointer to the internal data of a String. Safer than just grabbing
 * str.data. If a mutated StringView that is no longer properly null-terminated
 * is passed in, a null pointer will be returned. Using this with a StringView
 * object is highly discouraged, since whether you will get a valid pointer can
 * be unpredictable. */
char *String_to_cstr(String str);

// Converts a string to a long integer.
long String_to_long(String str, int base);

// Converts a string to a long long integer.
long long String_to_llong(String str, int base);

// Converts a string to an unsigned long integer.
unsigned long String_to_ulong(String str, int base);

// Converts a string to an unsigned long long integer.
unsigned long long String_to_ullong(String str, int base);

// Converts a string to a double-precision floating point number.
double String_to_double(String str);

// Converts a string to a single-precision floating point number.
float String_to_float(String str);

// Converts a string to a long double-precision floating point number.
long double String_to_ldouble(String str);

// Copies the data from str into a new String.
String String_copy(String str);

// Concatenates two strings together.
String String_concat(String s1, String s2);

/* Returns a string which is equivalent to concatenating the same string together
 * count times. */
String String_repeat(String str, size_t count);

// Returns a string with all uppercase characters.
String String_toupper(String str);

// Returns a string with all lowercase characters.
String String_tolower(String str);

// Replaces all characters matching old with new.
String String_replace(String str, const char old, const char new);

// Creates a new String object from a null-terminated string.
StringView StringView_from_cstr(char *str);
String String_from_cstr(char *str);

// Returns a new String only consisting of characters between begin and end.
StringView StringView_substring(StringView str, ssize_t begin, ssize_t end);
String String_substring(StringView str, ssize_t begin, ssize_t end);

// Compares the values of two strings.
int StringView_compare(StringView s1, StringView s2);
int String_compare(String s1, String s2);

// Same as String_compare, but ignoring case.
int StringView_compare_nocase(StringView s1, StringView s2);
int String_compare_nocase(String s1, String s2);

// Checks whether a string contains a character or not.
int StringView_contains_chr(const StringView str, const char c);
int String_contains_chr(const String str, const char c);

// Checks whether a string contains a substring.
int StringView_contains_str(const StringView str, const StringView substr);
int String_contains_str(const String str, const String substr);

// Returns the index of the first occurrence of a character, or -1 if it is not found.
int StringView_indexof_chr(const StringView str, const char c);
int String_indexof_chr(const String str, const char c);

// Returns the index of the last occurrence of a character, or -1 if it is not found.
int StringView_indexof_rchr(const StringView str, const char c);
int String_indexof_rchr(const String str, const char c);

// Returns the index of the first occurrence of a substring, or -1 if it is not found.
int StringView_indexof_str(const StringView str, const StringView substr);
int String_indexof_str(const String str, const String substr);

/* Searches for the first occurrence of a character.
 * Returns a new String beginning at the point that this character was found,
 * or a null string if it is not found. */
StringView StringView_search_chr(StringView str, const char c);
String String_search_chr(String str, const char c);

/* Searches for the last occurrence of a character.
 * Returns a new String beginning at the point that this character was found,
 * or a null string if it is not found. */
StringView StringView_search_rchr(StringView str, const char c);
String String_search_rchr(String str, const char c);

/* Searches for the first occurrence of a substring.
 * Returns a new String beginning at the point that this substring was found,
 * or a null string if it is not found. */
StringView StringView_search_str(StringView str, const StringView substr);
String String_search_str(String str, String substr);

// Checks if a string begins with the sequence of characters in a substring.
int StringView_startswith(StringView str, const StringView substr);
int String_startswith(String str, const String substr);

// Checks if a string ends with the sequence of characters in a substring.
int StringView_endswith(StringView str, const StringView substr);
int String_endswith(String str, const String substr);

// Strips all leading whitespace from a string.
StringView StringView_strip_leading(StringView str);
String String_strip_leading(String str);

// Strips all trailing whitespace from a string.
StringView StringView_strip_trailing(StringView str);
String String_strip_trailing(String str);

// Strips all leading and trailing whitespace from a string.
StringView StringView_strip(StringView str);
String String_strip(String str);

#endif
