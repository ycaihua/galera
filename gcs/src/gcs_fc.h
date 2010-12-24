/*
 * Copyright (C) 2010 Codership Oy <info@codership.com>
 *
 * $Id$
 */

/*! @file This unit contains Flow Control parts deemed worthy to be 
 *        taken out of gcs.c */

#ifndef _gcs_fc_h_
#define _gcs_fc_h_

#include <time.h>
#include <unistd.h>
#include <errno.h>

typedef struct gcs_fc
{
    ssize_t hard_limit; // hard limit for slave queue size
    ssize_t soft_limit; // soft limit for slave queue size, after it FC kicks in
    double  max_throttle; // limit on how much we can throttle replication
    ssize_t init_size;  // initial queue size
    ssize_t size;       // current queue size
    ssize_t last_sleep; // queue size when last sleep happened
    ssize_t act_count;  // action count
    double  max_rate;   // normal replication data rate (byte/s)
    double  scale;      // data rate scale factor
    double  offset;     // data rate offset (rate = scale*size + offset)
    long long start;    // beginning of the time interval (nanosec, monotonic)
}
gcs_fc_t;

extern double const gcs_fc_hard_limit_fix; //! allow for some overhead

/*! Initializes operational constants before oprning connection to group */
extern int
gcs_fc_init (gcs_fc_t* fc,
             ssize_t   hard_limit,   // hard limit
             double    soft_limit,   // soft limit as a fraction of hard limit
             double    max_throttle);

/*! Reinitializes object at the beginning of state transfer */
extern void
gcs_fc_reset (gcs_fc_t* fc, ssize_t queue_size);

/*! Processes a new action added to a slave queue.
 *  @return 1 if sleep is needed, 0 if not and -ENOMEM in case of hitting
 *          hard limit */
extern int
gcs_fc_process (gcs_fc_t* fc, ssize_t act_size, struct timespec* period);

#endif /* _gcs_fc_h_ */
