#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <poll.h>

#include <curl/curl.h>

#include "picoc.h"

#include "cheat.h"
#include "util.h"

Semaphore done;

char *line;

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

CURLcode dlUrlToFile(char *url, char *path)
{
    CURL *curl;
    FILE *fp;
    CURLcode res = CURLE_FAILED_INIT;
    curl = curl_easy_init();
    if (curl)
    {
        fp = fopen(path, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    else
        return CURLE_FAILED_INIT;
    return res;
}

char *runPath;

/*void luaHookFunc(lua_State *L, lua_Debug *ar)
{
    if (ar->event == LUA_HOOKLINE)
        if (semaphoreTryWait(&done))
            luaL_error(L, "Sucessfully terminated lua-script!");
    svcSleepThread(1);
}*/

#define PICOC_STACK_SIZE (128 * 1024) /* space for the the stack */


void picocRunner(void *runPath)
{
    Picoc pc;

    PicocInitialise(&pc, PICOC_STACK_SIZE);

    // If picoc fails it will jump here and the check will go through.
    if (!PicocPlatformSetExitPoint(&pc))
    {
        PicocPlatformScanFile(&pc, (char*) runPath);
        PicocCallMain(&pc, 0, 0);
    }

    PicocCleanup(&pc);

    printf("Done!\n");
    semaphoreSignal(&done);
}

int picocRunPath(char *path)
{
    detach();
    mutexUnlock(&actionLock);
    if (!strncmp(path, "http://", 7))
    {
        CURLcode res = dlUrlToFile(path, "/netcheat/dl.lua");
        if (res != CURLE_OK)
        {
            printf("Failed to dl file :/\r\n");
            return 1;
        }

        path = "/netcheat/dl.lua";
    }
    runPath = path;

    semaphoreInit(&done, 0);

    //    lua_sethook(L, &luaHookFunc, LUA_MASKLINE, 0);

    Thread picocThread;
    Result rc = threadCreate(&picocThread, picocRunner, runPath, 0x4000, 49, 3);
    if (R_FAILED(rc))
        fatalLater(rc);
    threadStart(&picocThread);

    line = malloc(MAX_LINE_LENGTH);
    line[0] = 0;

    struct pollfd fd;
    fd.fd = sock;
    fd.events = POLLIN;
    int ret;

    while (!semaphoreTryWait(&done))
    {
        svcSleepThread(300000000L);
        ret = poll(&fd, 1, 200);
        mutexLock(&actionLock);
        switch (ret)
        {
        case -1:
            semaphoreSignal(&done);
            mutexUnlock(&actionLock);
            goto done;
            break;
        case 0:
            break;
        default:
        {
            int len = recv(sock, line, MAX_LINE_LENGTH, 0);
            if (len <= 0)
            {
                printf("Receive failed!\r\n");
                semaphoreSignal(&done);
                mutexUnlock(&actionLock);
                goto done;
            }
            line[len - 1] = 0;
            if (!strcmp(line, "stop"))
            {
                printf("Aborting script!\r\n");
                line[0] = 0;
                semaphoreSignal(&done);
                mutexUnlock(&actionLock);
                goto done;
            }
            break;
        }
        }
        mutexUnlock(&actionLock);
    }

done:

    threadWaitForExit(&picocThread);

    free(line);

    return 0;
}
