.c uzantılı dosyayı çalıştırmak için derlenmesi gerekmektedir. Bunun için gerekli olan yazılımlar şunlardır:

1. Clang + LLVM = eBPF bytecode üretmek için clang derleyicisi gerekir. (Yüklemek için "sudo apt install -y clang llvm libbpf-dev")
2. Kernel Headers = Linux çekirdek kaynakları ya da başlık dosyaları. (Linux tabanlı işletim sistemlerinde güncelleme yapmak için "sudo apt update && sudo apt install linux-headers-$(uname -r)")

Çalışma Ortamı Gereksinimleri

1.Linux 5.x veya üstü kernel 

2.Desteklenen Ağ Sürücüsü (XDP'nin Generic modu kullanıldığı için her ağ arayüzünde çalışmaktadır.)

3.Root Erişimi

Not : .c uzantılı dosyayı derlemek için gerekli derleme programını kurduktan sonra terminal üzerinden "xdp_prog.c" dosyasının bulunduğu konumda "clang -O2 -target bpf -c xdp_prog.c -o xdp_prog.o" komutunu yazmak gerekmektedir.



