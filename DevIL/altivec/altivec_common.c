//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Last modified: 17/04/2005
// by Meloni Dario
// 
// Description: Common altivec function.
//
//-----------------------------------------------------------------------------

#include "config.h"

#ifdef ALTIVEC_GCC
#include "altivec_common.h"

// from http://developer.apple.com/hardware/ve/alignment.html
vector unsigned char load_unaligned( unsigned char *buffer ) {
	vector unsigned char MSQ, LSQ;
	vector unsigned char mask;
	MSQ = vec_ld(0, buffer); // most significant quadword
	LSQ = vec_ld(15, buffer); // least significant quadword
	mask = vec_lvsl(0, buffer); // create the permute mask
	return vec_perm(MSQ, LSQ, mask);// align the data
}

vector float fill_vector_f( float value ) {
	vector_t vec;
	vec.sf[0] = value;
	vector float temp = vec_ld(0,vec.sf);
	return vec_splat(temp,0);
}


// Format conversion function
void abc2cba_byte( unsigned char *data, unsigned int length, unsigned char *newdata ) {
	register vector unsigned char d0,d1,d2,t0,t1,t2;
	register const vector unsigned char p0 = (vector unsigned char)
		(0x02,0x01,0x00,0x05,0x04,0x03,0x08,0x07,0x06,0x0B,0x0A,0x09,0x0E,0x0D,0x0C,0x11);
	register const vector unsigned char p1 = (vector unsigned char)
		(0x00,0x10,0x04,0x03,0x02,0x07,0x06,0x05,0x0A,0x09,0x08,0x0D,0x0C,0x0B,0x0E,0x0F);
	register const vector unsigned char p2 = (vector unsigned char)
		(0x1E,0x03,0x02,0x01,0x06,0x05,0x04,0x09,0x08,0x07,0x0C,0x0B,0x0A,0x0F,0x0E,0x0D);
	register const vector unsigned char p3 = (vector unsigned char)
		(0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x10,0x0F);
	
	// Approximate at the greatest group fo pixel
	float pixgrp = length / 16;
	length = ((int)(pixgrp*10)%10) > 0 ? pixgrp : pixgrp+1;
	
	while( length >= 3 ) {
		d2 = vec_ld(32,data);
		d1 = vec_ld(16,data);
		d0 = vec_ld(0,data);
		
		t0 = d0; // used to avoid stall in the permutation
		t1 = d1; // not the whole register is overwritten
		t2 = d2;
		
		d0 = vec_perm(d0,t1,p0);
		d1 = vec_perm(d1,t0,p1);
		d2 = vec_perm(d2,t1,p2);
		d1 = vec_perm(d1,t2,p3);
		
		vec_st(d0,0,newdata);
		vec_st(d2,32,newdata);
		vec_st(d1,16,newdata); // forced out of order execution to avoid stall
		
		length -= 3;
		data += 16*3;
		newdata += 16*3;
	}
	
	if( length == 2 ) {
		d0 = vec_ld(0,data);
		d1 = vec_ld(16,data);
		
		t0 = d0;
		t1 = d1;
		
		d0 = vec_perm(d0,t1,p0);
		d1 = vec_perm(d1,t0,p1);
		
		vec_st(d0,0,newdata);
		vec_st(d1,16,newdata);
	} else if( length == 1 ) {
		d0 = vec_ld(0,data);
		d0 = vec_perm(d0,d0,p0);
		vec_st(d0,0,newdata);
	}
}

void abc2cba_short( unsigned short *data, unsigned int length, unsigned short *newdata ) {
	register vector unsigned short d0,d1,d2,t0,t1,t2;
	register const vector unsigned char p0 = (vector unsigned char) // there is an error in here...
		(0x04,0x05,0x02,0x03,0x00,0x01,0x0A,0x0B,0x08,0x09,0x06,0x07,0x10,0x11,0x0E,0x0F);
	register const vector unsigned char p1 = (vector unsigned char)
		(0x1C,0x1D,0x06,0x07,0x04,0x05,0x02,0x03,0x0C,0x0D,0x0A,0x0B,0x08,0x09,0x0E,0x0F);
	register const vector unsigned char p2 = (vector unsigned char)
		(0x00,0x01,0x1E,0x1F,0x08,0x09,0x06,0x07,0x04,0x05,0x0E,0x0F,0x0C,0x0D,0x0A,0x0B);
	
	
	register const vector unsigned char p3 = (vector unsigned char)
	//	(0x00,0x01,0x10,0x11,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F);
		(0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x12,0x13);
	
	// Approximate at the greatest group fo pixel
	float pixgrp = length / 16;
	length = ((int)(pixgrp*10)%10) > 0 ? pixgrp : pixgrp+1;
	
	while( length >= 3 ) {
		d2 = vec_ld(32,data);
		d1 = vec_ld(16,data);
		d0 = vec_ld(0,data);
		
		t0 = d0; // used to avoid stall in the permutation
		t1 = d1; // not the whole register is overwritten
		t2 = d2;
		
		d0 = vec_perm(d0,t1,p0);
		d1 = vec_perm(d1,t0,p1);
		d2 = vec_perm(d2,t1,p2);
		d1 = vec_perm(d1,t2,p3);
		
		vec_st(d0,0,newdata);
		vec_st(d2,32,newdata);
		vec_st(d1,16,newdata); // forced out of order execution to avoid stall
		
		length -= 3;
		newdata = (unsigned short*)( ((unsigned char*)newdata) + 16*3);
		data = (unsigned short*)( ((unsigned char*)data) + 16*3);
	}
	
	if( length == 2 ) {
		d0 = vec_ld(0,data);
		d1 = vec_ld(16,data);
		
		t0 = d0;
		t1 = d1;
		
		d0 = vec_perm(d0,t1,p0);
		d1 = vec_perm(d1,t0,p1);
		
		vec_st(d0,0,newdata);
		vec_st(d1,16,newdata);
	} else if( length == 1 ) {
		d0 = vec_ld(0,data);
		d0 = vec_perm(d0,d0,p0);
		vec_st(d0,0,newdata);
	}
}

#endif // ALTIVEC_GCC