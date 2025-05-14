#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/if_link.h>
#include <linux/bpf.h>
#include <bpf/libbpf.h>
#include <bpf/bpf.h>
#include <linux/bpf.h> // Include the header for bpf_set_link_xdp_fd
#include <bpf/libbpf.h> // Include the correct header for bpf_object__find_program_by_title
#include <bpf/bpf.h> // Include the header for bpf_set_link_xdp_fd

// Explicitly declare the bpf_set_link_xdp_fd function prototype
int bpf_set_link_xdp_fd(int ifindex, int fd, __u32 flags);
#include <bpf/bpf.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>

/**
 * attach_xdp_program - XDP programını belirtilen ağ arayüzüne bağlar.
 * @interface_name: Ağ arayüzü adı (örneğin, "eth0")
 * @bpf_prog_fd: Yüklenmiş BPF programının dosya tanıtıcısı
 * @xdp_flags: XDP bağlama bayrakları (örneğin, XDP_FLAGS_SKB_MODE)
 * 
 * Return: 0 başarıyla bağlanırsa, aksi halde hata kodu
 */
int attach_xdp_program(const char *interface_name, int bpf_prog_fd, __u32 xdp_flags) {
    int ifindex;

    // Ağ arayüzü indeksini al
    ifindex = if_nametoindex(interface_name);
    if (ifindex == 0) {
        perror("Ağ arayüzü indeksini alırken hata");
        return -1;
    }

    // XDP programını bağla
    if (bpf_set_link_xdp_fd(ifindex, bpf_prog_fd, xdp_flags) < 0) {
        perror("XDP programını bağlama başarısız");
        return -2;
    }

    printf("XDP programı '%s' arayüzüne başarıyla bağlandı.\n", interface_name);
    return 0;
}

int main(int argc, char **argv) {
    const char *interface_name = "lo"; // Bağlanılacak ağ arayüzü
    const char *bpf_prog_path = "ips_kern.o"; // BPF programının derlenmiş dosyası
    int bpf_prog_fd = -1; // Yüklenmiş BPF programının dosya tanıtıcısı
    __u32 xdp_flags = XDP_FLAGS_SKB_MODE; // XDP_GENERIC modunda bağlanmak için bayrak
    struct bpf_object *obj;

    if (argc > 1) {
        interface_name = argv[1]; // Komut satırından arayüz adı al
    }

    // BPF programını yükle
    obj = bpf_object__open_file(bpf_prog_path, NULL);
    if (!obj) {
        fprintf(stderr, "Hata: BPF programı '%s' açılamadı.\n", bpf_prog_path);
        return -1;
    }

    if (bpf_object__load(obj)) {
        fprintf(stderr, "Hata: BPF programı yüklenemedi.\n");
        return -1;
    }

    // BPF programının dosya tanıtıcısını al
    struct bpf_program *prog = bpf_object__find_program_by_name(obj, "ips_kern");
    if (!prog) {
        fprintf(stderr, "Hata: BPF programı 'ips_kern' bulunamadı.\n");
        return -1;
    }
    bpf_prog_fd = bpf_program__fd(prog);
    if (bpf_prog_fd < 0) {
        fprintf(stderr, "Hata: BPF programı dosya tanıtıcısı alınamadı.\n");
        return -1;
    }

    // XDP programını bağla
    int result = attach_xdp_program(interface_name, bpf_prog_fd, xdp_flags);
    if (result == 0) {
        printf("Bağlantı başarılı.\n");
    } else {
        printf("Bağlantı başarısız. Hata kodu: %d\n", result);
    }

    return 0;
}