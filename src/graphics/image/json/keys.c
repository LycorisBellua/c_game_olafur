#include "olafur.h"

int	json_key(t_json *j, char **key)
{
	*key = 0;
	json_peek(j);
	j->key_pos = j->i;
	if (!json_string(j, key))
		return (0);
	return (json_expect(j, ':', E_JSON_COLON));
}

int	json_unknown_field(t_json *j, const char *key)
{
	char	msg[PATH_MSG_SIZE];

	if (!JSON_REJECT_UNKNOWN_FIELDS)
		return (json_skip_value(j));
	else if (!key)
		key = "";
	snprintf(msg, PATH_MSG_SIZE, "%s \"%s\"", E_JSON_UNKNOWN, key);
	j->i = j->key_pos;
	return (json_error(j, msg));
}
