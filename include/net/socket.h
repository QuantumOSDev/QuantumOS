#ifndef SOCKET_H
#define SOCKET_H

typedef struct __socket_t {
    int domain;
    int type;
    int protocol;

    long ref;
    void* p;
} socket_t;

#include <fs/vfs.h>

/* Took from sys/socket.h linux header */
#define AF_UNSPEC	    0	/* Unspecified.  */
#define AF_LOCAL	    1	/* Local to host (pipes and file-domain).  */
#define AF_UNIX		    AF_LOCAL /* POSIX name for PF_LOCAL.  */
#define AF_FILE		    AF_LOCAL /* Another non-standard name for PF_LOCAL.  */
#define AF_INET		    2	/* IP protocol family.  */
#define AF_AX25		    3	/* Amateur Radio AX.25.  */
#define AF_IPX		    4	/* Novell Internet Protocol.  */
#define AF_APPLETALK	5	/* Appletalk DDP.  */
#define AF_NETROM	    6	/* Amateur radio NetROM.  */
#define AF_BRIDGE	    7	/* Multiprotocol bridge.  */
#define AF_ATMPVC	    8	/* ATM PVCs.  */
#define AF_X25		    9	/* Reserved for X.25 project.  */
#define AF_INET6	    10	/* IP version 6.  */
#define AF_ROSE		    11	/* Amateur Radio X.25 PLP.  */
#define AF_DECnet	    12	/* Reserved for DECnet project.  */
#define AF_NETBEUI	    13	/* Reserved for 802.2LLC project.  */
#define AF_SECURITY	    14	/* Security callback pseudo AF.  */
#define AF_KEY		    15	/* PF_KEY key management API.  */
#define AF_NETLINK	    16
#define AF_ROUTE	    AF_NETLINK /* Alias to emulate 4.4BSD.  */
#define AF_PACKET	    17	/* Packet family.  */
#define AF_ASH		    18	/* Ash.  */
#define AF_ECONET	    19	/* Acorn Econet.  */
#define AF_ATMSVC	    20	/* ATM SVCs.  */
#define AF_RDS		    21	/* RDS sockets.  */
#define AF_SNA		    22	/* Linux SNA Project */
#define AF_IRDA		    23	/* IRDA sockets.  */
#define AF_PPPOX	    24	/* PPPoX sockets.  */
#define AF_WANPIPE	    25	/* Wanpipe API sockets.  */
#define AF_LLC		    26	/* Linux LLC.  */
#define AF_IB		    27	/* Native InfiniBand address.  */
#define AF_MPLS		    28	/* MPLS.  */
#define AF_CAN		    29	/* Controller Area Network.  */
#define AF_TIPC		    30	/* TIPC sockets.  */
#define AF_BLUETOOTH	31	/* Bluetooth sockets.  */
#define AF_IUCV		    32	/* IUCV sockets.  */
#define AF_RXRPC	    33	/* RxRPC sockets.  */
#define AF_ISDN		    34	/* mISDN sockets.  */
#define AF_PHONET	    35	/* Phonet sockets.  */
#define AF_IEEE802154	36	/* IEEE 802.15.4 sockets.  */
#define AF_CAIF		    37	/* CAIF sockets.  */
#define AF_ALG		    38	/* Algorithm sockets.  */
#define AF_NFC		    39	/* NFC sockets.  */
#define AF_VSOCK	    40	/* vSockets.  */
#define AF_KCM		    41	/* Kernel Connection Multiplexor.  */
#define AF_QIPCRTR	    42	/* Qualcomm IPC Router.  */
#define AF_SMC		    43	/* SMC sockets.  */
#define AF_XDP		    44	/* XDP sockets.  */
#define AF_MAX		    45	/* For now..  */

#define SOCK_STREAM     1
#define SOCK_DGRAM      2
#define SOCK_RAW        3
#define SOCK_RDM        4
#define SOCK_SEQPACKET  5
#define SOCK_DCCP       6
#define SOCK_PACKET     10
#define SOCK_CLOEXEC    02000000
#define SOCK_NONBLOCK   00004000

typedef unsigned int socklen_t;
typedef struct __sockaddr {
    unsigned short int sa_family;
    unsigned char      sa_data[14];
} sockaddr;

int socket_create(struct __vfs_node* file, int domain, int type, int protocol);

int socket_accept(struct __vfs_node* file, struct __vfs_node* conn, sockaddr* addr, socklen_t* len);

int socket_bind(struct __vfs_node* file, sockaddr* addr, unsigned int len);

int socket_connect(struct __vfs_node* file, sockaddr* addr, unsigned int len);

int socket_listen(struct __vfs_node* file, int backlog);

int socket_send(struct __vfs_node* file, void* buf, unsigned long len, int flags);

int socket_recv(struct __vfs_node* file, void* buf, unsigned long len, int flags);

int socket_can_read(struct __vfs_node* file, unsigned long len);

int socket_can_write(struct __vfs_node* file, unsigned long len);

int socket_shutdown(struct __vfs_node* file, int how);

#endif