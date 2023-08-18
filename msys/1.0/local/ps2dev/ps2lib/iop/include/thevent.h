/*
  _____     ___ ____
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2002, Florin Sasu (florinsasu@yahoo.com)
  ------------------------------------------------------------------------
  thevent.h
                           IOP thevent library prototypes
*/

#ifndef __THEVENT_H__
#define __THEVENT_H__

#define THEVENT_VER	0x101

struct t_event {
	int attr;
	int option;
	int init_mask;
};

struct t_eventStatus {
    unsigned int	attr;
    unsigned int	option;
    unsigned int	init_mask;
    unsigned int	current_mask;
    int				num_wait_threads;
};

int CreateEventFlag(struct t_event*);
int SetEventFlag(int evfid, unsigned int bitpattern);
int iSetEventFlag(int evfid, unsigned int bitpattern);
int ClearEventFlag(int evfid, unsigned int bitpattern);
int iClearEventFlag(int evfid, unsigned int bitpattern);	
int WaitEventFlag(int statusFlag, unsigned int bitpattern, int waitmode, unsigned int *resultpat);
int ReferEventFlagStatus(int evfid, struct t_eventStatus *info );
int iReferEventFlagStatus(int evfid, struct t_eventStatus *info );


#endif//__THEVENT_H__
