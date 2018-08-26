#include <switch.h>

static Service g_pmdmntSrv;

Result pmdmntAtmosphereGetProcessHandle(Handle *handle_out, u64 pid)
{
    smGetService(&g_pmdmntSrv, "pm:dmnt");

    IpcCommand c;
    ipcInitialize(&c);

    struct
    {
        u64 magic;
        u64 cmd_id;
        u64 pid;
    } * raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));

    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 65000;
    raw->pid = pid;

    Result rc = serviceIpcDispatch(&g_pmdmntSrv);

    if (R_SUCCEEDED(rc))
    {
        IpcParsedCommand r;
        ipcParse(&r);

        struct
        {
            u64 magic;
            u64 result;
        } *resp = r.Raw;

        rc = resp->result;

        if (R_SUCCEEDED(rc)) {
            *handle_out = r.Handles[0];
        }
        
    }
    serviceClose(&g_pmdmntSrv);
    return rc;
}