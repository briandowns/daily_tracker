/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Brian J. Downs
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define STR1(x) #x
#define STR(x) STR1(x)

// USAGE contains the application usage.
#define USAGE \
    "usage: %s [-vhf]\n\
    -v          version\n\
    -h          help\n\
    -f          create a new file. default name: yyyy-mm-dd\n"

// DAILY_TRACKER_TEMPLATE contains the base information to be filled out with today's
// and tomorrow's dates.  This is then ready to be used to be further fleshed out.
#define DAILY_TRACKER_TEMPLATE  \
    "### %s Yesterday\n"        \
    "---\n"                     \
    "* \n\n"                    \
    "### %s Today\n"            \
    "---\n"                     \
    "* \n\n"                    \
    "### Blockers\n"            \
    "---\n"                     \
    "* \n\n"                    \
    "### Notes\n"               \
    "---\n"                     \
    "* \n\n"

// DATE_FORMAT for getting the year, month, and day in
// yyyy-mm-dd format.
#define DATE_FORMAT "%d-%02d-%02d" 
#define DATE_BUF_SZ 11

int
main(int argc, char **argv)
{
    int file_output = 0;
    FILE *out;

    int c;
    if (argc > 1) {
        while ((c = getopt(argc, argv, "hvf")) != -1) {
            switch (c) {
                case 'h':
                    printf(USAGE, STR(bin_name));
                    return 0;
                case 'v':
                    printf("%s %s - git: %s\n", STR(bin_name), STR(daily_tracker_version), STR(git_sha));
                    return 0;
                case 'f':
                    file_output = 1;
                    break;
                default:
                    printf(USAGE, STR(bin_name));
                    return 1;
            }
        }
    }

    time_t s = time(NULL);
    struct tm *now = localtime(&s);

    int year = now->tm_year + 1900;
    int month = now->tm_mon + 1;

    char today[DATE_BUF_SZ];
    sprintf(today, DATE_FORMAT, year, month, now->tm_mday);

    // check if the output needs to be written to a file. If so, set the output
    // to the file descriptor derived from "today's" date, otherwise set it to STDOUT.
    if (file_output) {
        out = fopen(today, "w");
    } else {
        out = stdout;
    }

    char yesterday[DATE_BUF_SZ];

    now->tm_mday--;
    sprintf(yesterday, DATE_FORMAT, year, month, now->tm_mday);

    fprintf(out, DAILY_TRACKER_TEMPLATE, yesterday, today);
    fclose(out);

    return 0;
}
