#include "olafur.h"
#ifdef _WIN32
# include <windows.h>
#endif

static void	add_to_error_box(const char *msg, const char *arg);

/*
	On Windows, the game has no console to print to, so errors are also 
	gathered and shown together in a message box when the game exits.
*/
int	put_error(const char *msg, const char *arg, int errnum)
{
	if (msg)
	{
		fprintf(stderr, "Error: %s", msg);
		if (arg)
			fprintf(stderr, ": %s", arg);
		fprintf(stderr, "\n");
		add_to_error_box(msg, arg);
	}
	return (errnum);
}

#ifdef _WIN32

static void	show_error_box(void);

/* The first time the text is used, the box is set to show at exit. */
static char	*get_error_box_text(void)
{
	static char	text[ERROR_BOX_SIZE];
	static int	is_box_set;

	if (!is_box_set)
	{
		atexit(show_error_box);
		is_box_set = 1;
	}
	return (text);
}

/* The text is converted from UTF-8 so that accents and symbols show. */
static void	show_error_box(void)
{
	wchar_t	text[ERROR_BOX_SIZE];

	if (MultiByteToWideChar(CP_UTF8, 0, get_error_box_text(), -1, text,
		ERROR_BOX_SIZE))
		MessageBoxW(NULL, text, L"" TITLE, MB_OK | MB_ICONERROR);
	return ;
}

/* A message is only added if the same line isn't already there. */
static void	add_to_error_box(const char *msg, const char *arg)
{
	char		*text;
	char		*p;
	const char	*end;

	text = get_error_box_text();
	p = strstr(text, msg);
	while (p)
	{
		end = p + strlen(msg);
		if (arg && !strncmp(end, ": ", 2)
			&& !strncmp(end + 2, arg, strlen(arg)))
			end += 2 + strlen(arg);
		else if (arg)
			end = "";
		if (*end == '\n' && (p == text || p[-1] == '\n'))
			return ;
		p = strstr(p + 1, msg);
	}
	p = text + strlen(text);
	if (arg)
		snprintf(p, ERROR_BOX_SIZE - (p - text), "%s: %s\n", msg, arg);
	else
		snprintf(p, ERROR_BOX_SIZE - (p - text), "%s\n", msg);
	return ;
}
#else

static void	add_to_error_box(const char *msg, const char *arg)
{
	(void)msg;
	(void)arg;
	return ;
}
#endif
