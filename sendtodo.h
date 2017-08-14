#ifndef SENDTODO_H
#define SENDTODO_H

/* critical timing feature #1: if not triggered, do not busy-loop */
/* critical timing feature #2: if triggered, respond within fixed time */
/* important timing feature: when triggered, respond instantly */
#define SLEEP_TODO 1500 /* check todo/ every 25 minutes in any case */
#define SLEEP_FUZZ 1 /* slop a bit on sleeps to avoid zeno effect */
#define SLEEP_FOREVER 86400 /* absolute maximum time spent in select() */
#define SLEEP_CLEANUP 76431 /* time between cleanups */
#define SLEEP_SYSFAIL 123
#define OSSIFIED 129600 /* 36 hours; _must_ exceed q-q's DEATH (24 hours) */

#endif
