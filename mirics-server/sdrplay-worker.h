#
/*
 *    Copyright (C) 2014
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair programming
 *
 *    This file is part of the SDR-J (JSDR).
 *    Many of the ideas as implemented in JSDR are derived from
 *    other work, made available through the GNU general Public License. 
 *    All copyrights of the original authors are recognized.
 *
 *    SDR-J is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    SDR-J is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with SDR-J; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __SDRPLAY_WORKER
#define	__SDRPLAY_WORKER

#include	<stdlib.h>
#include	<stdio.h>
#include	<math.h>
#include	<string.h>
#include	<unistd.h>
#include	<stdint.h>
#include	<QThread>
#include	"ringbuffer.h"
#include	"sdrplay-loader.h"
#include	"fir-filters.h"

class	sdrplayWorker: public QThread {
Q_OBJECT
public:
			sdrplayWorker	(int32_t,	// dongleRate
	                                 int32_t,	// outputRate,
	                                 int32_t,	// dongle bandwidth
	                                 int32_t,	// initial freq
	                                 sdrplayLoader *,
	                                 RingBuffer<DSPCOMPLEX> *,
	                                 bool *);
			~sdrplayWorker	(void);
	void		setVFOFrequency	(int32_t);
	void		setExternalGain	(int16_t);
	void		stop		(void);
private:
	void			run	(void);
	sdrplayLoader		*functions;	// 
	RingBuffer<DSPCOMPLEX>	*_I_Buffer;
	float			defaultFreq;
	double			deviceRate;
	double			outputRate;
	double			deltaRate;
	int32_t			bandWidth;
	double			lastFrequency;
	int16_t			lastGain;
	bool			runnable;
	DecimatingFIR		*d_filter;
	int32_t			sps;
	int32_t			anyChange;
signals:
	void			sendSamples	(int);
};
#endif

