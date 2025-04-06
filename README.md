# XDP Programı Derleme ve Çalıştırma Rehberi

## Gerekli Yazılımlar

`.c` uzantılı dosyayı çalıştırmak için önce derlenmesi gerekmektedir. Bunun için aşağıdaki yazılımlar gereklidir:

1. **Clang + LLVM**  
   eBPF bytecode üretmek için `clang` derleyicisi gereklidir.  
   **Kurulum komutu:**
   ```bash
   sudo apt install -y clang llvm libbpf-dev
   ```

2. **Kernel Headers**  
   Linux çekirdek kaynakları veya başlık dosyaları gereklidir.  
   **Güncelleme ve kurulum komutu:**
   ```bash
   sudo apt update && sudo apt install linux-headers-$(uname -r)
   ```

---

## Çalışma Ortamı Gereksinimleri

- ✅ **Linux 5.x veya üstü kernel**
- ✅ **Desteklenen Ağ Sürücüsü**  
  (XDP'nin Generic modu kullanıldığı için her ağ arayüzünde çalışmaktadır.)
- ✅ **Root Erişimi**

---

## Derleme İşlemi

`.c` uzantılı dosyayı derlemek için aşağıdaki adımları izleyin:

1. Terminalde, `xdp_prog.c` dosyasının bulunduğu dizine gidin.
2. Aşağıdaki komutu çalıştırarak derlemeyi gerçekleştirin:
   ```bash
   clang -O2 -target bpf -c xdp_prog.c -o xdp_prog.o
   ```

✅ Derleme başarılı olursa `xdp_prog.o` adında bir dosya oluşacaktır.

---

## Ağ Arayüzüne Entegre Etme

Derlenen `.o` uzantılı dosyayı ilgili ağ arayüzüne entegre etmek için:

```bash
sudo ip link set dev <arayuz> xdp obj xdp_prog.o sec xdp
```

**Not:** `<arayuz>` kısmına (örneğin `eth0`, `ens33` vb.) kendi ağ arayüzünüzün adını yazın.

---

## Entegrasyon Kontrolü

XDP programının başarılı bir şekilde entegre edilip edilmediğini kontrol etmek için:

```bash
ip a | grep xdp
```

Bu komut, arayüzünüzde XDP’nin aktif olup olmadığını gösterir.

---

Her şey doğru şekilde ayarlandıysa, XDP programınız artık çalışmaya hazır!

