/*
  _____     ___ ____
   ____|   |    ____|      PS2 OpenSource Project
  |     ___|   |____       (C)2003 ,Mavy & Bigboss ( mavy@ps2reality.net, 
												    bigboss@ps2reality.net )
  ----------------------------------------------------------------------------
  ps2Perf.h  PS2 Performance  Some functions to use performance counters of EE
 
  All rights reserved.

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, and/or sell copies of the
  Software, and to permit persons to whom the Software is furnished to do so,
  provided that the above copyright notice(s) and this permission notice appear
  in all copies of the Software and that both the above copyright notice(s)
  and this permission notice appear in supporting documentation.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS.
  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS INCLUDED IN THIS NOTICE BE
  LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR
  ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
  IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
  OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

  Except as contained in this notice, the name of a copyright holder shall not
  be used in advertising or otherwise to promote the sale, use or other dealings
  in this Software without prior written authorization of the copyright holder.  
*/



#ifndef PS2PERF_H
#define PS2PERF_H


#define PC0	0
#define PC1	1

#define EV0_ProcessorCycle 					1
#define EV0_SingleInstructionIssue			2
#define EV0_BranchIssued					3
#define EV0_BTACmiss						4
#define EV0_ITLBmiss						5
#define EV0_InstructionCacheMiss			6
#define EV0_AccessToDTLB					7
#define EV0_nonBlockingLoad					8
#define EV0_WBBSingleRequest				9
#define EV0_WBBBurstRequest					10
#define EV0_CPUAdressBusBusy				11
#define EV0_InstructionCompleted			12
#define EV0_NonBDSInstrucionCompleted		13	
#define EV0_COP2InstructionCompleted		14
#define EV0_LoadCompleted					15

#define EV1_LowOrderBranchIssued			0
#define EV1_ProcessorCycle   				1
#define EV1_DualInstructionIssue			2
#define EV1_BranchMispredicted				3
#define EV1_TLBmiss							4
#define EV1_DTLBmiss						5
#define EV1_DataCacheMiss					6
#define EV1_WBBSingleRequestUnavailable		7
#define EV1_WBBBurstRequestUnavailable		8
#define EV1_WBBBurstRequestAlmostFull		9
#define EV1_WBBBurstRequestFull				10
#define EV1_CPUDataBusBusy					11
#define EV1_InstructionCompleted			12
#define EV1_NonBDSInstrucionCompleted		13	
#define EV1_COP1InstructionCompleted		14
#define EV1_StoreCompleted					15


typedef struct {
	unsigned long dummy1:1;  		// Always 0
	// PCR0
	unsigned long EXL0:1;			// PCR0 operation in Level 1 (0 Not Counted/1 Counted)
	unsigned long K0:1;				// PCR0 in Kernel Mode (0 Not Counted/1 Counted)
	unsigned long S0:1;				// PCR0 in Supervisor Mode
	unsigned long U0:1;				// PCR0 in User Mode
	unsigned long EVENT0:5;			// Event to Count by PCR0
	// PCR1
	unsigned long dummy2:1;  		// Always 0
	unsigned long EXL1:1;			// PCR1 operation in Level 1 (0 Not Counted/1 Counted)
	unsigned long K1:1;				// PCR1 in Kernel Mode (0 Not Counted/1 Counted)
	unsigned long S1:1;				// PCR1 in Supervisor Mode
	unsigned long U1:1;				// PCR1 in User Mode
	unsigned long EVENT1:5;			// Event to Count by PCR1
	unsigned long dummy3:11;  		// Always 0
	unsigned long CTE:1;			// Counter Enable (1 Enabled)
} PCCR_Register;

typedef struct
{  // <0 => Overflow
	long ProcessorCycle;   				//	1
	long SingleInstructionIssue;		//	2
	long BranchIssued;					//	3
	long BTACmiss;						//	4
	long ITLBmiss;						//	5
	long InstructionCacheMiss;			//	6
	long AccessToDTLB;					//	7
	long nonBlockingLoad;				//	8
	long WBBSingleRequest;				//	9
	long WBBBurstRequest;				//	10
	long CPUAdressBusBusy;				//	11
	long InstructionCompleted;			//	12
	long NonBDSInstrucionCompleted;		//	13
	long COP2InstructionCompleted;		//	14
	long LoadCompleted;					//	15
} Counters0;

typedef struct
{  // <0 => Overflow
	long LowOrderBranchIssued;			//	0
	long ProcessorCycle;   				//	1
	long DualInstructionIssue;			//	2
	long BranchMispredicted;			//	3
	long TLBmiss;						//	4
	long DTLBmiss;						//	5
	long DataCacheMiss;					//	6
	long WBBSingleRequestUnavailable;	//	7
	long WBBBurstRequestUnavailable;	//	8
	long WBBBurstRequestAlmostFull;		//	9
	long WBBBurstRequestFull;			//	10
	long CPUDataBusBusy;				//	11
	long InstructionCompleted;			//	12
	long NonBDSInstrucionCompleted;		//	13	
	long COP1InstructionCompleted;		//	14
	long StoreCompleted;				//	15
} Counters1;


void initPs2Perf 	();
void startPs2Perf 	();
void event0Select 	(char event0);
void mode0Select 	(char level1,char kernel,char supervisor, char user);
void event1Select 	(char event1);
void mode1Select 	(char level1,char kernel,char supervisor, char user);
void stopPs2Perf 	();
void initCounter 	(void * counters,char type);
void flushCounter 	(void * counters,char type,char reStart);	
char * DumpInfo (Counters0 *c0,Counters1 *c1);






#endif
