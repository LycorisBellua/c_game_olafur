#include "olafur.h"

char	json_peek(t_json *j)
{
	while (j->text[j->i] == ' ' || j->text[j->i] == '\t' || j->text[j->i] == '\n'
		|| j->text[j->i] == '\r')
		++j->i;
	return (j->text[j->i]);
}

int	json_expect(t_json *j, char c, const char *msg)
{
	if (json_peek(j) != c)
		return (json_error(j, msg));
	++j->i;
	return (1);
}

int	json_error(t_json *j, const char *msg)
{
	char	location[PATH_MSG_SIZE];
	size_t	line;
	size_t	col;
	size_t	k;

	line = 1;
	col = 1;
	k = 0;
	while (k < j->i)
	{
		++col;
		if (j->text[k++] == '\n')
		{
			++line;
			col = 1;
		}
	}
	if (!j->text[j->i])
		msg = E_JSON_EOF;
	snprintf(location, PATH_MSG_SIZE, "%s, line %lu, column %lu", j->path,
		(unsigned long)line, (unsigned long)col);
	return (put_error(location, msg, 0));
}

int	json_list(t_json *j, char close, t_json_item item, void *ctx)
{
	const char	*msg;

	msg = E_JSON_ARRAY_END;
	if (close == '}')
		msg = E_JSON_OBJECT_END;
	if (json_peek(j) == close)
	{
		++j->i;
		return (1);
	}
	while (item(j, ctx))
	{
		if (json_peek(j) == close)
		{
			++j->i;
			return (1);
		}
		if (!json_expect(j, ',', msg))
			return (0);
	}
	return (0);
}
