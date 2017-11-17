/* -*- c++ -*- */
/* 
 * Copyright 2016 Trinity Connect Centre.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_SVL_SVL_FFT_H
#define INCLUDED_SVL_SVL_FFT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <hydra/api.h>

#include <boost/shared_ptr.hpp>
#include <fftw3.h>
#include <gnuradio/types.h>

namespace gr {
	namespace hydra {

// Implemented my own fft_complex clas.
// GNURADIO's one was not working properly. To work, I would need to perform memory aligments in the input buffers
// So, I implemented this one which has the same function signatures.
class SVL_API fft_complex 
{
	private:
		size_t g_fft_size;
      bool g_forward;
		gr_complex *g_inbuf;
      gr_complex *g_outbuf;
		fftwf_plan g_plan;

	public:
      /** CTOR
       * @param fft_size
       * @param fortward
       */
		fft_complex(size_t fft_size, bool forward = true);

      /**
       */
      int set_data(const gr_complex *data, size_t len);

      /**
       */
		gr_complex* get_inbuf();


      /**
       */
		gr_complex* get_outbuf();

      /**
       */
		void execute();
};

/* TYPEDEFS for this class */
typedef boost::shared_ptr<gr::hydra::fft_complex> sfft_complex;


} /* namespace hydra */
} /* namespace gr */

#endif /* #ifndef INCLUDED_SVL_SVL_FFT_H */
