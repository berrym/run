/**
 * run.c - Fork a subprocess to execute a command.
 *
 * Copyright (c) 2015 Michael Berry <trismegustis@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv)
{
    int i;
    pid_t pid;
    int status;
    char **args = NULL;

    /* Print a usage message */
    if (argc < 2) {
        fprintf(stderr, "usage: %s command args\n", argv[0]);
        exit(EXIT_SUCCESS);
    }

    /* Create a string vector to pass to exec */
    args = calloc(argc, sizeof(char *));
    if (!args) {
        perror("calloc error");
        exit(EXIT_FAILURE);
    }

    /* Fill the string vector */
    for (i = 1; i < argc; i++) {
        size_t line_len = strlen(argv[i]) + 1;
        args[i - 1] = calloc(line_len, sizeof(char));
        if (!args[i - 1]) {
            perror("calloc error");
            exit(EXIT_FAILURE);
        }
        size_t arg_len = strlen(argv[i] + 1);
        strncpy(args[i - 1], argv[i], arg_len);
    }

    /* Fork a process then call execvp on args vector */
    if ((pid = fork()) < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        execvp(args[0], args);
        perror("couldn't execute");
        exit(127);
    } else {
        if (waitpid(pid, &status, WNOHANG) < 0) {
            perror("waitpid error");
            exit(EXIT_FAILURE);
        }
    }

    /* Free args vector */
    for (i = argc - 1; i >= 0; i--) {
        free(args[i]);
        args[i] = NULL;
    }

    free(args);
    args = NULL;

    exit(EXIT_SUCCESS);
}
