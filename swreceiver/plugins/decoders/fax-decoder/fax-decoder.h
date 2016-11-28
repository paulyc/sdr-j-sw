#
/*
 *    Copyright (C) 2008, 2009, 2010
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
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

#ifndef	__FAX_DECODER
#define	__FAX_DECODER

#include	<QFrame>
#include	<QByteArray>
#include	<QImage>
#include	<QSettings>
#include	<QPoint>
#include	"utilities.h"
#include	"ui_widget.h"
#include	"swradio-constants.h"
#include	"decoder-interface.h"
#include	"fax-scroller.h"
#include	"fir-filters.h"
#include	"fax-params.h"

class	RadioInterface;
class	Oscillator;
class	common_fft;
class	faxDemodulator;
class	faxImage;
class	faxFilenames;

/*
 *	states:
 *	we look at the bits
 */
#define	APTSTART		0001
#define	FAX_PHASING		0002
#define	FAX_IMAGING		0004
#define	FAX_DONE		0010

class	faxDecoder: public decoderInterface, public Ui_Form {
Q_OBJECT
Q_PLUGIN_METADATA (IID "faxDecoder")
Q_INTERFACES (decoderInterface)

public:
	QWidget *createPluginWindow	(int32_t, QSettings *);
	        ~faxDecoder ();
	void	doDecode		(DSPCOMPLEX);
	bool	initforRate		(int32_t);
	int32_t	rateOut			(void);
	int16_t	detectorOffset		(void);

	enum Teint {
	   FAX_COLOR		= 1,
	   FAX_GRAY		= 2,
	   FAX_BLACKWHITE	= 3
	};

	enum Mode {
	   FAX_AM	= 100,
	   FAX_FM	= 101
	};

private:
	QSettings	*faxSettings;
	int32_t		theRate;
	QFrame		*theFrame;
	void		setup_faxDecoder	(QSettings *);
	void		initPhasing		(void);
	common_fft	*myfft;
	DSPCOMPLEX	*fftVector;
	int32_t		getFreq			(DSPCOMPLEX *);
	faxParams	*getFaxParams		(QString);
private slots:
	void		reset			(void);
	void		fax_setIOC		(const QString &);
	void		fax_setMode		(const QString &);
	void		fax_setPhase		(const QString &);
	void		fax_setColor		(const QString &);
	void		fax_setDeviation	(int);
	void		fax_setSkip		(void);
	void		fax_setLPM		(int);
	void		fax_setstartFreq	(int);
	void		fax_setstopFreq		(int);
	void		fax_setCarrier		(int);
	void		fax_setsavingonDone	(void);
	void		fax_setClicked		(int, int);
	void		fax_setautoContinue	(void);
private:
	void		correctWidth		(int16_t);
	void		addtoPhasing		(int16_t);
	void		addtoImage		(int16_t);
	void		endReception		(void);
	bool		weFoundaValidPhaseline	(void);
	void		correctLPM		(DSPFLOAT);
	void		fax_saveFile		(QImage, bool);
	void		fax_displayImage	(QImage, int, int);
	void		fax_displayImage	(QImage);
	void		fax_setLPM		(float);

	faxFilenames	*nameGenerator;
	faxScroller	*faxContainer;
	Oscillator	*localOscillator;

	bool		savingRequested;
	void		set_savingRequested	(bool);
	bool		autoContinue;
	void		set_autoContinue	(bool);
	void		addSampletoImage	(float, int32_t, int32_t);
	int32_t		delayCounter;
	faxImage	*theImage;
	faxDemodulator	*myDemodulator;
	LowPassFIR	*faxLowPass;
	average		*faxAverager;
	QByteArray	rawData;
	uint8_t		faxState;
	int32_t		currentSampleIndex;
	DSPCOMPLEX	fax_oldz;
	int16_t		fax_IOC;
	DSPFLOAT	lpm;
	DSPFLOAT	f_lpm;
	int16_t		deviation;
	int16_t		aptCount;
	int16_t		apt_upCrossings;
	int16_t		aptStartFreq;
	int16_t		aptStopFreq;
	int16_t		txLPM;
	bool		phaseInvers;
	uint8_t		faxColor;
	double		lpmSum;
	bool		whiteSide;
	int16_t		currPhaseLength;
	int16_t		whiteLength;
	int16_t		phaseLines;
	int16_t		noPhasingLines;
	int16_t		carrier;
	uint8_t		faxMode;
	int32_t		samplesperLine;
	int16_t		numberofColums;
	int16_t		currentColumn;
	int16_t		lastRow;
	int32_t		pixelValue;
	float		pixelSamples;
	int16_t		currentValue;
	bool		savingFlag;
//	for adjusting, we need
	QPoint		slant1;
	QPoint		slant2;
	bool		pointNeeded;
};

#endif

