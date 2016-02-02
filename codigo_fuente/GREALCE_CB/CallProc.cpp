#include <stdio.h>
#include <windows.h>

#define BUFSIZE 4096

HANDLE hChildStdinRd, hChildStdinWr, hChildStdinWrDup,
    hChildStdoutRd, hChildStdoutWr,
    hInputFile, hSaveStdin, hSaveStdout;

BOOL CreateChildProcess(VOID);
VOID WriteToPipe(VOID);
VOID ReadFromPipe(VOID);
VOID ErrorExit(LPTSTR);
VOID ErrMsg(LPTSTR, BOOL);

DWORD main(int argc, char *argv[]) {
    SECURITY_ATTRIBUTES saAttr;
    BOOL fSuccess;

    /* Set the bInheritHandle flag so pipe handles are inherited. */


    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    /*
     * The steps for redirecting child's STDOUT:
     *     1.  Save current STDOUT, to be restored later.
     *     2.  Create anonymous pipe to be STDOUT for child.
     *     3.  Set STDOUT of parent to be write handle of pipe, so
     *         it is inherited by child.
     */ 
 
    /* Save the handle to the current STDOUT. */ 

 
    hSaveStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
 
    /* Create a pipe for the child's STDOUT. */ 
 
    if (! CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0)) 
        ErrorExit("Stdout pipe creation failed\n"); 
 
    /* Set a write handle to the pipe to be STDOUT. */ 
 
    if (! SetStdHandle(STD_OUTPUT_HANDLE, hChildStdoutWr)) 
        ErrorExit("Redirecting STDOUT failed"); 
 
    /* 
     * The steps for redirecting child's STDIN: 
     *     1.  Save current STDIN, to be restored later. 

     *     2.  Create anonymous pipe to be STDIN for child. 
     *     3.  Set STDIN of parent to be read handle of pipe, so 
     *         it is inherited by child.
     *     4.  Create a noninheritable duplicate of write handle,
     *         and close the inheritable write handle. 
     */ 
 
    /* Save the handle to the current STDIN. */ 
 
    hSaveStdin = GetStdHandle(STD_INPUT_HANDLE); 
 
    /* Create a pipe for the child's STDIN. */ 
 
    if (! CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0)) 

        ErrorExit("Stdin pipe creation failed\n"); 
 
    /* Set a read handle to the pipe to be STDIN. */ 
 
    if (! SetStdHandle(STD_INPUT_HANDLE, hChildStdinRd)) 
        ErrorExit("Redirecting Stdin failed"); 
 
    /* Duplicate the write handle to the pipe so it is not inherited. */ 
 
    fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdinWr, 
        GetCurrentProcess(), &hChildStdinWrDup, 0, 
        FALSE,       /* not inherited */ 
        DUPLICATE_SAME_ACCESS);

    if (! fSuccess)
        ErrorExit("DuplicateHandle failed");

    CloseHandle(hChildStdinWr);

    /* Now create the child process. */

    if (! CreateChildProcess())
        ErrorExit("Create process failed");

    /* After process creation, restore the saved STDIN and STDOUT. */

    if (! SetStdHandle(STD_INPUT_HANDLE, hSaveStdin))
        ErrorExit("Re-redirecting Stdin failed\n");

    if (! SetStdHandle(STD_OUTPUT_HANDLE, hSaveStdout))

        ErrorExit("Re-redirecting Stdout failed\n");

    /* Get a handle to the parent's input file. */
 
    if (argc > 1) 
        hInputFile = CreateFile(argv[1], GENERIC_READ, 0, NULL, 
            OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
    else 
        hInputFile = hSaveStdin; 
 
    if (hInputFile == INVALID_HANDLE_VALUE) 
        ErrorExit("no input file\n"); 
 
    /* Write to pipe that is the standard input for a child process. */ 

    WriteToPipe();


    /* Read from pipe that is the standard output for child process. */

    ReadFromPipe();

    return 0;
}

BOOL CreateChildProcess() {
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;

    /* Set up members of STARTUPINFO structure. */

    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.lpReserved = NULL;
    siStartInfo.lpReserved2 = NULL;
    siStartInfo.cbReserved2 = 0;
    siStartInfo.lpDesktop = NULL;
    siStartInfo.dwFlags = 0;


    /* Create the child process. */

    return CreateProcess(NULL,
        "child",       /* command line                       */
        NULL,          /* process security attributes        */
        NULL,          /* primary thread security attributes */
        TRUE,          /* handles are inherited              */
        0,             /* creation flags                     */
        NULL,          /* use parent's environment           */
        NULL,          /* use parent's current directory     */

        &siStartInfo,  /* STARTUPINFO pointer                */
        &piProcInfo);  /* receives PROCESS_INFORMATION       */

}
VOID WriteToPipe(VOID) {
    DWORD dwRead, dwWritten; 
    CHAR chBuf[BUFSIZE];
 
    /* Read from a file and write its contents to a pipe. */ 
 
    for (;;) { 
        if (! ReadFile(hInputFile, chBuf, BUFSIZE, &dwRead, NULL) || 
            dwRead == 0) break; 
        if (! WriteFile(hChildStdinWrDup, chBuf, dwRead, 
            &dwWritten, NULL)) break; 

    } 
 
    /* Close the pipe handle so the child stops reading. */ 
 
    if (! CloseHandle(hChildStdinWrDup)) 
        ErrorExit("Close pipe failed\n"); 
} 
 
VOID ReadFromPipe(VOID) { 
    DWORD dwRead, dwWritten; 
    CHAR chBuf[BUFSIZE]; 
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 

    /* 
     * Close the write end of the pipe before reading from the 
     * read end of the pipe.
     */

    if (! CloseHandle(hChildStdoutWr))
        ErrorExit("Closing handle failed");


    /* Read output from child, and write it to parent's STDOUT. */

    for (;;) {
        if (! ReadFile(hChildStdoutRd, chBuf, BUFSIZE, &dwRead, NULL) ||
            dwRead == 0) break; 
        if (! WriteFile(hSaveStdout, chBuf, dwRead, &dwWritten, NULL)) 
            break; 
    } 
} 
 
VOID ErrorExit (LPTSTR lpszMessage) { 
    fprintf(stderr, "%s\n", lpszMessage); 
    ExitProcess(0); 
} 
 


