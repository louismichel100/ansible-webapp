/* SPDX-License-Identifier: LGPL-2.1-or-later */
#pragma once

#include <stdbool.h>
#include "macro.h"

#if defined(__x86_64__) || defined(__i386__) || defined(__arm__) || defined(__aarch64__)
#define ARCHITECTURE_SUPPORTS_SMBIOS 1
#else
#define ARCHITECTURE_SUPPORTS_SMBIOS 0
#endif

#if defined(__x86_64__) || defined(__arm__) || defined(__aarch64__)
#define ARCHITECTURE_SUPPORTS_TPM 1
#else
#define ARCHITECTURE_SUPPORTS_TPM 0
#endif

#if defined(__arm__) || defined(__aarch64__)
#define DEFAULT_SERIAL_TTY "ttyAMA0"
#elif defined(__s390__) || defined(__s390x__)
#define DEFAULT_SERIAL_TTY "ttysclp0"
#elif defined(__powerpc__) || defined(__powerpc64__)
#define DEFAULT_SERIAL_TTY "hvc0"
#else
#define DEFAULT_SERIAL_TTY "ttyS0"
#endif

typedef struct OvmfConfig {
        char *path;
        char *format;
        char *vars;
        char *vars_format;
        bool supports_sb;
} OvmfConfig;

static inline const char *ovmf_config_format(const OvmfConfig *c) {
        return ASSERT_PTR(c)->format ?: "raw";
}

static inline const char *ovmf_config_vars_format(const OvmfConfig *c) {
        return ASSERT_PTR(c)->vars_format ?: "raw";
}

OvmfConfig* ovmf_config_free(OvmfConfig *ovmf_config);
DEFINE_TRIVIAL_CLEANUP_FUNC(OvmfConfig*, ovmf_config_free);

typedef enum QemuNetworkStack {
        QEMU_NET_TAP,
        QEMU_NET_USER,
        QEMU_NET_NONE,
        _QEMU_NET_MAX,
        _QEMU_NET_INVALID = -EINVAL,
} QemuNetworkStack;

static const char* const qemu_network_stack_table[_QEMU_NET_MAX] = {
        [QEMU_NET_TAP]  = "tap",
        [QEMU_NET_USER] = "user",
        [QEMU_NET_NONE] = "none",
};

const char* qemu_network_stack_to_string(QemuNetworkStack type) _const_;
QemuNetworkStack qemu_network_stack_from_string(const char *s) _pure_;

int qemu_check_kvm_support(void);
int qemu_check_vsock_support(void);
int list_ovmf_config(char ***ret);
int load_ovmf_config(const char *path, OvmfConfig **ret);
int find_ovmf_config(int search_sb, OvmfConfig **ret);
int find_qemu_binary(char **ret_qemu_binary);
int vsock_fix_child_cid(int vsock_fd, unsigned *machine_cid, const char *machine);
