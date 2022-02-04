
#define CMD_BUF_SIZE (512)

void CmdShell_TaskInit(void);
void CmdShell_Task(void *params);
void CmdShell_AppendToCommandBuf(char *buf, uint16_t len);
