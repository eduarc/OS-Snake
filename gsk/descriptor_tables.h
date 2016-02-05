// 
// descriptor_tables.h - Defines the interface for initialising the GDT and IDT.
//                       Also defines needed structures.
//                       Based on code from Bran's kernel development tutorials.
//                       Rewritten for JamesM's kernel development tutorials.
//
// Tomado del turorial de JamesM's Kernel development
// Autores: JamesMs y Bran
#ifndef _DTS_H
#define _DTS_H

#include "common.h"

// Initialisation function is publicly accessible.
void init_descriptor_tables();

// A struct describing an interrupt gate.
struct idt_entry_struct {
    uint16 base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
    uint16 sel;                 // Kernel segment selector.
    uint8  always0;             // This must always be zero.
    uint8  flags;               // More flags. See documentation.
    uint16 base_hi;             // The upper 16 bits of the address to jump to.
}_PACKED_;

typedef struct idt_entry_struct idt_entry_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct idt_ptr_struct {
    uint16 limit;
    uint32 base;                // The address of the first element in our idt_entry_t array.
}_PACKED_;

typedef struct idt_ptr_struct idt_ptr_t;

extern void irq0();
extern void irq1();

#endif
