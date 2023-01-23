#ifndef ELF_H
#define ELF_H

/* ELF Types */
typedef unsigned short Elf32_Half;
typedef unsigned short Elf64_Half;

typedef unsigned int   Elf32_Word;
typedef	int            Elf32_Sword;
typedef unsigned int   Elf64_Word;
typedef	int            Elf64_Sword;

typedef unsigned long  Elf32_Xword;
typedef	long           Elf32_Sxword;
typedef unsigned long  Elf64_Xword;
typedef	long           Elf64_Sxword;

typedef unsigned int   Elf32_Addr;
typedef unsigned long  Elf64_Addr;

typedef unsigned int   Elf32_Off;
typedef unsigned long  Elf64_Off;

typedef unsigned short Elf32_Section;
typedef unsigned short Elf64_Section;

typedef Elf32_Half     Elf32_Versym;
typedef Elf64_Half     Elf64_Versym;

/* ELF Headers */

#define EI_NIDENT (16)

typedef struct __elf32_ehdr_t {
  unsigned char	e_ident[EI_NIDENT];	/* Magic number and other info */
  Elf32_Half	e_type;			          /* Object file type */
  Elf32_Half	e_machine;		        /* Architecture */
  Elf32_Word	e_version;		        /* Object file version */
  Elf32_Addr	e_entry;		          /* Entry point virtual address */
  Elf32_Off	  e_phoff;		          /* Program header table file offset */
  Elf32_Off	  e_shoff;		          /* Section header table file offset */
  Elf32_Word	e_flags;		          /* Processor-specific flags */
  Elf32_Half	e_ehsize;		          /* ELF header size in bytes */
  Elf32_Half	e_phentsize;		      /* Program header table entry size */
  Elf32_Half	e_phnum;		          /* Program header table entry count */
  Elf32_Half	e_shentsize;		      /* Section header table entry size */
  Elf32_Half	e_shnum;		          /* Section header table entry count */
  Elf32_Half	e_shstrndx;		        /* Section header string table index */
} elf32_ehdr_t;


/* Magic number */
enum Elf_Ident {
    EI_MAG0		= 0, // 0x7F
    EI_MAG1		= 1, // 'E'
    EI_MAG2		= 2, // 'L'
    EI_MAG3		= 3, // 'F'
    EI_CLASS	= 4, // Architecture (32/64)
    EI_DATA		= 5, // Byte Order
    EI_VERSION	= 6, // ELF Version
    EI_OSABI	= 7, // OS Specific
    EI_ABIVERSION	= 8, // OS Specific
    EI_PAD		= 9  // Padding
};

#define ELFMAG0		0x7f    /* Magic number byte 0 */
#define ELFMAG1		'E'		/* Magic number byte 1 */
#define ELFMAG2		'L'		/* Magic number byte 2 */
#define ELFMAG3		'F'		/* Magic number byte 3 */

#define ELFCLASSNONE	0   /* Invalid class */
#define ELFCLASS32	1		  /* 32-bit objects */
#define ELFCLASS64	2		  /* 64-bit objects */
#define ELFCLASSNUM	3

#define ELFOSABI_NONE		0	/* UNIX System V ABI */
#define ELFOSABI_SYSV		0	/* Alias.  */
#define ELFOSABI_HPUX		1	/* HP-UX */
#define ELFOSABI_NETBSD		2	/* NetBSD.  */
#define ELFOSABI_GNU		3	/* Object uses GNU ELF extensions.  */
#define ELFOSABI_LINUX		ELFOSABI_GNU /* Compatibility alias.  */
#define ELFOSABI_SOLARIS	6	/* Sun Solaris.  */
#define ELFOSABI_AIX		7	/* IBM AIX.  */
#define ELFOSABI_IRIX		8	/* SGI Irix.  */
#define ELFOSABI_FREEBSD	9	/* FreeBSD.  */
#define ELFOSABI_TRU64		10	/* Compaq TRU64 UNIX.  */
#define ELFOSABI_MODESTO	11	/* Novell Modesto.  */
#define ELFOSABI_OPENBSD	12	/* OpenBSD.  */
#define ELFOSABI_ARM_AEABI	64	/* ARM EABI */
#define ELFOSABI_ARM		97	/* ARM */
#define ELFOSABI_STANDALONE	255	/* Standalone (embedded) application */

/* e_types values  */
#define ET_NONE		0		    /* No file type */
#define ET_REL		1		    /* Relocatable file */
#define ET_EXEC		2		    /* Executable file */
#define ET_DYN		3		    /* Shared object file */
#define ET_CORE		4		    /* Core file */
#define	ET_NUM		5		    /* Number of defined types */
#define ET_LOOS		0xfe00		/* OS-specific range start */
#define ET_HIOS		0xfeff		/* OS-specific range end */
#define ET_LOPROC	0xff00		/* Processor-specific range start */
#define ET_HIPROC	0xffff		/* Processor-specific range end */

/* e_machine values (only 2 because we don't need all of them) */
#define EM_NONE		 0	/* No machine */
#define EM_386		 3	/* Intel 80386 */

/* EI_DATA value in magic number */
#define ELFDATA2LSB   1

/* e_version value (only correct one that we support) */
#define EV_CURRENT 1

typedef struct __elf32_phdr_t {
  Elf32_Word	p_type;			/* Segment type */
  Elf32_Off	  p_offset;		/* Segment file offset */
  Elf32_Addr	p_vaddr;		/* Segment virtual address */
  Elf32_Addr	p_paddr;		/* Segment physical address */
  Elf32_Word	p_filesz;		/* Segment size in file */
  Elf32_Word	p_memsz;		/* Segment size in memory */
  Elf32_Word	p_flags;		/* Segment flags */
  Elf32_Word	p_align;		/* Segment alignment */
} elf32_phdr_t;

/* correct values of p_type */
#define PT_LOAD 1

#define INVALID_ELF  -1
#define NCORRECT_ELF -2

int load_elf(char* data, elf32_ehdr_t* header);

int is_elf_correct(elf32_ehdr_t* header);

int is_elf_magic(elf32_ehdr_t* header);

#endif