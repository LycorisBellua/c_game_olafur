#include "olafur.h"

static int	copy_string(t_json *j, char *s);
static int	get_escaped_char(t_json *j, char *c);

int	json_string(t_json *j, char **dst)
{
	size_t	k;
	char	*s;

	if (!json_expect(j, '"', E_JSON_STRING))
		return (0);
	k = j->i;
	while (j->text[k] && j->text[k] != '"')
		k += 1 + (j->text[k] == '\\' && j->text[k + 1]);
	s = malloc(k - j->i + 1);
	if (!s)
		return (put_error(E_FAIL_MEM, 0, 0));
	if (!copy_string(j, s))
	{
		free(s);
		return (0);
	}
	free(*dst);
	*dst = s;
	if (!s[0])
	{
		free(s);
		*dst = 0;
	}
	return (1);
}

static int	copy_string(t_json *j, char *s)
{
	size_t	len;

	len = 0;
	while (j->text[j->i] != '"')
	{
		if ((unsigned char)j->text[j->i] < ' ')
			return (json_error(j, E_JSON_STRING_END));
		if (j->text[j->i] != '\\')
			s[len++] = j->text[j->i];
		else if (!get_escaped_char(j, &s[len++]))
			return (0);
		++j->i;
	}
	s[len] = 0;
	++j->i;
	return (1);
}

static int	get_escaped_char(t_json *j, char *c)
{
	const char	*from;
	const char	*esc;

	from = "\"\\/bfnrt";
	esc = 0;
	++j->i;
	if (j->text[j->i])
		esc = strchr(from, j->text[j->i]);
	if (!esc)
		return (json_error(j, E_JSON_ESCAPE));
	*c = "\"\\/\b\f\n\r\t"[esc - from];
	return (1);
}

int	json_int(t_json *j, int *dst)
{
	long long	n;
	size_t		start;

	json_peek(j);
	start = j->i;
	j->i += (j->text[j->i] == '-');
	if (!isdigit((unsigned char)j->text[j->i]))
		return (json_error(j, E_JSON_INT));
	n = 0;
	while (isdigit((unsigned char)j->text[j->i]))
	{
		if (n <= INT_MAX)
			n = n * 10 + j->text[j->i] - '0';
		++j->i;
	}
	if (j->text[j->i] && strchr(".eE", j->text[j->i]))
		return (json_error(j, E_JSON_INT));
	if (n > INT_MAX + (long long)(j->text[start] == '-'))
	{
		j->i = start;
		return (json_error(j, E_JSON_INT_RANGE));
	}
	*dst = (int)(n * (1 - 2 * (j->text[start] == '-')));
	return (1);
}

int	json_ivec2(t_json *j, t_ivec2 *dst)
{
	return (json_expect(j, '[', E_JSON_ARRAY) && json_int(j, &dst->x)
		&& json_expect(j, ',', E_JSON_COMMA) && json_int(j, &dst->y)
		&& json_expect(j, ']', E_JSON_ARRAY_CLOSE));
}
