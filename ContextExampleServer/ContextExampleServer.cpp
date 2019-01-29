// File ContextExampleServer.cpp
#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sys/stat.h>


#include "../ContextExample/ContextExample.h"

using namespace std;

// Write a formatted error message to std::cerr.
DWORD HandleError(const char* szFunction, DWORD dwError)
{
   void* pBuffer = NULL;
   FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
      FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
      NULL,
      dwError,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      LPTSTR(&pBuffer),
      0,
      NULL);

   std::cerr << szFunction << " failed. "<< (pBuffer ? LPCTSTR(pBuffer) : "Unknown error. ")<< "(" << dwError << ")" << std::endl;
   LocalFree(pBuffer);
   return dwError;
}

CONTEXT_HANDLE Open(
   /* [in] */ handle_t hBinding,
   /* [string][in] */ const char* szString)
{
   std::string* pContext = new std::string(szString);
   CONTEXT_HANDLE hContext = pContext;
   cout << endl << " Connection Established with Client handle id = " << hContext;
   return hContext;
}

int CreateNewFile(/* [in] */ CONTEXT_HANDLE hContext,
    /* [string][in] */ const char* filePath = ".\\Test.txt", 
    /* [string][in] */ const char* Text = __TIMESTAMP__)
{
    std::fstream fileIO(filePath, ios::out | ios::app);
    
    if (fileIO.is_open())
    {   //fileIO.open(filePath, ios::out | ios::app);

        fileIO << Text << endl;
        fileIO.close();
    }

    return GetLastError();
}

int CopyPaste(CONTEXT_HANDLE hContext, const char* sourcePath, const char* destinationPath) 
{
    std::clog << "\n****** Calling Copy Paste Functionality ******";
    int status = CopyFile(sourcePath, destinationPath, false);
    if (status == 0)
        std::clog << "\n****** " << GetLastError() << "******";
    else
        std::clog << "\n****** File Pasted Successfully ******";
    return GetLastError();
}

int DeleteFileOnPath(CONTEXT_HANDLE hContext, const char* Path)
{
    int status=-1;
    ifstream file(Path);
    if (file.good())
    {
        file.close();
        status = remove(Path);
        cout << "\n status after calling remove = " << status;
    }
    if (status == 0)
        cout << "\n****** File Deleted Successufully ******";
    
    return GetLastError();
}

int getFileInformation(CONTEXT_HANDLE hContext, const char* Path, ::WIN32_FIND_DATA* metaData)
{
    HANDLE fileHandle = FindFirstFile(Path, metaData);
    
    if (fileHandle != INVALID_HANDLE_VALUE)
    {
        return GetLastError();
    }
    else
    {
        FindClose(fileHandle);
        return true;
    }

}


void Output(/* [in] */ CONTEXT_HANDLE hContext)
{
   std::clog << "Output: Context = " << hContext << std::endl;
   std::string* pContext = static_cast<std::string*>(hContext);
   std::cout << *pContext << std::endl;
}

void Close(/* [out][in] */ CONTEXT_HANDLE* phContext)
{
   std::clog << "Close: Context = " << *phContext << std::endl;
   std::string* pContext = static_cast<std::string*>(*phContext);
   delete pContext;

   //  must set the context handle to NULL, or else  will get
   // a rundown later anyway.
   *phContext = NULL;
}

// The RPC runtime will call this function if the connection to the client
// is lost.
void __RPC_USER CONTEXT_HANDLE_rundown(CONTEXT_HANDLE hContext)
{
   std::clog << "CONTEXT_HANDLE_rundown: Context = " << hContext << std::endl;
   Close(&hContext);
}

// The thread that will listen for incoming RPC calls.
DWORD WINAPI RpcServerListenThreadProc(LPVOID /*pParam*/)
{

   return RpcServerListen(
      1, // Recommended minimum number of threads.
      RPC_C_LISTEN_MAX_CALLS_DEFAULT, // Recommended maximum number of threads.
      FALSE); // Start listening now.
}

// Naive security callback.
RPC_STATUS CALLBACK SecurityCallback(RPC_IF_HANDLE /*hInterface*/, void* /*pBindingHandle*/)
{
    return RPC_S_OK; // Always allow anyone.
}

int main()
{
   RPC_STATUS status;

   status = RpcServerUseProtseqEp(
      reinterpret_cast<unsigned char*>("ncacn_ip_tcp"), // Use TCP/IP protocol.
      RPC_C_PROTSEQ_MAX_REQS_DEFAULT, // Backlog queue length for TCP/IP.
      reinterpret_cast<unsigned char*>("4747"), // TCP/IP port to use.
      NULL); // No security.
   
   if (status)
      return HandleError("RpcServerUseProtseqEp", status);

   // Registers the ContextExample interface.
   status = RpcServerRegisterIf2(
      ContextExample_v1_0_s_ifspec, // Interface to register.
      NULL, // Use the MIDL generated entry-point vector.
      NULL, // Use the MIDL generated entry-point vector.
      RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH, // Forces use of security callback.
      RPC_C_LISTEN_MAX_CALLS_DEFAULT, // Use default number of concurrent calls.
      (unsigned)-1, // Infinite max size of incoming data blocks.
      SecurityCallback); // Naive security callback.
  
   if (status)
      return HandleError("RpcServerRegisterIf", status);

   std::clog << "Creating listen thread" << std::endl;
   const HANDLE hThread = CreateThread(NULL, 0, RpcServerListenThreadProc,
      NULL, 0, NULL);
   if (!hThread)
      return HandleError("CreateThread", GetLastError());

   std::cout << "Press enter to stop listening" << std::endl;
   std::cin.get();

   status = RpcMgmtStopServerListening(NULL);
   if (status)
      return HandleError("RpcMgmtStopServerListening", status);

   std::clog << "Waiting for listen thread to finish";
   while (WaitForSingleObject(hThread, 1000) == WAIT_TIMEOUT)
      std::clog << '.';
   std::clog << std::endl << "Listen thread finished" << std::endl;

   DWORD dwExitCodeThread = 0;
   GetExitCodeThread(hThread, &dwExitCodeThread);
   CloseHandle(hThread);
   if (dwExitCodeThread)
      return HandleError("RpcServerListen", dwExitCodeThread);

   std::cout << "Press enter to exit" << std::endl;
   std::cin.get();
}

// Memory allocation function for RPC.
void* __RPC_USER midl_user_allocate(size_t size)
{
   return malloc(size);
}

// Memory deallocation function for RPC.
void __RPC_USER midl_user_free(void* p)
{
   free(p);
}
