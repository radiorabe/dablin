/*
    DABlin - capital DAB experience
    Copyright (C) 2015-2017 Stefan Pöschel

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ETI_SOURCE_H_
#define ETI_SOURCE_H_

// support 2GB+ files on 32bit systems
#define _FILE_OFFSET_BITS 64

#define DAB2ETI_AUTO_GAIN -10000

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <mutex>
#include <string>
#include <thread>
#include <unistd.h>
#include <fcntl.h>


struct ETI_PROGRESS {
	double value;
	std::string text;
};


// --- ETISourceObserver -----------------------------------------------------------------
class ETISourceObserver {
public:
	virtual ~ETISourceObserver() {};

	virtual void ETIProcessFrame(const uint8_t* /*data*/) {};
	virtual void ETIUpdateProgress(const ETI_PROGRESS /*progress*/) {};
};


// --- ETISource -----------------------------------------------------------------
class ETISource {
protected:
	std::string filename;
	ETISourceObserver *observer;

	std::mutex status_mutex;
	bool do_exit;

	FILE *input_file;

	uint8_t eti_frame[6144];
	size_t eti_frame_count;
	size_t eti_frame_total;
	unsigned long int eti_progress_next_ms;

	bool OpenFile();
	virtual void PrintSource();

	static std::string FramecountToTimecode(size_t value);
public:
	ETISource(std::string filename, ETISourceObserver *observer);
	virtual ~ETISource();

	int Main();
	void DoExit();
};


// --- DAB2ETISource -----------------------------------------------------------------
class DAB2ETISource : public ETISource {
private:
	uint32_t freq;
	std::string binary_name;

	void PrintSource();
public:
	DAB2ETISource(std::string binary, uint32_t freq, int gain, ETISourceObserver *observer);
	~DAB2ETISource();
};




#endif /* ETI_SOURCE_H_ */
