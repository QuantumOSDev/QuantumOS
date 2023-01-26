/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <net/socket.h>

#include <quantum/init.h>
#include <core/errno.h>

#include <fs/vfs.h>

int socket_create(__vfs_node_t* file, int domain, int type, int protocol)
{
    if (domain == AF_UNIX)
    {

    }

    return -EAFNOSUPPORT;
}

int socket_accept(__vfs_node_t* file, __vfs_node_t* conn, sockaddr* addr, socklen_t* len);

int socket_bind(__vfs_node_t* file, sockaddr* addr, unsigned int len);

int socket_connect(__vfs_node_t* file, sockaddr* addr, unsigned int len);

int socket_listen(__vfs_node_t* file, int backlog);

int socket_send(__vfs_node_t* file, void* buf, unsigned long len, int flags);

int socket_recv(__vfs_node_t* file, void* buf, unsigned long len, int flags);

int socket_can_read(__vfs_node_t* file, unsigned long len);

int socket_can_write(__vfs_node_t* file, unsigned long len);

int socket_shutdown(__vfs_node_t* file, int how);