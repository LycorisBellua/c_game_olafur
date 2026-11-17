#include "olafur.h"

static int	skip_item(t_json *j, void *ctx);
static int	skip_member(t_json *j, void *ctx);
static int	skip_number(t_json *j);
static int	skip_literal(t_json *j);

int	json_skip_value(t_json *j)
{
	char	c;
	char	*s;

	c = json_peek(j);
	if (c == '"')
	{
		s = 0;
		if (!json_string(j, &s))
			return (0);
		free(s);
		return (1);
	}
	if (c == '[' || c == '{')
	{
		++j->i;
		if (c == '[')
			return (json_list(j, ']', skip_item, 0));
		return (json_list(j, '}', skip_member, 0));
	}
	if (c == '-' || isdigit((unsigned char)c))
		return (skip_number(j));
	return (skip_literal(j));
}

static int	skip_item(t_json *j, void *ctx)
{
	(void)ctx;
	return (json_skip_value(j));
}

static int	skip_member(t_json *j, void *ctx)
{
	char	*key;
	int		is_success;

	(void)ctx;
	is_success = json_key(j, &key) && json_skip_value(j);
	free(key);
	return (is_success);
}

static int	skip_number(t_json *j)
{
	size_t	k;
	size_t	digits;

	k = j->i + (j->text[j->i] == '-');
	digits = strspn(j->text + k, "0123456789");
	if (digits && j->text[k + digits] == '.')
	{
		k += digits + 1;
		digits = strspn(j->text + k, "0123456789");
	}
	if (digits && (j->text[k + digits] == 'e' || j->text[k + digits] == 'E'))
	{
		k += digits + 1;
		k += (j->text[k] == '+' || j->text[k] == '-');
		digits = strspn(j->text + k, "0123456789");
	}
	if (!digits)
	{
		j->i = k;
		return (json_error(j, E_JSON_NUMBER));
	}
	j->i = k + digits;
	return (1);
}

static int	skip_literal(t_json *j)
{
	if (!strncmp(j->text + j->i, "true", 4)
		|| !strncmp(j->text + j->i, "null", 4))
		j->i += 4;
	else if (!strncmp(j->text + j->i, "false", 5))
		j->i += 5;
	else
		return (json_error(j, E_JSON_VALUE));
	return (1);
}
