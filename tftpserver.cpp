//
// tftpserver.cpp - TFTP server for BMC64.
//

#include "tftpserver.h"
#include <string.h>
#include <stdio.h>

// Resolve path: strip leading "/" if present, prepend "./" for Circle's FAT fs
static void resolve_path(const char *pFileName, char *pOut, unsigned nOutSize) {
    const char *p = pFileName;
    // Skip leading slashes
    while (*p == '/') p++;
    // Prepend "./"
    snprintf(pOut, nOutSize, "./%s", p);
}

CTFTPServer::CTFTPServer(CNetSubSystem *pNetSubSystem)
    : CTFTPDaemon(pNetSubSystem), m_pFile(nullptr) {
}

CTFTPServer::~CTFTPServer(void) {
    if (m_pFile) {
        fclose(m_pFile);
        m_pFile = nullptr;
    }
}

boolean CTFTPServer::FileOpen(const char *pFileName) {
    char path[256];
    resolve_path(pFileName, path, sizeof(path));
    m_pFile = fopen(path, "rb");
    return m_pFile != nullptr ? TRUE : FALSE;
}

boolean CTFTPServer::FileCreate(const char *pFileName) {
    char path[256];
    resolve_path(pFileName, path, sizeof(path));
    m_pFile = fopen(path, "wb");
    return m_pFile != nullptr ? TRUE : FALSE;
}

boolean CTFTPServer::FileClose(void) {
    if (m_pFile) {
        fclose(m_pFile);
        m_pFile = nullptr;
    }
    return TRUE;
}

int CTFTPServer::FileRead(void *pBuffer, unsigned nCount) {
    if (!m_pFile) return -1;
    return (int)fread(pBuffer, 1, nCount, m_pFile);
}

int CTFTPServer::FileWrite(const void *pBuffer, unsigned nCount) {
    if (!m_pFile) return -1;
    return (int)fwrite(pBuffer, 1, nCount, m_pFile);
}
