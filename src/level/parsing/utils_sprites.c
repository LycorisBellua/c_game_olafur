#include "cimmerian.h"

static void	is_special(char *line, int *i, int *collec, int *light);

int	get_id(char *line, int *i, t_ivec2 *special, char **id)
{
	int	extra;
	int	tmp;

	extra = 1;
	tmp = 0;
	if (strncmp(line, "sprite_", 7) == 0)
	{
		tmp += 7;
		is_special(line, &tmp, &special->x, &special->y);
		if (isalnum(line[tmp]) == 0)
			return (-1);
		while (line && line[tmp] != ':'
			&& (isalnum(line[tmp]) == 1 || line[tmp] == '_'))
			tmp++;
		if (line && line[tmp] != ':')
			return (-1);
		skip_char(line + tmp, &extra, ' ');
		if (line[tmp + extra] != '\n')
			return (-1);
	}
	*id = ft_substr(line, 0, strchr(line, ':') - line);
	if (!(*id))
		return (-1);
	(*i) += tmp + extra + 1;
	return (0);
}

int	get_pos(char *line, int *start, t_vec2 *pos)
{
	int		tmp;

	tmp = 0;
	if (isdigit(line[tmp]) == 0)
		return (-1);
	pos->x = atod(line + tmp, &tmp);
	if (tmp == -1)
		return (-1);
	skip_char(line, &tmp, ',');
	skip_char(line, &tmp, ' ');
	if (isdigit(line[tmp]) == 0)
		return (-1);
	pos->y = atod(line + tmp, &tmp);
	if (tmp == -1)
		return (-1);
	skip_char(line, &tmp, ' ');
	if (!(line[tmp] == '\0' || line[tmp] == '\n'))
		return (-1);
	*start += tmp + 1;
	return (0);
}

static void	is_special(char *line, int *i, int *collec, int *light)
{
	if (strncmp(line + *i, "collec_", 7) == 0)
	{
		(*i) += 7;
		*collec = 1;
	}
	else
		*collec = 0;
	if (strncmp(line + *i, "light_", 6) == 0)
	{
		(*i) += 6;
		*light = 1;
	}
	else
		*light = 0;
}
