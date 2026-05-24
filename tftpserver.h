//
// tftpserver.h - TFTP server for BMC64.
//
// Allows uploading disk images and other files to the SD card over the
// network using any standard TFTP client:
//   tftp -e put game.d64 <pi-ip>          (uploads to /disks/game.d64)
//   tftp -e put cart.crt <pi-ip>:carts/   (uploads to /carts/cart.crt)
//   tftp -e get game.d64 <pi-ip>          (downloads /disks/game.d64)
//
// Files are read/written relative to the SD card root ("/").
// Standard TFTP port 69.
//

#ifndef _tftpserver_h
#define _tftpserver_h

#include <circle/net/tftpdaemon.h>
#include <circle/net/netsubsystem.h>
#include <stdio.h>

class CTFTPServer : public CTFTPDaemon
{
public:
    CTFTPServer(CNetSubSystem *pNetSubSystem);
    ~CTFTPServer(void);

    boolean FileOpen(const char *pFileName) override;
    boolean FileCreate(const char *pFileName) override;
    boolean FileClose(void) override;
    int FileRead(void *pBuffer, unsigned nCount) override;
    int FileWrite(const void *pBuffer, unsigned nCount) override;

private:
    FILE *m_pFile;
};

#endif
