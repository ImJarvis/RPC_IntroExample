// File ContextExampleClient.cpp
#include <iostream>
#include <string>
#include "Logger.h"
#include "../ContextExample/ContextExample.h"


using namespace std;
#define mylog Logger::getLoogerObject() 
// Write a formatted error message to std::cerr.
DWORD HandleError(const char* szFunction, DWORD dwError)
{
   void* pBuffer = NULL;
   FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
      FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
      NULL,
      dwError,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      LPSTR(&pBuffer),
      0,
      NULL);

   std::cerr << szFunction << " failed. "
      << (pBuffer ? LPCSTR(pBuffer) : "Unknown error. ")
      << "(" << dwError << ")" << std::endl;
   LocalFree(pBuffer);
   return dwError;
}

// local functions 

int cpaste(CONTEXT_HANDLE& hContext)
{

    std::clog << " Calling CopyPaste " << std::endl;
    std::string sourcePath, destPath;
    cout << endl << "Enter Source Path File to get Copied\n ";
    getline(cin, sourcePath);
    cout << endl << "Enter Destination Path File to put the file\n ";
    getline(cin, destPath);
    
    return CopyPaste(hContext, sourcePath.c_str(), destPath.c_str());

}


int getProperty(CONTEXT_HANDLE& hContext, WIN32_FIND_DATA* data)
{
    string filepath;
    cout <<endl<< " Enter Path of the file to get the properties ";
    getline(cin, filepath);
    
    return getFileInformation(hContext, filepath.c_str(), data);
}

int CreateMyFile(CONTEXT_HANDLE& hContext)
{
    cout << endl << " Enter File Name with Path where you want to create it " << endl;
    string path, text;
    getline(cin, path);

    cout << endl << "Enter Text you want to write in the file " << endl;
    getline(cin, text);

    return CreateNewFile(hContext, path.c_str(), text.c_str());
    
}


void callmenu(handle_t hBinding)
{
    //fflush(stdout);
    /*cout << endl<< "Choose from the following Options -";
    cout << endl << "Enter 1 - For creating a file";
    cout << endl << "Enter 2 - For Copy and Paste an exisiting File";
    cout << endl << "Enter 3 - For Deleting an Existing File";
    cout << endl << "Enter 4 - For getting the properties of an exisiting file";
    cout << endl << "Enter 0 - For Closing Client Connection";
    cout << endl << "Enter --- ";*/

    // Open the context handle.
    CONTEXT_HANDLE hContext = Open(hBinding, "UbiSoft_Test");
    mylog << "Connection Established with Server with context Id = "<<(long)hContext;
    cout << "Press enter to  Create New File";
    std::cin.get();
    mylog<< " Calling CreateNewFile ";

    // Calls the RPC function. The hBinding binding handle is used explicitly.
    mylog << "Status of Creating New File is = " <<CreateMyFile(hContext);
    mylog << "Calling Copy Paste ";
    mylog<< "Status of CopyPaste is = " << cpaste(hContext);

    mylog<< "Calling getFileProperty ";

    WIN32_FIND_DATA metaData;
    mylog << "Status of Get File Property is = " << getProperty(hContext, &metaData);

    cout << "\n\n ####################### FILE PROPERTIES ###################";
    cout << "\nFile Name = " << metaData.cFileName << "\n" << "File Size = " << (metaData.nFileSizeLow - metaData.nFileSizeHigh) << "bytes";
    
    cout << endl<< "\nLast access Time =" << metaData.ftLastAccessTime.dwLowDateTime - metaData.ftLastAccessTime.dwHighDateTime; ;

    cout << "\n##########################################################\n";

    cout << "\nEnter File path to delete from server";
    string filetoDelete;
    getline(cin, filetoDelete);
    mylog << "Calling Delete ";
    mylog << "Status of DeleteFile is = " << DeleteFileOnPath(hContext, filetoDelete.c_str());
    cout << "\nDelete function called";
    std::cout << "\nPress enter to  Close Context & Exit from Client" << std::endl;
    std::cin.get();
    // Close the context handle.
    Close(&hContext);
    std::clog << "Context Closed. Exiting.." << std::endl;
    mylog << "Connection Closed, Exit";
    mylog << "\n\n\n\n";
}

int main()
{
   RPC_STATUS status;
   unsigned char* szStringBinding = nullptr;
   
   // This function is nothing more than a printf.
   // Connection is not done here.
   status = RpcStringBindingCompose(
      nullptr, // UUID to bind to.
      reinterpret_cast<unsigned char*>("ncacn_ip_tcp"), // Use TCP/IP protocol.
      reinterpret_cast<unsigned char*>("localhost"), // TCP/IP network address to use. //used for testing 146.122.195.213 
      reinterpret_cast<unsigned char*>("4747"), // TCP/IP port to use.
      nullptr, // Protocol dependent network options to use.
      &szStringBinding); // String binding output.

   if (status)
      return HandleError("RpcStringBindingCompose", status);

   handle_t hBinding = nullptr;

   //std::clog << "Calling RpcBindingFromStringBinding" << std::endl;

   // Validates the format of the string binding handle and converts it to a binding handle.
   status = RpcBindingFromStringBinding(szStringBinding, // The string binding to validate.
      &hBinding); // Put the result in the explicit binding handle.
   
   if (status)
      return HandleError("RpcBindingFromStringBinding", status);

   // Free the memory allocated by a string.
   status = RpcStringFree(
      &szStringBinding); // String to be freed.
   
   if (status)
      return HandleError("RpcStringFree", status);

   status = RpcEpResolveBinding(hBinding, ContextExample_v1_0_c_ifspec);
   
   if (status)
      return HandleError("RpcEpResolveBinding", status);
   
   char pathToDelete[256];
   RpcTryExcept
   {
       
    
     cout << endl << "Connection Established" << endl;
     callmenu(hBinding);
      
   }
   RpcExcept(1)
   {
      HandleError("Remote Procedure Call", RpcExceptionCode());
   }
   RpcEndExcept

   // Releases binding handle resources and disconnects from the server.
   status = RpcBindingFree(
      &hBinding); // Frees the explicit binding handle.
   if (status)
      return HandleError("RpcBindingFree", status);
    
}

// Memory allocation function for RPC.
// The runtime uses these two functions for allocating/deallocating
// enough memory to pass the string to the server.
void* __RPC_USER midl_user_allocate(size_t size)
{
   return malloc(size);
}

// Memory deallocation function for RPC.
void __RPC_USER midl_user_free(void* p)
{
   free(p);
}
