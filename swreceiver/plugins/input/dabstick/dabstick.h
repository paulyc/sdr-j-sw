#
/*
 *    Copyright (C) 2010, 2011, 2012
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair programming
 *
 *    This file is part of the SDR-J.
 *    Many of the ideas as implemented in SDR-J are derived from
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

#ifndef __DABSTICK_DLL
#define	__DABSTICK_DLL

#include	<QThread>
#include	<QObject>
#include	<QFrame>
#include	"swradio-constants.h"
#include	"rig-interface.h"
#include	"ringbuffer.h"
#include	"fir-filters.h"
#include	"samplerate.h"
#include	"oscillator.h"
#include	"dongleselect.h"

class		QSettings;
class		dll_driver;
class		dabStick;
//
//	create typedefs for the library functions
typedef	struct rtlsdr_dev rtlsdr_dev_t;

typedef	void (*rtlsdr_read_async_cb_t) (uint8_t *buf, uint32_t len, void *ctx);
typedef	 int (*  pfnrtlsdr_open )(rtlsdr_dev_t **, uint32_t);
typedef	int (*  pfnrtlsdr_close) (rtlsdr_dev_t *);
typedef	int (*  pfnrtlsdr_set_center_freq) (rtlsdr_dev_t *, uint32_t);
typedef uint32_t (*  pfnrtlsdr_get_center_freq) (rtlsdr_dev_t *);
typedef	int (*  pfnrtlsdr_get_tuner_gains) (rtlsdr_dev_t *, int *);
typedef	int (*  pfnrtlsdr_set_tuner_gain_mode) (rtlsdr_dev_t *, int);
typedef	int (*  pfnrtlsdr_set_sample_rate) (rtlsdr_dev_t *, uint32_t);
typedef	int (*  pfnrtlsdr_get_sample_rate) (rtlsdr_dev_t *);
typedef	int (*  pfnrtlsdr_set_agc_mode) (rtlsdr_dev_t *, int);
typedef	int (*  pfnrtlsdr_set_tuner_gain) (rtlsdr_dev_t *, int);
typedef	int (*  pfnrtlsdr_get_tuner_gain) (rtlsdr_dev_t *);
typedef int (*  pfnrtlsdr_reset_buffer) (rtlsdr_dev_t *);
typedef	int (*  pfnrtlsdr_read_async) (rtlsdr_dev_t *,
	                               rtlsdr_read_async_cb_t,
	                               void *,
	                               uint32_t,
	                               uint32_t);
typedef int (*  pfnrtlsdr_cancel_async) (rtlsdr_dev_t *);
typedef int (*  pfnrtlsdr_set_direct_sampling) (rtlsdr_dev_t *, int);
typedef uint32_t (*  pfnrtlsdr_get_device_count) (void);
typedef	int (* pfnrtlsdr_set_freq_correction)(rtlsdr_dev_t *, int);
typedef	char *(* pfnrtlsdr_get_device_name)(int);


#include	"ui_widget.h"

class	dll_driver : public QThread {
public:
	dll_driver (dabStick *d);
	~dll_driver (void);
//	For the callback, we do need some environment 
//	This is the user-side call back function
static
void	RTLSDRCallBack (unsigned char *buf, uint32_t len, void *ctx);
private:
virtual void	run (void);
	dabStick	*theStick;
};

class	dabStick: public rigInterface, public Ui_Form {
Q_OBJECT
#if QT_VERSION >= 0x050000
Q_PLUGIN_METADATA (IID "dabstick")
#endif
Q_INTERFACES (rigInterface)

public:
	QWidget	*createPluginWindow	(int32_t, QSettings *);
		~dabStick		(void);
	int32_t	getRate			(void);
	void	setVFOFrequency		(int32_t);
	int32_t	getVFOFrequency		(void);
	bool	legalFrequency		(int32_t);
	int32_t	defaultFrequency	(void);

	bool	restartReader		(void);
	void	stopReader		(void);
	int32_t	Samples			(void);
	int32_t	getSamples		(DSPCOMPLEX *, int32_t, uint8_t);
	int16_t	bitDepth		(void);
	void	exit			(void);
	bool	isOK			(void);
//
//	I_Buffer needs to be visible for use within the callback
	RingBuffer<uint8_t>	*_I_Buffer;
	pfnrtlsdr_read_async rtlsdr_read_async;
	struct rtlsdr_dev	*device;
	int32_t		inputRate;
	void	newdataAvailable	(int);

private slots:
	void		setExternalGain	(int);
	void		setCorrection	(int);
	void		setKhzOffset	(int);
	void		setHzOffset	(int);
	void		set_rateSelector	(const QString &);
	void		setAgc		(int);

private:
	dongleSelect	*dongleSelector;
	QSettings	*dabSettings;
	void		setupDevice	(int32_t, int32_t);
	int32_t		outputRate;
	QFrame		*myFrame;
	HINSTANCE	Handle;
	int32_t		deviceCount;
	dll_driver	*workerHandle;
	bool		libraryLoaded;
	int32_t		vfoFrequency;
	int32_t		vfoOffset;
	bool		open;
	int		*gains;
	int16_t		gainsCount;
	DecimatingFIR	*d_filter;
	int32_t		getInputRate	(int32_t);
//
//	here we need to load functions from the dll
	bool		load_rtlFunctions	(void);
	pfnrtlsdr_open	rtlsdr_open;
	pfnrtlsdr_close	rtlsdr_close;
	pfnrtlsdr_set_center_freq rtlsdr_set_center_freq;
	pfnrtlsdr_get_center_freq rtlsdr_get_center_freq;
	pfnrtlsdr_get_tuner_gains rtlsdr_get_tuner_gains;
	pfnrtlsdr_set_tuner_gain_mode rtlsdr_set_tuner_gain_mode;
	pfnrtlsdr_set_sample_rate rtlsdr_set_sample_rate;
	pfnrtlsdr_get_sample_rate rtlsdr_get_sample_rate;
	pfnrtlsdr_set_agc_mode rtlsdr_set_agc_mode;
	pfnrtlsdr_set_tuner_gain rtlsdr_set_tuner_gain;
	pfnrtlsdr_get_tuner_gain rtlsdr_get_tuner_gain;
	pfnrtlsdr_reset_buffer rtlsdr_reset_buffer;
	pfnrtlsdr_cancel_async rtlsdr_cancel_async;
	pfnrtlsdr_set_direct_sampling	rtlsdr_set_direct_sampling;
	pfnrtlsdr_get_device_count rtlsdr_get_device_count;
	pfnrtlsdr_set_freq_correction rtlsdr_set_freq_correction;
	pfnrtlsdr_get_device_name rtlsdr_get_device_name;
};
#endif

