
#ifndef _APIC_H
#define _APIC_H


#define APIC_BROADCAST_ID		       	0xFF


/* APIC version register */
#define	APIC_VERSION(x)				((x) & 0xFF)
#define APIC_VER_NEW				0x10
#define	APIC_MAXREDIR(x)			(((x) >> 16) & 0xFF)
/* APIC id register */
#define	APIC_OLD_ID(x)				((x) >> 24)
#define	APIC_NEW_ID(x)				(((x) >> 24) & 0xF)

#define IOAPIC_REGSEL				0
#define IOAPIC_RW				0x10
#define		IOAPIC_ID			0
#define		IOAPIC_VER			1
#define		IOAPIC_REDIR			0x10
#define LAPIC_ID				0x20
#define LAPIC_VER				0x30
#define LAPIC_TPR				0x80
#define LAPIC_APR				0x90
#define LAPIC_PPR				0xA0
#define LAPIC_EOI				0xB0
#define LAPIC_LDR				0xD0
#define LAPIC_DFR				0xE0
#define LAPIC_SPIV				0xF0
#define		LAPIC_SPIV_ENABLE_APIC		0x100
#define LAPIC_ISR				0x100
#define LAPIC_TMR				0x180
#define LAPIC_IRR				0x200
#define LAPIC_ESR				0x280
#define LAPIC_ICR				0x300
#define		LAPIC_DEST_MASK			0xFFFFFF
#define LAPIC_LVTT				0x320
#define LAPIC_LVTPC		       		0x340
#define LAPIC_LVT0				0x350
#define LAPIC_LVT1				0x360
#define LAPIC_LVTE				0x370
#define LAPIC_TICR				0x380
#define LAPIC_TCCR				0x390
#define LAPIC_TDCR				0x3E0

#endif /* _APIC_H */
