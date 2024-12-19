#include "cimmerian.h"

static char	*bypass_field(char *line);

int	is_field(char *line, const char *field)
{
	char	*start_line;
	size_t	len_field;
	int		is_match;

	if (!line || !field)
		return (0);
	start_line = strchr(line, '"');
	if (!start_line)
		return (0);
	++start_line;
	len_field = strlen(field);
	is_match = !strncmp(start_line, field, len_field);
	return (is_match && start_line[len_field] == '"');
}

char	*get_string_value(char *line)
{
	char	*str;
	char	*p_char;

	p_char = bypass_field(line);
	if (p_char)
		p_char = strchr(p_char, '"');
	if (!p_char)
		return (0);
	str = strdup(p_char + 1);
	if (!str)
		return (0);
	p_char = strrchr(str, '"');
	if (p_char)
		*p_char = 0;
	if (!str[0])
	{
		free(str);
		str = 0;
	}
	return (str);
}

int	get_int_value(char *line)
{
	char	*p_char;

	p_char = bypass_field(line);
	if (!p_char)
		return (0);
	return (atoi(p_char));
}

t_ivec2	get_ivec2_value(char *line)
{
	t_ivec2	vec;
	char	*p_char;

	set_ivec2(&vec, 0, 0);
	p_char = bypass_field(line);
	if (p_char)
		p_char = strchr(p_char, '[');
	if (!p_char)
		return (vec);
	vec.x = atoi(++p_char);
	if (!*p_char)
		p_char = 0;
	else
		p_char = strchr(++p_char, ',');
	if (!p_char)
	{
		set_ivec2(&vec, 0, 0);
		return (vec);
	}
	while (isspace(*p_char))
		++p_char;
	vec.y = atoi(p_char);
	return (vec);
}

static char	*bypass_field(char *line)
{
	char	*p_char;

	if (!line)
		return (0);
	p_char = strchr(line, '"');
	if (!p_char)
		return (strchr(line, '['));
	p_char = strchr(++p_char, '"');
	if (!p_char)
		return (0);
	p_char = strchr(++p_char, ':');
	if (!p_char)
		return (0);
	++p_char;
	while (isspace(*p_char))
		++p_char;
	return (p_char);
}
