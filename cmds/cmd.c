/*
 * Phoenix-RTOS
 *
 * plo - perating system loader
 *
 * Loader commands
 *
 * Copyright 2012, 2017, 2020-2021 Phoenix Systems
 * Copyright 2001, 2005 Pawel Pisarczyk
 * Author: Pawel Pisarczyk, Pawel Kolodziej, Hubert Buczynski, Gerard Swiderski
 *
 * This file is part of Phoenix-RTOS.
 *
 * %LICENSE%
 */

#include "cmd.h"
#include "hal.h"
#include "lib.h"
#include "console.h"


#define SIZE_HIST            8
#define SIZE_CMDS            24
#define CONSOLE_TIMEOUT_MS   200

#define PROMPT              "(plo)% "


/* Linker symbol points to the beginning of .data section */
extern char script[];

struct {
	const cmd_t *cmds[SIZE_CMDS];

	int ll;
	int cl;
	char lines[SIZE_HIST][SIZE_CMD_ARG_LINE + 1];
} cmd_common;


void cmd_reg(const cmd_t *cmd)
{
	unsigned int i;

	for (i = 0; i < SIZE_CMDS; ++i) {
		if (cmd_common.cmds[i] != NULL) {
			if (hal_strcmp(cmd_common.cmds[i]->name, cmd->name) == 0)
				return;

			continue;
		}

		cmd_common.cmds[i] = cmd;
		break;
	}
}


void cmd_run(void)
{
	lib_printf("\ncmd: Executing pre-init script");
	cmd_parse((char *)script, 0);
}


const cmd_t *cmd_getCmd(unsigned int id)
{
	if (id >= SIZE_CMDS)
		return NULL;

	return cmd_common.cmds[id];
}


/* TODO: old code needs to be cleaned up */
void cmd_parse(char *line, int show)
{
	unsigned int p = 0, wp, i;
	char word[SIZE_CMD_ARG_LINE + 1], cmd[SIZE_CMD_ARG_LINE + 1];

	for (;;) {
		if (cmd_getnext(line, &p, "\n", DEFAULT_CITES, word, sizeof(word)) == NULL) {
			lib_printf("\nSyntax error!\n");
			return;
		}
		if (*word == 0)
			 break;
		if (show)
			lib_printf("\n%s", word);
		wp = 0;
		if (cmd_getnext(word, &wp, DEFAULT_BLANKS, DEFAULT_CITES, cmd, sizeof(cmd)) == NULL) {
			lib_printf("\nSyntax error!\n");
			return;
		}

		/* Find command and launch associated function */
		for (i = 0; i < SIZE_CMDS; i++) {
			if (hal_strcmp(cmd, cmd_common.cmds[i]->name) != 0)
				continue;

			if (cmd_common.cmds[i]->run(word + wp) < 0)
				return;

			break;
		}
		if (i >= SIZE_CMDS)
			lib_printf("\n'%s' - unknown command!\n", cmd);
	}
}


void cmd_skipblanks(char *line, unsigned int *pos, char *blanks)
{
	char *p;

	while (line[*pos]) {
		for (p = blanks; *p && line[*pos] != *p; p++);
		if (!*p)
			break;
		(*pos)++;
	}
}


/* TODO: old code needs to be cleaned up */
char *cmd_getnext(char *line, unsigned int *pos, char *blanks, char *cites, char *word, unsigned int len)
{
	char citefl = 0, c;
	unsigned int i, wp = 0;

	/* Skip leading blank characters */
	cmd_skipblanks(line, pos, blanks);

	wp = 0;
	while ((c = line[*pos]) != 0) {

		/* Test cite characters */
		if (cites) {
			for (i = 0; cites[i]; i++) {
				if (c == cites[i]) {
					citefl ^= 1;
					break;
				}
			}

			/* Go to next iteration if cite character found */
			if (cites[i]) {
				(*pos)++;
				continue;
			}
		}

		/* Test separators */
		for (i = 0; blanks[i]; i++) {
			if (c == blanks[i])
				break;
		}
		if (!citefl && blanks[i])
			break;

		word[wp++] = c;
		if (wp == len)
			return NULL;

		(*pos)++;
	}

	if (citefl)
		return NULL;

	word[wp] = 0;

	return word;
}


/* TODO: old code needs to be cleaned up */
int cmd_parseArgs(char *s, char (*args)[SIZE_CMD_ARG_LINE + 1], u16 *argsc, u16 maxArgNb)
{
	int i;
	unsigned int pos = 0;

	for (i = 0; *argsc < maxArgNb; ++i) {
		cmd_skipblanks(s, &pos, DEFAULT_BLANKS);
		if (cmd_getnext(s, &pos, DEFAULT_BLANKS, NULL, args[i], sizeof(args[i])) == NULL || *args[i] == 0)
			break;
		(*argsc)++;
	}

	if (!*argsc)
		return ERR_ARG;

	return 0;
}


/* TODO: old code needs to be cleaned up */
void cmd_prompt(void)
{
	char c = 0, sc = 0;
	int pos = 0;
	int i, k, chgfl = 0, ncl;

	cmd_common.ll = 0;
	cmd_common.cl = 0;

	for (k = 0; k < SIZE_HIST; k++)
		cmd_common.lines[k][0] = 0;

	lib_printf("\n%s", PROMPT);
	while (c != '#') {
		while (console_getc(&c, CONSOLE_TIMEOUT_MS) <= 0)
			;

		sc = 0;
		/* Translate backspace */
		if (c == 127) {
			c = 8;
		}
		/* Simple parser for VT100 commands */
		else if (c == 27) {
			while (console_getc(&c, CONSOLE_TIMEOUT_MS) <= 0)
				;

			switch (c) {
				case 91:
					while (console_getc(&c, CONSOLE_TIMEOUT_MS) <= 0)
						;

					switch (c) {
						case 'A': /* UP */
							sc = 72;
							break;
						case 'B': /* DOWN */
							sc = 80;
							break;
					}
					break;
			}
			c = 0;
		}

		/* Regular characters */
		if (c) {
			if (c == '\r') {
				if (pos) {
					cmd_common.lines[cmd_common.ll][pos] = 0;

					cmd_parse(cmd_common.lines[cmd_common.ll], 0);

					cmd_common.ll = (cmd_common.ll + 1) % SIZE_HIST;
					cmd_common.cl = cmd_common.ll;
				}
				pos = 0;
				lib_printf("\n%s", PROMPT);
				continue;
			}

			/* If character isn't backspace add it to line buffer */
			if ((c != 8) && (pos < SIZE_CMD_ARG_LINE)) {
				console_putc(c);
				cmd_common.lines[cmd_common.ll][pos++] = c;
				cmd_common.lines[cmd_common.ll][pos] = 0;
			}

			/* Remove character before cursor */
			if ((c == 8) && (pos > 0)) {
				cmd_common.lines[cmd_common.ll][--pos] = 0;
				lib_printf("%c %c", 8, 8);
			}
		}
		/* Control characters */
		else {
			switch (sc) {
				case 72:
					ncl = ((cmd_common.cl + SIZE_HIST - 1) % SIZE_HIST);
					if ((ncl != cmd_common.ll) && (cmd_common.lines[ncl][0])) {
						cmd_common.cl = ncl;
						hal_memcpy(cmd_common.lines[cmd_common.ll], cmd_common.lines[cmd_common.cl], hal_strlen(cmd_common.lines[cmd_common.cl]) + 1);
						chgfl = 1;
					}
					break;

				case 80:
					ncl = ((cmd_common.cl + 1) % SIZE_HIST);
					if (cmd_common.cl != cmd_common.ll) {
						cmd_common.cl = ncl;
						chgfl = 1;

						if (ncl != cmd_common.ll)
							hal_memcpy(cmd_common.lines[cmd_common.ll], cmd_common.lines[cmd_common.cl], hal_strlen(cmd_common.lines[cmd_common.cl]) + 1);
						else
							cmd_common.lines[cmd_common.ll][0] = 0;
					}
					break;
			}

			if (chgfl) {
				lib_printf("\r%s", PROMPT);
				for (i = 0; i < pos; i++)
					lib_printf(" ");

				pos = hal_strlen(cmd_common.lines[cmd_common.ll]);
				lib_printf("\r%s", PROMPT);
				lib_printf("%s", cmd_common.lines[cmd_common.ll]);
				chgfl = 0;
			}
		}
	}
}