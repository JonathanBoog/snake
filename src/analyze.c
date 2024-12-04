
extern void print(const char *);               // Print a string to console or log
extern void print_dec(unsigned int);


void clear_counters(void){

// Clear the mcycle CSR by writing 0 to it

asm volatile ("csrw mcycle, x0");
asm volatile ("csrw minstret, x0");
asm volatile ("csrw mhpmcounter3, x0");
asm volatile ("csrw mhpmcounter4, x0");
asm volatile ("csrw mhpmcounter5, x0");
asm volatile ("csrw mhpmcounter6, x0");
asm volatile ("csrw mhpmcounter7, x0");
asm volatile ("csrw mhpmcounter8, x0");
asm volatile ("csrw mhpmcounter9, x0");

}

void read_counters(void){
    unsigned int cycle_count, instruction_count, mhpm3_count, mhpm4_count, mhpm5_count, mhpm6_count, mhpm7_count, mhpm8_count, mhpm9_count;

    asm("csrr %0, mcycle" : "=r"(cycle_count) );
    asm("csrr %0, minstret" : "=r"(instruction_count) );
    asm("csrr %0, mhpmcounter3" : "=r"(mhpm3_count) );
    asm("csrr %0, mhpmcounter4" : "=r"(mhpm4_count) );
    asm("csrr %0, mhpmcounter5" : "=r"(mhpm5_count) );
    asm("csrr %0, mhpmcounter6" : "=r"(mhpm6_count) );
    asm("csrr %0, mhpmcounter7" : "=r"(mhpm7_count) );
    asm("csrr %0, mhpmcounter8" : "=r"(mhpm8_count) );
    asm("csrr %0, mhpmcounter9" : "=r"(mhpm9_count) );

    print("\n mcycle: ");
    print_dec(cycle_count);
    print("\n minstret: ");
    print_dec(instruction_count);
    print("\n mhpmcounter3: ");
    print_dec(mhpm3_count);
    print("\n mhpmcounter4: ");
    print_dec(mhpm4_count);
    print("\n mhpmcounter5: ");
    print_dec(mhpm5_count);
    print("\n mhpmcounter6: ");
    print_dec(mhpm6_count);
    print("\n mhpmcounter7: ");
    print_dec(mhpm7_count);
    print("\n mhpmcounter8: ");
    print_dec(mhpm8_count);
    print("\n mhpmcounter9: ");
    print_dec(mhpm9_count);
    
/*
    print("\nExecution time (s) was: ");
    print_dec(cycle_count/30000000);

    print("\nCPI (Cycles/Instr) was: ");
    print_dec(cycle_count/instruction_count);

    print("\nIPC (Instr/Cycle) was: ");
    print_dec(instruction_count/cycle_count);

    print("\nD-cache miss ratio was: ");
    print_dec(mhpm5_count/mhpm3_count);

    print("\nD-cache hit ratio was: ");
    print_dec(1-(mhpm5_count/mhpm3_count));

    print("\nI-cache miss ratio was: ");
    print_dec(mhpm4_count/instruction_count);

    print("\nI-cache hit ratio was: ");
    print_dec(1- (mhpm4_count/instruction_count));

    print("\nALU-stall ratio was: ");
    print_dec(mhpm9_count/cycle_count);

    print("\nMemory Intensity was: ");
    print_dec(mhpm3_count/instruction_count);

    print("\nHazard-stall ratio was: ");
    print_dec(mhpm8_count/cycle_count);

    print("\nCache misses: ");
    print_dec(mhpm4_count+mhpm5_count);
    */
}

