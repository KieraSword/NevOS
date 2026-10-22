typedef unsigned int size_t;
typedef long int time_t;
typedef int fd_t;
typedef int pid_t;
#define NULL ((void *)0)
#define VIDEO ((char *)0xB8000)
#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8
#define EPERM 1
#define ENOENT 2
#define ESRCH 3
#define EINTR 4
#define EIO 5
#define ENXIO 6
#define E2BIG 7
#define ENOEXEC 8
#define EBADF 9
#define ECHILD 10
#define EAGAIN 11
#define ENOMEM 12
#define EACCES 13
#define EFAULT 14
#define ENOTBLK 15
#define EBUSY 16
#define EEXIST 17
#define EXDEV 18
#define ENODEV 19
#define ENOTDIR 20
#define EISDIR 21
#define EINVAL 22
#define ENFILE 23
#define EMFILE 24
#define ENOTTY 25
#define ETXTBSY 26
#define EFBIG 27
#define ENOSPC 28
#define ESPIPE 29
#define EROFS 30
#define EMLINK 31
#define EPIPE 32
#define EDOM 33
#define ERANGE 34
#define EDEADLK 35
#define ENAMETOOLONG 36
#define ENOLCK 37
#define ENOSYS 38
#define MAX_FILES 16
#define BCD2BIN(v) ((v & 0x0F) + ((v >> 4) * 10))
#define JUMP(a) ((void (*)(void))(a))()
int con_write(char *msg, size_t len);
void con_putch(char chr);
void con_puts(char *msg);
void con_puti(int val);
char con_getch();
void con_gets(char *buf);
int con_read(char *msg, size_t len);
void fat_mount(void);
int fat_open(char *name);
void fat_close(int fd);
int fat_write(int fd, void *buf, int count);
int fat_read(int fd, void *buf, int count);
int fat_create(char *name);
int fat_delete(char *name);
char io_inb(short port);
void io_outb(short port, char val);
int sys_syscall(int a, int b, int c, int d);
void idt_init();
void time_rtctime();
time_t time_time(time_t *tloc);
void user_enter(void *entry, void *stack);
size_t strlen(char *str);
int strcmp(char *a, char *b);
int atoi(char *str);
void itoa(int num, char *buf);
struct idt_entry {
    unsigned short off_lo, sel;
    unsigned char zero, flags;
    unsigned short off_hi;
} __attribute__((packed));
struct idt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));
struct idt_entry hw_idt[256];
struct idt_ptr idtp;
struct fat_fs {
    unsigned int bytes_per_sector;
    unsigned int sectors_per_cluster;
    unsigned int reserved_sectors;
    unsigned int num_fats;
    unsigned int sectors_per_fat;
    unsigned int first_data_sector;
    unsigned int root_cluster;
    unsigned int total_clusters;
};
struct fat_boot_sector {
    unsigned char jump[3];
    unsigned char oem[8];
    unsigned short bytes_per_sector;
    unsigned char sectors_per_cluster;
    unsigned short reserved_sectors;
    unsigned char num_fats;
    unsigned short root_entries;
    unsigned short total_sectors_16;
    unsigned char media;
    unsigned short sectors_per_fat_16;
    unsigned short sectors_per_track;
    unsigned short num_heads;
    unsigned int hidden_sectors;
    unsigned int total_sectors_32;
    unsigned int sectors_per_fat_32;
    unsigned short ext_flags;
    unsigned short fs_version;
    unsigned int root_cluster;
    unsigned short fs_info;
    unsigned short backup_boot;
    unsigned char reserved[12];
    unsigned char drive_num;
    unsigned char reserved1;
    unsigned char boot_sig;
    unsigned int volume_id;
    unsigned char volume_label[11];
    unsigned char fs_type[8];
} __attribute__((packed));
struct fs_file {
    int used;
    unsigned int first_cluster;
    unsigned int size;
    unsigned int pos;
	unsigned int dir_sector;
	unsigned int dir_offset;
};
struct time_rtc {
	char sec, min, hour, day, mon, year;
};
struct time_tm {
	int tm_sec, tm_min, tm_hour, tm_mday, tm_mon, tm_year, m_yday;
};
struct user_task {
    int *entry;
    int *stack;
    unsigned char *data[];
    int size;
    int active;
};
struct user_tss_entry {
    unsigned int prev_tss;
    unsigned int esp0;
    unsigned int ss0;
    unsigned int esp1;
    unsigned int ss1;
    unsigned int esp2;
    unsigned int ss2;
    unsigned int cr3;
    unsigned int eip;
    unsigned int eflags;
    unsigned int eax, ecx, edx, ebx;
    unsigned int esp, ebp, esi, edi;
    unsigned int es, cs, ss, ds, fs, gs;
    unsigned int ldt;
    unsigned short trap;
    unsigned short iomap_base;
} __attribute__((packed));
typedef int (*syscall)();
struct fat_fs fs;
struct fs_file fs_files[MAX_FILES];
struct time_rtc time_current;
struct user_task user_tasks[2048];
struct user_tss_entry user_tss;
int con_pos = 0;
unsigned char fs_buf[512];
int time_ticks = 0;
unsigned char kstack[4096] __attribute__((aligned(16)));
unsigned char ustack[4096] __attribute__((aligned(16)));
unsigned char *gdt_start = 0x7C02;
__attribute__((section(".text.entry")))
void entry() {
	idt_init();
	fat_mount();
	time_rtctime();
	/*con_write("Hello!", 6);
	con_putch('\n');
	con_write("Hello.", 6);
	con_putch('\n');
	con_puts("Demo.");
	con_putch('\n');
	con_puts("1\t2\t3");
	con_putch('\n');
	//asm("ud2");
	//con_puts("== NeverOS ==");
	fat_create("hello.txt");
	fd_t fd = fat_open("hello.txt");
	fat_write(fd, "Hello\n", 6);
	fd = fat_open("hello.txt");
	char fbuf[12];
	fat_read(fd, fbuf, 6);
	con_puts(fbuf);
	fat_close(fd);
	time_t now = time_time(NULL);
	con_puts("Epoch: ");
	con_puti(now);
	con_putch('\n');
	sys_syscall(5, 1, "Hello world\n", 12);*/
	/*char buf[256];
	int i = 0;
	char c;
	while((c = con_getch()) != '\n') {
		con_putch(c);
		buf[i++] = c;
	}
	con_putch('\n');/
	if(strcmp(buf, "calc") == 0) {
		while(1) {
			int a = 0;
			int b = 0;
			char op = '\0';
			con_puts("First number: ");
			char buf[256];
			int i = 0;
			char c;
			while((c = con_getch()) != '\n') {
				con_putch(c);
				buf[i++] = c;
				buf[i] = '\0'; 
			}
			con_putch('\n');
			a = atoi(buf);
			con_puts("Operator: ");
			op = con_getch();
			con_putch(op);
			con_getch();
			con_putch('\n');
			con_puts("Second number: ");
			i = 0;
			while((c = con_getch()) != '\n') {
				con_putch(c);
				buf[i++] = c;
				buf[i] = '\0'; 
			}
			con_putch('\n');
			b = atoi(buf);
			switch(op) {
				case('+'): {
					char buf[256];
					itoa(a + b, buf);
					con_puts(buf);
					break;
				}
				case('-'): {
					char buf[256];
					itoa(a - b, buf);
					con_puts(buf);
					break;
				}
				case('*'): {
					char buf[256];
					itoa(a * b, buf);
					con_puts(buf);
					break;
				}
				case('/'): {
					char buf[256];
					itoa(a / b, buf);
					con_puts(buf);
					break;
				}
			}
			con_putch('\n');
		}
	}*/
	fd_t progfile = fat_open("program.nxe");
	char progbuf[256];
	//con_puti(&progbuf);
	fat_read(progfile, progbuf, 256);
	fat_close(progfile);
	user_tss_init();
	user_enter(progbuf, ustack);
	//JUMP(progbuf);
	asm("hlt");
}
void panic(char *msg) {
	int eax_val;
	char eax_str[12];
	asm("" : "=a"(eax_val));
	int ebx_val;
	char ebx_str[12];
	asm("" : "=b"(ebx_val));
	int ecx_val;
	char ecx_str[12];
	asm("" : "=c"(ecx_val));
	int edx_val;
	char edx_str[12];
	asm("" : "=d"(edx_val));
	itoa(eax_val, eax_str);
	itoa(ebx_val, ebx_str);
	itoa(ecx_val, ecx_str);
	itoa(edx_val, edx_str);
	con_puts("== ");
	con_puts(msg);
	con_puts(" ==");
	con_putch('\n');
	con_puts("EAX: ");
	con_puts(eax_str);
	con_putch('\n');
	con_puts("EBX: ");
	con_puts(ebx_str);
	con_putch('\n');
	con_puts("ECX: ");
	con_puts(ecx_str);
	con_putch('\n');
	con_puts("EDX: ");
	con_puts(edx_str);
	con_putch('\n');
	#if defined(PANIC_RESET)
		asm("jmp $0xFFFFFFF0");
	#elif defined(PANIC_POWEROFF)
		io_outw(0x604, 0x2000);
	#endif
	asm("hlt");
}
size_t strlen(char *str) {
	int size;
	for(size = 0; str[size] != '\0'; size++);
	return size;
}
int strcmp(char *a, char *b) {
    while(*a && *a == *b) {
		a++; 
		b++;
	}
    return (unsigned char)*a - (unsigned char)*b;
}
int atoi(char *str) {
    int n = 0;
    while (*str >= '0' && *str <= '9') {
        n = n * 10 + (*str - '0');
        str++;
    }
    return n;
}
void itoa(int num, char *buf) {
    char tmp[12];
    int i = 0;
    if (num == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    while (num > 0) {
        tmp[i++] = '0' + (num % 10);
        num /= 10;
    }
    int j = 0;
    while (i > 0) {
        buf[j++] = tmp[--i];
    }
    buf[j] = '\0';
}
int sys_restart() {
	return -ENOSYS;
}
int sys_exit(int code) {
	char buf[12];
	itoa(code, buf);
	panic(buf);
	return -ENOSYS;
}
int sys_fork() {
	return -ENOSYS;
}
int sys_spawn() {
	return -ENOSYS;
}
int sys_read(fd_t fd, char *buf, int len) {
	if(fd > 2) {
		return fat_read(fd, buf, len);
	} else if(fd == 0) {
		return con_read(buf, len);
	} else return -EBADF;
}
int sys_write(fd_t fd, char *buf, int len) {
	if(len < 0) return -EINVAL;
	if(fd > 2) {
		return fat_write(fd, buf, len);
	} else if(fd == 1 || fd == 2) {
		return con_write(buf, len);
	} else return -EBADF;
}
fd_t sys_create(char *name) {
	return fat_create(name);
}
fd_t sys_open(char *name) {
	return fat_open(name);
}
int sys_close(fd_t fd) {
	if(fd > 2) {
		fat_close(fd);
		return 0;
	} else return -EBADF;
}
int sys_remove(char *name) {
	return fat_delete(name);
}
int sys_time(time_t *tloc) {
	return time_time(tloc);
}
__attribute__((naked)) void stub0() {
	asm("pusha\n" "call int_de\n" "popa\n" "iret\n");	
}
__attribute__((naked)) void stub6() {
	asm("pusha\n" "call int_ud\n" "popa\n" "iret\n");	
}
__attribute__((naked)) void stub13() {
	asm("pusha\n" "call int_gp\n" "popa\n" "iret\n");	
}
__attribute__((naked)) void stub32() {
	asm("pusha\n" "call irq_tick\n" "popa\n" "iret\n");	
}
__attribute__((naked)) void stub66() {
	asm("pusha\n" "pushl %edx\n" "pushl %ecx\n" "pushl %ebx\n" "pushl %eax\n" "call int_sys\n" "addl $16, %esp\n" "movl %eax, 28(%esp)\n" "popa\n" "iret\n");
}
__attribute__((naked)) void stub118() {
	asm("pusha\n" "call mystery\n" "popa\n" "iret\n");	
}
void int_ud() {
	panic("Fatal exception: Undefined opcode (#UD)");
}
void int_de() {
	panic("Fatal exception: Division error (#DE)");
}
void int_gp() {
	panic("Fatal exception: Protection fault (#GP)");
}
void mystery() {
	//con_puts("Mystery fault\n");
}
int int_sys(int a, int b, int c, int d) {
	static syscall syscalls[256] = {sys_restart, sys_exit, sys_fork, sys_spawn, sys_read, sys_write, sys_open, sys_close, sys_create, sys_remove, sys_time};
	if(a < 0 || a >= 256) return -1;
	return syscalls[a](b, c, d);
}
void irq_tick() {
	time_ticks++;
	io_outb(0x20, 0x20);
}
void idt_load(struct idt_ptr *p) {
    asm("lidt (%0)" : : "r"(p));
}
int sys_syscall(int a, int b, int c, int d) {
	asm("int $0x42" : "=a"(a) : "a"(a), "b"(b), "c"(c), "d"(d));
	return a;
}
void idt_set(int n, unsigned int addr) {
    hw_idt[n].off_lo = addr & 0xFFFF;
    hw_idt[n].sel    = 0x08;
    hw_idt[n].zero   = 0;
    hw_idt[n].flags  = 0x8E;
    hw_idt[n].off_hi = (addr >> 16) & 0xFFFF;
}
void idt_init() {
    idtp.limit = sizeof(hw_idt) - 1;
    idtp.base = (unsigned int)&hw_idt;
    idt_set(0x00, (unsigned int)stub0);
    idt_set(0x06, (unsigned int)stub6);
    idt_set(0x0D, (unsigned int)stub13);
	idt_set(0x20, (unsigned int)stub32);
    idt_set(0x42, (unsigned int)stub66);
	hw_idt[0x42].flags = 0xEE;
    idt_set(0x76, (unsigned int)stub118);
    idt_load(&idtp);
}
char io_inb(short port) {
	char val;
	asm("inb %%dx, %%al" : "=a"(val) : "d"(port));
	return val;
}
void io_outb(short port, char val) {
	asm("outb %%al, %%dx" : : "a"(val), "d"(port));
}
short io_inw(short port) {
	short val;
	asm("inw %%dx, %%ax" : "=a"(val) : "d"(port));
	return val;
}
void io_outw(short port, short val) {
	asm("outw %%ax, %%dx" : : "a"(val), "d"(port));
}
void con_putch(char chr) {
    while (con_pos >= 80 * 25) {
        for (int i = 0; i < 24 * 80; i++) {
            VIDEO[i * 2] = VIDEO[(i + 80) * 2];
            VIDEO[i * 2 + 1] = VIDEO[(i + 80) * 2 + 1];
        }
        for (int i = 24 * 80; i < 80 * 25; i++) {
            VIDEO[i * 2] = ' ';
            VIDEO[i * 2 + 1] = 0x07;
        }
        con_pos -= 80;
    }
	switch(chr) {
		case('\n'): {
			con_pos += 80 - (con_pos % 80);
			io_outb(0x3D4, 0x0F);
			io_outb(0x3D5, (con_pos & 0xFF));
			io_outb(0x3D4, 0x0E);
			io_outb(0x3D5, (con_pos >> 8) & 0xFF);
			io_outb(0x3F8, '\r');
			io_outb(0x3F8, '\n');
			break;
		}
		case('\t'): {
			con_pos = (con_pos + 8) & ~7;
			io_outb(0x3D4, 0x0F);
			io_outb(0x3D5, (con_pos & 0xFF));
			io_outb(0x3D4, 0x0E);
			io_outb(0x3D5, (con_pos >> 8) & 0xFF);
			break;
		}
		case('\b'): {
			if(con_pos != 0) {
				con_pos--;
				VIDEO[con_pos * 2] = '\0';
				VIDEO[con_pos * 2 + 1] = 7;
				io_outb(0x3D4, 0x0F);
				io_outb(0x3D5, (con_pos & 0xFF));
				io_outb(0x3D4, 0x0E);
				io_outb(0x3D5, (con_pos >> 8) & 0xFF);
			}
			break;
		}
		default: {
			VIDEO[con_pos * 2] = chr;
			VIDEO[con_pos * 2 + 1] = 7;
			io_outb(0x3D4, 0x0F);
			io_outb(0x3D5, ((con_pos + 1) & 0xFF));
			io_outb(0x3D4, 0x0E);
			io_outb(0x3D5, ((con_pos + 1) >> 8) & 0xFF);
			con_pos++;
			io_outb(0x3F8, chr);
		}
	}
}
void con_puts(char *msg) {
	for(int i = 0; msg[i] != '\0'; i++) con_putch(msg[i]);
}
void con_puti(int val) {
	char buf[12];
	itoa(val, buf);
	con_puts(buf);
}
int con_write(char *msg, size_t len) {
	int i = 0;
	for(i = 0; i < len; i++) con_putch(msg[i]);
	return i;
}
char con_getch() {
	static const char kbd[128] = {0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',};
	static const char kbds[128] = {0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' '};
	static int shift = 0;
	while(1) {
		if(io_inb(0x64) & 1) {
			char sc = io_inb(0x60);
			if(sc & 0x80) {
				char rsc = sc & 0x7F;
				if(rsc == 0x2A || rsc == 0x36) shift = 0;
				continue;
			}
			if(sc == 0x2A || sc == 0x36) {
				shift = 1;
				continue;
			}
			char chr = shift ? kbds[sc & 0x7F] : kbd[sc & 0x7F];
			return chr;
		}
	}
}
void con_gets(char *buf) {
	int i = 0;
	char c;
	while((c = con_getch()) != '\n') {
		con_putch(c);
		buf[i++] = c;
	}
	buf[i] = '\0';
}
int con_read(char *buf, size_t len) {
	int i = 0;
	char c;
	while((c = con_getch()) && i < len) {
		con_putch(c);
		buf[i++] = c;
	}
	buf[i] = '\0';
	return i;
}
void fat_read_sector(int lba, char *buf) {
    while (io_inb(0x1F7) & 0x80);

    io_outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));
    io_outb(0x1F2, 1);
    io_outb(0x1F3, lba & 0xFF);
    io_outb(0x1F4, (lba >> 8) & 0xFF);
    io_outb(0x1F5, (lba >> 16) & 0xFF);
    io_outb(0x1F7, 0x20);

    while (!(io_inb(0x1F7) & 0x08));

    for (int i = 0; i < 256; i++) {
        unsigned short w;
        asm("inw %%dx, %%ax" : "=a"(w) : "d"(0x1F0));
        *(unsigned short *)(buf + i * 2) = w;
    }
}
void fat_write_sector(int lba, char *buf) {
    while (io_inb(0x1F7) & 0x80);

    io_outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));
    io_outb(0x1F2, 1);
    io_outb(0x1F3, lba & 0xFF);
    io_outb(0x1F4, (lba >> 8) & 0xFF);
    io_outb(0x1F5, (lba >> 16) & 0xFF);
    io_outb(0x1F7, 0x30);

    while (!(io_inb(0x1F7) & 0x08));

    for (int i = 0; i < 256; i++) {
        unsigned short w = *(unsigned short *)(buf + i * 2);
		asm("outw %%ax, %%dx" : : "a"(w), "d"(0x1F0));
    }

    io_outb(0x1F7, 0xE7);
    while (io_inb(0x1F7) & 0x80);
}
int fat_to_83(const char *name, unsigned char out[11]) {
    for (int i = 0; i < 11; i++) out[i] = ' ';

    int i = 0, o = 0;
    while (name[i] && name[i] != '.' && o < 8) {
        char c = name[i++];
        if (c >= 'a' && c <= 'z') c -= 32;
        out[o++] = c;
    }
    if (name[i] && name[i] != '.') return -1;
    if (name[i] == '.') i++;

    o = 8;
    while (name[i] && o < 11) {
        char c = name[i++];
        if (c >= 'a' && c <= 'z') c -= 32;
        out[o++] = c;
    }
    if (name[i]) return -1;

    return 0;
}
int fat_cluster_to_sector(int cluster) {
    return fs.first_data_sector + (cluster - 2) * fs.sectors_per_cluster;
}
void fat_mount(void) {
    unsigned char buf[512];
    fat_read_sector(0, buf);
    struct fat_boot_sector *bs = (struct fat_boot_sector*)buf;
    fs.bytes_per_sector = bs->bytes_per_sector;
    fs.sectors_per_cluster = bs->sectors_per_cluster;
    fs.reserved_sectors = bs->reserved_sectors;
    fs.num_fats = bs->num_fats;
    fs.sectors_per_fat = bs->sectors_per_fat_32;
    fs.root_cluster = bs->root_cluster;
    unsigned int total_sectors = bs->total_sectors_16 ? bs->total_sectors_16 : bs->total_sectors_32;
    fs.first_data_sector = fs.reserved_sectors + fs.num_fats * fs.sectors_per_fat;
    unsigned int data_sectors = total_sectors - fs.first_data_sector;
    fs.total_clusters = data_sectors / fs.sectors_per_cluster;
}
unsigned int fat_next(unsigned int cluster) {
    unsigned int off = cluster * 4;
    unsigned int sector = fs.reserved_sectors + off / fs.bytes_per_sector;
    unsigned int offset = off % fs.bytes_per_sector;
    fat_read_sector(sector, fs_buf);
    unsigned int v = *(unsigned int *)(fs_buf + offset) & 0x0FFFFFFF;
    if (v >= 0x0FFFFFF8) return 0xFFFFFFFF;
    if (v == 0x0FFFFFF7) return 0xFFFFFFFE;
    return v;
}
void fat_set(unsigned int cluster, unsigned int value) {
    unsigned int off = cluster * 4;
    unsigned int sector = fs.reserved_sectors + off / fs.bytes_per_sector;
    unsigned int offset = off % fs.bytes_per_sector;
    fat_read_sector(sector, fs_buf);
    unsigned int old = *(unsigned int *)(fs_buf + offset);
    *(unsigned int *)(fs_buf + offset) = (old & 0xF0000000) | (value & 0x0FFFFFFF);
    fat_write_sector(sector, fs_buf);
}
unsigned int fat_alloc(void) {
    for (unsigned int c = 2; c < fs.total_clusters + 2; c++) {
        if (fat_next(c) == 0) {
            fat_set(c, 0x0FFFFFFF);
            return c;
        }
    }
    return 0xFFFFFFFF;
}
int fat_open(char *name) {
    unsigned char want[11];
    if (fat_to_83(name, want) != 0) return -1;
    int fd = -1;
    for (int k = 3; k < MAX_FILES; k++) {
        if (!fs_files[k].used) { fd = k; break; }
    }
    if (fd < 0) return -1;
    unsigned int cluster = fs.root_cluster;
    while (cluster < 0x0FFFFFF8) {
        unsigned int sector = fat_cluster_to_sector(cluster);
        for (unsigned int s = 0; s < fs.sectors_per_cluster; s++) {
            fat_read_sector(sector + s, fs_buf);
            for (unsigned int off = 0; off < 512; off += 32) {
                unsigned char *e = fs_buf + off;
                if (e[0] == 0x00) return -1;
                if (e[0] == 0xE5 || e[11] == 0x0F || (e[11] & 0x08)) continue;
                int match = 1;
                for (int k = 0; k < 11; k++) if (e[k] != want[k]) { match = 0; break; }
                if (match) {
                    unsigned int lo = *(unsigned short *)(e + 26);
                    unsigned int hi = *(unsigned short *)(e + 20) & 0x0FFF;
                    fs_files[fd].used = 1;
                    fs_files[fd].first_cluster = (hi << 16) | lo;
                    fs_files[fd].size = *(unsigned int *)(e + 28);
                    fs_files[fd].pos = 0;
                    fs_files[fd].dir_sector = sector + s;
                    fs_files[fd].dir_offset = off;
                    return fd;
                }
            }
        }
        cluster = fat_next(cluster);
    }
    return -1;
}
int fat_find_entry(char *name, unsigned char *entry, unsigned int *sector_out, unsigned int *offset_out) {
    unsigned char want[11];
    if (fat_to_83(name, want) != 0) return -1;
    unsigned int cluster = fs.root_cluster;
    while (cluster < 0x0FFFFFF8) {
        unsigned int sector = fat_cluster_to_sector(cluster);
        for (unsigned int s = 0; s < fs.sectors_per_cluster; s++) {
            fat_read_sector(sector + s, fs_buf);
            for (unsigned int off = 0; off < 512; off += 32) {
                unsigned char *e = fs_buf + off;
                if (e[0] == 0x00) return -1;
                if (e[0] == 0xE5 || e[11] == 0x0F || (e[11] & 0x08)) continue;
                int match = 1;
                for (int k = 0; k < 11; k++) if (e[k] != want[k]) { match = 0; break; }
                if (match) {
                    for (int k = 0; k < 32; k++) entry[k] = e[k];
                    *sector_out = sector + s;
                    *offset_out = off;
                    return 0;
                }
            }
        }
        cluster = fat_next(cluster);
    }
    return -1;
}
int fat_create(char *name) {
    unsigned char want[11];
    if (fat_to_83(name, want) != 0) return -1;
    unsigned char entry[32];
    unsigned int s, o;
    if (fat_find_entry(name, entry, &s, &o) == 0) return 0;
    unsigned int cluster = fs.root_cluster;
    while (cluster < 0x0FFFFFF8) {
        unsigned int sector = fat_cluster_to_sector(cluster);
        for (unsigned int i = 0; i < fs.sectors_per_cluster; i++) {
            fat_read_sector(sector + i, fs_buf);
            for (unsigned int off = 0; off < 512; off += 32) {
                unsigned char *e = fs_buf + off;
                if (e[0] == 0x00 || e[0] == 0xE5) {
                    for (int k = 0; k < 11; k++) e[k] = want[k];
                    e[11] = 0x20;
                    e[12] = 0;
                    for (int k = 13; k < 32; k++) e[k] = 0;
                    fat_write_sector(sector + i, fs_buf);
                    return 0;
                }
            }
        }
        cluster = fat_next(cluster);
    }
    return -1;
}
int fat_update_dir(int fd) {
    if (fd < 0 || fd >= MAX_FILES) return -1;
    struct fs_file *f = &fs_files[fd];
    if (!f->used) return -1;

    fat_read_sector(f->dir_sector, fs_buf);
    unsigned char *e = fs_buf + f->dir_offset;
    *(unsigned short *)(e + 26) = f->first_cluster & 0xFFFF;
    *(unsigned short *)(e + 20) = (f->first_cluster >> 16) & 0x0FFF;
    *(unsigned int  *)(e + 28) = f->size;
    fat_write_sector(f->dir_sector, fs_buf);
    return 0;
}
int fat_write(int fd, void *buf, int len) {
    if (fd < 0 || fd >= MAX_FILES) return -1;
    struct fs_file *f = &fs_files[fd];
    if (!f->used) return -1;
    if (len <= 0) return 0;

    unsigned int bpc = fs.bytes_per_sector * fs.sectors_per_cluster;
    unsigned char *in = (unsigned char *)buf;
    unsigned int done = 0;

    if (f->first_cluster == 0) {
        unsigned int c = fat_alloc();
        if (c == 0xFFFFFFFF) return -1;
        f->first_cluster = c;
    }

    unsigned int cluster = f->first_cluster;
    unsigned int skip = f->pos / bpc;
    for (unsigned int n = 0; n < skip; n++) {
        unsigned int next = fat_next(cluster);
        if (next >= 0x0FFFFFF8) {
            unsigned int c = fat_alloc();
            if (c == 0xFFFFFFFF) return done;
            fat_set(cluster, c);
            next = c;
        }
        cluster = next;
    }

    unsigned int off = f->pos % bpc;

    while (done < (unsigned int)len) {
        unsigned int sec = fat_cluster_to_sector(cluster) + off / fs.bytes_per_sector;
        unsigned int so = off % fs.bytes_per_sector;
        unsigned int chunk = bpc - off;
        if (chunk > (unsigned int)len - done) chunk = (unsigned int)len - done;

        while (chunk > 0) {
            fat_read_sector(sec, fs_buf);
            unsigned int avail = fs.bytes_per_sector - so;
            if (avail > chunk) avail = chunk;
            for (unsigned int k = 0; k < avail; k++)
                fs_buf[so + k] = in[done + k];
            fat_write_sector(sec, fs_buf);
            done += avail;
            chunk -= avail;
            sec++;
            so = 0;
        }

        off = 0;

        if (done < (unsigned int)len) {
            unsigned int next = fat_next(cluster);
            if (next >= 0x0FFFFFF8) {
                unsigned int c = fat_alloc();
                if (c == 0xFFFFFFFF) break;
                fat_set(cluster, c);
                next = c;
            }
            cluster = next;
        }
    }

    f->pos += done;
    if (f->pos > f->size) f->size = f->pos;
    fat_update_dir(fd);

    return done;
}
int fat_read(int fd, void *buf, int len) {
    if (fd < 0 || fd >= MAX_FILES) return -1;
    struct fs_file *f = &fs_files[fd];
    if (!f->used) return -1;
    if (f->pos >= f->size) return 0;
    if (len > f->size - f->pos) len = f->size - f->pos;
    unsigned int bpc = fs.bytes_per_sector * fs.sectors_per_cluster;
    unsigned char *out = (unsigned char *)buf;
    unsigned int done = 0;
    unsigned int cluster = f->first_cluster;
    unsigned int skip = f->pos / bpc;
    for (unsigned int n = 0; n < skip; n++) cluster = fat_next(cluster);
    unsigned int off = f->pos % bpc;
    while (done < len) {
        unsigned int sector = fat_cluster_to_sector(cluster) + off / fs.bytes_per_sector;
        unsigned int so = off % fs.bytes_per_sector;
        unsigned int chunk = bpc - off;
        if (chunk > len - done) chunk = len - done;
        while (chunk > 0) {
            fat_read_sector(sector, fs_buf);
            unsigned int avail = fs.bytes_per_sector - so;
            if (avail > chunk) avail = chunk;
            for (unsigned int k = 0; k < avail; k++) out[done + k] = fs_buf[so + k];
            done += avail;
            chunk -= avail;
            sector++;
            so = 0;
        }
        off = 0;
        if (done < len) cluster = fat_next(cluster);
    }
    f->pos += done;
    return done;
}
void fat_close(int fd) {
    if (fd >= 0 && fd < MAX_FILES) fs_files[fd].used = 0;
}
int fat_delete(char *name) {
    unsigned char entry[32];
    unsigned int sector, offset;
    if (fat_find_entry(name, entry, &sector, &offset) != 0)return -1;
    unsigned int lo = *(unsigned short *)(entry + 26);
    unsigned int hi = *(unsigned short *)(entry + 20);
    unsigned int cluster = ((hi & 0x0FFF) << 16) | lo;
    fat_read_sector(sector, fs_buf);
    fs_buf[offset] = 0xE5;
    fat_write_sector(sector, fs_buf);
    while (cluster >= 2 && cluster < 0x0FFFFFF8) {
        unsigned int next = fat_next(cluster);
        if (next == 0x0FFFFFF7 || next == 0xFFFFFFFE) return -1;
        fat_set(cluster, 0);
        if (next >= 0x0FFFFFF8) break;
        cluster = next;
    }
    return 0;
}
void time_rtctime() {
    io_outb(0x70, 0x00);
    time_current.sec = BCD2BIN(io_inb(0x71));
    io_outb(0x70, 0x02);
    time_current.min = BCD2BIN(io_inb(0x71));
    io_outb(0x70, 0x04);
    time_current.hour = BCD2BIN(io_inb(0x71));
    io_outb(0x70, 0x07);
    time_current.day = BCD2BIN(io_inb(0x71));
    io_outb(0x70, 0x08);
    time_current.mon = BCD2BIN(io_inb(0x71));
    io_outb(0x70, 0x09);
    time_current.year = BCD2BIN(io_inb(0x71));
}
time_t time_time(time_t *tloc) {
    int year = 2000 + time_current.year;
    int days = 0;
    for(int y = 1970; y < year; y++) days += (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)) ? 366 : 365;
    static int mdays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for(int m = 1; m < time_current.mon; m++) {
        days += mdays[m - 1];
        if (m == 2 && year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) days++;
    }
    days += time_current.day - 1;
    time_t time = days * 86400 + time_current.hour * 3600 + time_current.min * 60 + time_current.sec + time_ticks;
    if(tloc) *tloc = time;
    return time;
}
__attribute__((naked)) void user_enter(void *entry, void *stack) {
    asm(
        "movl 4(%esp), %eax\n"
        "movl 8(%esp), %edx\n"
        "movw $0x23, %cx\n"
        "movw %cx, %ds\n"
        "movw %cx, %es\n"
        "movw %cx, %fs\n"
        "movw %cx, %gs\n"
        "pushl $0x23\n"
        "pushl %edx\n"
        "pushfl\n"
        "popl %ecx\n"
        "orl $0x200, %ecx\n"
        "pushl %ecx\n"
        "pushl $0x1B\n"
        "pushl %eax\n"
        "iret\n"
    );
}
void user_tss_set(int index, int base, int limit) {
    unsigned char *desc = gdt_start + index * 8;
    desc[0] = limit & 0xFF;
    desc[1] = (limit >> 8) & 0xFF;
    desc[2] = base & 0xFF;
    desc[3] = (base >> 8) & 0xFF;
    desc[4] = (base >> 16) & 0xFF;
    desc[5] = 0x89;
    desc[6] = (limit >> 16) & 0x0F;
    desc[7] = (base >> 24) & 0xFF;
}
void user_tss_init() {
	user_tss.ss0 = 0x10;
	user_tss.esp0 = kstack + sizeof(kstack);
	user_tss.iomap_base = sizeof(user_tss);
	user_tss_set(5, &user_tss, sizeof(user_tss) - 1);
	asm("ltr %%ax" : : "a"(0x28));
}
