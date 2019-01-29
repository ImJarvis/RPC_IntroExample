

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Tue Jan 29 19:45:05 2019
 */
/* Compiler settings for ContextExample.idl:
    Oicf, W4, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __ContextExample_h__
#define __ContextExample_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ContextExample_INTERFACE_DEFINED__
#define __ContextExample_INTERFACE_DEFINED__

/* interface ContextExample */
/* [explicit_handle][version][uuid] */ 

#include "minwinbase.h" 
typedef /* [context_handle] */ void *CONTEXT_HANDLE;

#ifdef __midl
typedef /* [helpstring] */ struct _FILETIME
    {
    long dwLowDateTime;
    long dwHighDateTime;
    } 	FILETIME;

typedef /* [helpstring] */ unsigned long DWORD;

typedef /* [public][public][helpstring] */ struct __MIDL_ContextExample_0001
    {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    unsigned char cFileName[ 256 ];
    unsigned char cAlternateFileName[ 256 ];
    } 	WIN32_FIND_DATA;

#endif
CONTEXT_HANDLE Open( 
    /* [in] */ handle_t hBinding,
    /* [string][in] */ const char *szString);

void Output( 
    /* [in] */ CONTEXT_HANDLE hContext);

int CreateNewFile( 
    /* [in] */ CONTEXT_HANDLE hContext,
    /* [defaultvalue][string][optional][in] */ const char *filePath,
    /* [defaultvalue][string][optional][in] */ const char *Text);

int CopyPaste( 
    /* [in] */ CONTEXT_HANDLE hContext,
    /* [string][in] */ const char *sourcePath,
    /* [string][in] */ const char *destinationPath);

int DeleteFileOnPath( 
    /* [in] */ CONTEXT_HANDLE hContext,
    /* [string][in] */ const char *Path);

int getFileInformation( 
    /* [in] */ CONTEXT_HANDLE hContext,
    /* [string][in] */ const char *Path,
    /* [out][in] */ WIN32_FIND_DATA *metaData);

void Close( 
    /* [out][in] */ CONTEXT_HANDLE *phContext);



extern RPC_IF_HANDLE ContextExample_v1_0_c_ifspec;
extern RPC_IF_HANDLE ContextExample_v1_0_s_ifspec;
#endif /* __ContextExample_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

void __RPC_USER CONTEXT_HANDLE_rundown( CONTEXT_HANDLE );

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


