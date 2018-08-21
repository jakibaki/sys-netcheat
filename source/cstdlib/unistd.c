/* stdlib.h library for large systems - small embedded systems use clibrary.c instead */
#include <switch.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include "../interpreter.h"

#ifndef BUILTIN_MINI_STDLIB

static int ZeroValue = 0;

void UnistdAccess(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = access(Param[0]->Val->Pointer, Param[1]->Val->Integer);
}

void UnistdChdir(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = chdir(Param[0]->Val->Pointer);
}

void UnistdClose(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = close(Param[0]->Val->Integer);
}

#if 0
void UnistdCuserid(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = cuserid(Param[0]->Val->Pointer);
}
#endif

void UnistdDup(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = dup(Param[0]->Val->Integer);
}

void UnistdDup2(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = dup2(Param[0]->Val->Integer, Param[1]->Val->Integer);
}

void Unistd_Exit(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    _exit(Param[0]->Val->Integer);
}

void UnistdFork(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = fork();
}

void UnistdFsync(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = fsync(Param[0]->Val->Integer);
}

void UnistdFtruncate(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = ftruncate(Param[0]->Val->Integer, Param[1]->Val->Integer);
}

void UnistdGetcwd(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = getcwd(Param[0]->Val->Pointer, Param[1]->Val->Integer);
}

void UnistdGethostid(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = gethostid();
}


#if 0
void UnistdGetpgid(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = getpgid(Param[0]->Val->Integer);
}
#endif

void UnistdGetpgrp(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = getpgrp();
}

void UnistdGetpid(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = getpid();
}

#if 0
void UnistdGetsid(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = getsid(Param[0]->Val->Integer);
}
#endif

void UnistdGetwd(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = getcwd(Param[0]->Val->Pointer, PATH_MAX);
}

void UnistdIsatty(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = isatty(Param[0]->Val->Integer);
}

void UnistdLink(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = link(Param[0]->Val->Pointer, Param[1]->Val->Pointer);
}

void UnistdLseek(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = lseek(Param[0]->Val->Integer, Param[1]->Val->Integer, Param[2]->Val->Integer);
}

#if 0
void UnistdPread(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = pread(Param[0]->Val->Integer, Param[1]->Val->Pointer, Param[2]->Val->Integer, Param[3]->Val->Integer);
}

void UnistdPwrite(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = pwrite(Param[0]->Val->Integer, Param[1]->Val->Pointer, Param[2]->Val->Integer, Param[3]->Val->Integer);
}
#endif

void UnistdRead(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = read(Param[0]->Val->Integer, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}


void UnistdRmdir(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = rmdir(Param[0]->Val->Pointer);
}

void UnistdSbrk(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = sbrk(Param[0]->Val->Integer);
}

void UnistdSleep(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = sleep(Param[0]->Val->Integer);
}

#if 0
void UnistdSwab(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = swab(Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}
#endif

void UnistdTruncate(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = truncate(Param[0]->Val->Pointer, Param[1]->Val->Integer);
}

void UnistdUnlink(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = unlink(Param[0]->Val->Pointer);
}

void UnistdUsleep(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = usleep(Param[0]->Val->Integer);
}

void UnistdWrite(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = write(Param[0]->Val->Integer, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}


/* handy structure definitions */
const char UnistdDefs[] = "\
typedef int uid_t; \
typedef int gid_t; \
typedef int pid_t; \
typedef int off_t; \
typedef int size_t; \
typedef int ssize_t; \
typedef int useconds_t;\
typedef int intptr_t;\
";

/* all unistd.h functions */
struct LibraryFunction UnistdFunctions[] =
{
    { UnistdAccess,        "int access(char *, int);" },
/*    { UnistdBrk,           "int brk(void *);" }, */
    { UnistdChdir,         "int chdir(char *);" },
    { UnistdClose,         "int close(int);" },
/*    { UnistdCuserid,       "char *cuserid(char *);" }, */
    { UnistdDup,           "int dup(int);" },
    { UnistdDup2,          "int dup2(int, int);" },
/*     { UnistdEncrypt,       "void encrypt(char[64], int);" }, */
/*    { UnistdExecl,         "int execl(char *, char *, ...);" }, */
/*    { UnistdExecle,        "int execle(char *, char *, ...);" }, */
/*    { UnistdExeclp,        "int execlp(char *, char *, ...);" }, */
/*    { UnistdExecv,         "int execv(char *, char *[]);" }, */
/*    { UnistdExecve,        "int execve(char *, char *[], char *[]);" }, */
/*    { UnistdExecvp,        "int execvp(char *, char *[]);" }, */
    { Unistd_Exit,         "void _exit(int);" },
    { UnistdFork,          "pid_t fork(void);" },
    { UnistdFtruncate,     "int ftruncate(int, off_t);" },
    { UnistdGetcwd,        "char *getcwd(char *, size_t);" },
/*    { UnistdGetgroups,     "int getgroups(int, gid_t []);" }, */
    { UnistdGethostid,     "long gethostid(void);" },
/*    { UnistdGetopt,        "int getopt(int, char * [], char *);" }, */
/*    { UnistdGetpgid,       "pid_t getpgid(pid_t);" }, */
    { UnistdGetpid,        "pid_t getpid(void);" },
/*    { UnistdGetsid,        "pid_t getsid(pid_t);" }, */
    { UnistdGetwd,         "char *getwd(char *);" },
    { UnistdIsatty,        "int isatty(int);" },
    { UnistdLink,          "int link(char *, char *);" },
    { UnistdLseek,         "off_t lseek(int, off_t, int);" },
/*    { UnistdPipe,          "int pipe(int [2]);" }, */
/*    { UnistdPread,         "ssize_t pread(int, void *, size_t, off_t);" }, */
/*    { UnistdPthread_atfork,"int pthread_atfork(void (*)(void), void (*)(void), void(*)(void));" }, */
/*    { UnistdPwrite,        "ssize_t pwrite(int, void *, size_t, off_t);" }, */
    { UnistdRead,          "ssize_t read(int, void *, size_t);" },
    { UnistdRmdir,         "int rmdir(char *);" },
    { UnistdSbrk,          "void *sbrk(intptr_t);" },
    { UnistdSleep,         "unsigned int sleep(unsigned int);" },
/*    { UnistdSwab,          "void swab(void *, void *, ssize_t);" }, */
    { UnistdTruncate,      "int truncate(char *, off_t);" },
    { UnistdUnlink,        "int unlink(char *);" },
    { UnistdUsleep,        "int usleep(useconds_t);" },
    { UnistdWrite,         "ssize_t write(int, void *, size_t);" },
    { NULL,                 NULL }
};

/* creates various system-dependent definitions */
void UnistdSetupFunc(Picoc *pc)
{
    /* define NULL */
    if (!VariableDefined(pc, TableStrRegister(pc, "NULL")))
        VariableDefinePlatformVar(pc, NULL, "NULL", &pc->IntType, (union AnyValue *)&ZeroValue, FALSE);

    /* define optarg and friends */
    VariableDefinePlatformVar(pc, NULL, "optarg", pc->CharPtrType, (union AnyValue *)&optarg, TRUE);
    VariableDefinePlatformVar(pc, NULL, "optind", &pc->IntType, (union AnyValue *)&optind, TRUE);
    VariableDefinePlatformVar(pc, NULL, "opterr", &pc->IntType, (union AnyValue *)&opterr, TRUE);
    VariableDefinePlatformVar(pc, NULL, "optopt", &pc->IntType, (union AnyValue *)&optopt, TRUE);
}

#endif /* !BUILTIN_MINI_STDLIB */

