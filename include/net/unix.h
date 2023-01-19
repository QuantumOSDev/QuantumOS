#ifndef SOCKET_UNIX_H
#define SOCKET_UNIX_H

#include <fs/vfs.h>

int unix_socket_create(struct __vfs_node* file, int domain, int type, int protocol);

int unix_socket_accept(struct __vfs_node* file, struct __vfs_node* conn, sockaddr* addr, socklen_t* len);

int unix_socket_bind(struct __vfs_node* file, sockaddr* addr, unsigned int len);

int unix_socket_connect(struct __vfs_node* file, sockaddr* addr, unsigned int len);

int unix_socket_listen(struct __vfs_node* file, int backlog);

int unix_socket_send(struct __vfs_node* file, void* buf, unsigned long len, int flags);

int unix_socket_recv(struct __vfs_node* file, void* buf, unsigned long len, int flags);

int unix_socket_can_read(struct __vfs_node* file, unsigned long len);

int unix_socket_can_write(struct __vfs_node* file, unsigned long len);

int unix_socket_shutdown(struct __vfs_node* file, int how);

#endif