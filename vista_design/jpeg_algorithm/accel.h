#ifndef __accel_h__
#define __accel_h__

#include <ac_fixed.h>
//#include <math/mgc_ac_math.h>  // MBMB removed

#pragma map_to_operator "Alt_4mult_addsub"
template <bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+2, sign_a || sign_b> Alt_4mult_addsub 
(
	ac_int<width_a,sign_a> a0, ac_int<width_b,sign_b> b0, 
	ac_int<width_a,sign_a> a1, ac_int<width_b,sign_b> b1,
	ac_int<width_a,sign_a> a2, ac_int<width_b,sign_b> b2,
	ac_int<width_a,sign_a> a3, ac_int<width_b,sign_b> b3,
	ac_int<1, false> isAdd01, ac_int<1, false> isAdd23
)
{
	return ((isAdd01 ? (a1*b1 + a0*b0) : (a0*b0 - a1*b1)) + (isAdd23 ? (a3*b3 + a2*b2) : (a2*b2 - a3*b3)));
}

template <bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register, int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+2, iwidth_a+iwidth_b+2, sign_a || sign_b> Alt_4mult_addsub 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_fixed<width_a,iwidth_a,sign_a> a1, ac_fixed<width_b,iwidth_b,sign_b> b1,
	ac_fixed<width_a,iwidth_a,sign_a> a2, ac_fixed<width_b,iwidth_b,sign_b> b2,
	ac_fixed<width_a,iwidth_a,sign_a> a3, ac_fixed<width_b,iwidth_b,sign_b> b3,
	ac_int<1, false> isAdd01, ac_int<1, false> isAdd23
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia1 = a1.template slc<width_a>(0);
    ac_int<width_a,sign_a> ia2 = a2.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia3 = a3.template slc<width_a>(0);
	
	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib1 = b1.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib2 = b2.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib3 = b3.template slc<width_b>(0);

	ac_int<width_a+width_b+2,sign_a||sign_b> c;
    ac_fixed<width_a+width_b+2, iwidth_a+iwidth_b+2, sign_a || sign_b> r;
	
	c = Alt_4mult_addsub<use_extra_input_reg,use_mult_reg,output_register>(ia0,ib0,ia1,ib1,ia2,ib2,ia3,ib3,isAdd01,isAdd23);
    r.set_slc(0,c);
	return r; 
}


#pragma map_to_operator "Alt_3mult_addsub"
template <bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+2, sign_a || sign_b> Alt_3mult_addsub 
(
	ac_int<width_a,sign_a> a0, ac_int<width_b,sign_b> b0, 
	ac_int<width_a,sign_a> a1, ac_int<width_b,sign_b> b1,
	ac_int<width_a,sign_a> a2, ac_int<width_b,sign_b> b2,
	ac_int<1, false> isAdd01
)
{
	return ((isAdd01 ? (a1*b1 + a0*b0) : (a0*b0 - a1*b1)) + a2*b2);
}

template <bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register, int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+2, iwidth_a+iwidth_b+2, sign_a || sign_b> Alt_3mult_addsub 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_fixed<width_a,iwidth_a,sign_a> a1, ac_fixed<width_b,iwidth_b,sign_b> b1,
	ac_fixed<width_a,iwidth_a,sign_a> a2, ac_fixed<width_b,iwidth_b,sign_b> b2,
	ac_int<1, false> isAdd01
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia1 = a1.template slc<width_a>(0);
    ac_int<width_a,sign_a> ia2 = a2.template slc<width_a>(0);

	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib1 = b1.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib2 = b2.template slc<width_b>(0);

	ac_int<width_a+width_b+2,sign_a||sign_b> c;
    ac_fixed<width_a+width_b+2, iwidth_a+iwidth_b+2, sign_a || sign_b> r;
	
	c = Alt_3mult_addsub<use_extra_input_reg,use_mult_reg,output_register>(ia0,ib0,ia1,ib1,ia2,ib2,isAdd01);
    r.set_slc(0,c);
	return r; 
}

#pragma map_to_operator "Alt_2mult_addsub"
template <bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+1, sign_a || sign_b> Alt_2mult_addsub 
(
	ac_int<width_a,sign_a> a0, ac_int<width_b,sign_b> b0, 
	ac_int<width_a,sign_a> a1, ac_int<width_b,sign_b> b1,
	ac_int<1, false> isAdd01
)
{
	return ((isAdd01 ? (a1*b1 + a0*b0) : (a0*b0 - a1*b1)));
}

template <bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register, int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+1, iwidth_a+iwidth_b+1, sign_a || sign_b> Alt_2mult_addsub 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_fixed<width_a,iwidth_a,sign_a> a1, ac_fixed<width_b,iwidth_b,sign_b> b1,
	ac_int<1, false> isAdd01
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia1 = a1.template slc<width_a>(0);

	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib1 = b1.template slc<width_b>(0);

	ac_int<width_a+width_b+1,sign_a||sign_b> c;
    ac_fixed<width_a+width_b+1, iwidth_a+iwidth_b+1, sign_a || sign_b> r;
	
	c = Alt_2mult_addsub<use_extra_input_reg,use_mult_reg,output_register>(ia0,ib0,ia1,ib1,isAdd01);
    r.set_slc(0,c);
	return r; 
}

//#pragma map_to_operator "Alt_4mult_add"
template <bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register, int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+2, sign_a || sign_b> Alt_4mult_add 
(
	ac_int<width_a,sign_a> a0, ac_int<width_b,sign_b> b0, 
	ac_int<width_a,sign_a> a1, ac_int<width_b,sign_b> b1,
	ac_int<width_a,sign_a> a2, ac_int<width_b,sign_b> b2,
	ac_int<width_a,sign_a> a3, ac_int<width_b,sign_b> b3
)
{
	return ((a1*b1 + a0*b0) + (a3*b3 + a2*b2));
}

template <bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register, int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+2, iwidth_a+iwidth_b+2, sign_a || sign_b> Alt_4mult_add 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_fixed<width_a,iwidth_a,sign_a> a1, ac_fixed<width_b,iwidth_b,sign_b> b1,
	ac_fixed<width_a,iwidth_a,sign_a> a2, ac_fixed<width_b,iwidth_b,sign_b> b2,
	ac_fixed<width_a,iwidth_a,sign_a> a3, ac_fixed<width_b,iwidth_b,sign_b> b3
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia1 = a1.template slc<width_a>(0);
    ac_int<width_a,sign_a> ia2 = a2.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia3 = a3.template slc<width_a>(0);
	
	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib1 = b1.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib2 = b2.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib3 = b3.template slc<width_b>(0);

	ac_int<width_a+width_b+2,sign_a||sign_b> c;
    ac_fixed<width_a+width_b+2, iwidth_a+iwidth_b+2, sign_a || sign_b> r;
	
	c = Alt_4mult_add<use_extra_input_reg,use_mult_reg,output_register>(ia0,ib0,ia1,ib1,ia2,ib2,ia3,ib3);
    r.set_slc(0,c);
	return r; 
}


#pragma map_to_operator "Alt_4mult_sub"
template <bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+2, sign_a || sign_b> Alt_4mult_sub 
(
	ac_int<width_a,sign_a> a0, ac_int<width_b,sign_b> b0, 
	ac_int<width_a,sign_a> a1, ac_int<width_b,sign_b> b1,
	ac_int<width_a,sign_a> a2, ac_int<width_b,sign_b> b2,
	ac_int<width_a,sign_a> a3, ac_int<width_b,sign_b> b3
)
{
	return ((a0*b0 - a1*b1) + (a2*b2 - a3*b3));
}

template <bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register, int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+2, iwidth_a+iwidth_b+2, sign_a || sign_b> Alt_4mult_sub 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_fixed<width_a,iwidth_a,sign_a> a1, ac_fixed<width_b,iwidth_b,sign_b> b1,
	ac_fixed<width_a,iwidth_a,sign_a> a2, ac_fixed<width_b,iwidth_b,sign_b> b2,
	ac_fixed<width_a,iwidth_a,sign_a> a3, ac_fixed<width_b,iwidth_b,sign_b> b3
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia1 = a1.template slc<width_a>(0);
    ac_int<width_a,sign_a> ia2 = a2.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia3 = a3.template slc<width_a>(0);
	
	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib1 = b1.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib2 = b2.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib3 = b3.template slc<width_b>(0);

	ac_int<width_a+width_b+2,sign_a||sign_b> c;
    ac_fixed<width_a+width_b+2, iwidth_a+iwidth_b+2, sign_a || sign_b> r;
	
	c = Alt_4mult_sub<use_extra_input_reg,use_mult_reg,output_register>(ia0,ib0,ia1,ib1,ia2,ib2,ia3,ib3);
    r.set_slc(0,c);
	return r; 
}


#pragma map_to_operator "Alt_3mult_add"
template <bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+2, sign_a || sign_b> Alt_3mult_add 
(
	ac_int<width_a,sign_a> a0, ac_int<width_b,sign_b> b0, 
	ac_int<width_a,sign_a> a1, ac_int<width_b,sign_b> b1,
	ac_int<width_a,sign_a> a2, ac_int<width_b,sign_b> b2
)
{
	return ((a1*b1 + a0*b0) + a2*b2);
}

template <bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register, int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+2, iwidth_a+iwidth_b+2, sign_a || sign_b> Alt_3mult_add 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_fixed<width_a,iwidth_a,sign_a> a1, ac_fixed<width_b,iwidth_b,sign_b> b1,
	ac_fixed<width_a,iwidth_a,sign_a> a2, ac_fixed<width_b,iwidth_b,sign_b> b2
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia1 = a1.template slc<width_a>(0);
    ac_int<width_a,sign_a> ia2 = a2.template slc<width_a>(0);

	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib1 = b1.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib2 = b2.template slc<width_b>(0);

	ac_int<width_a+width_b+2,sign_a||sign_b> c;
    ac_fixed<width_a+width_b+2, iwidth_a+iwidth_b+2, sign_a || sign_b> r;
	
	c = Alt_3mult_add<use_extra_input_reg,use_mult_reg,output_register>(ia0,ib0,ia1,ib1,ia2,ib2);
    r.set_slc(0,c);
	return r; 
}

#pragma map_to_operator "Alt_3mult_sub"
template <bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+2, sign_a || sign_b> Alt_3mult_sub 
(
	ac_int<width_a,sign_a> a0, ac_int<width_b,sign_b> b0, 
	ac_int<width_a,sign_a> a1, ac_int<width_b,sign_b> b1,
	ac_int<width_a,sign_a> a2, ac_int<width_b,sign_b> b2
)
{
	return ((a0*b0 - a1*b1) + a2*b2);
}

template <bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register, int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+2, iwidth_a+iwidth_b+2, sign_a || sign_b> Alt_3mult_sub 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_fixed<width_a,iwidth_a,sign_a> a1, ac_fixed<width_b,iwidth_b,sign_b> b1,
	ac_fixed<width_a,iwidth_a,sign_a> a2, ac_fixed<width_b,iwidth_b,sign_b> b2
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia1 = a1.template slc<width_a>(0);
    ac_int<width_a,sign_a> ia2 = a2.template slc<width_a>(0);

	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib1 = b1.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib2 = b2.template slc<width_b>(0);

	ac_int<width_a+width_b+2,sign_a||sign_b> c;
    ac_fixed<width_a+width_b+2, iwidth_a+iwidth_b+2, sign_a || sign_b> r;
	
	c = Alt_3mult_sub<use_extra_input_reg,use_mult_reg,output_register>(ia0,ib0,ia1,ib1,ia2,ib2);
    r.set_slc(0,c);
	return r; 
}


#pragma map_to_operator "Alt_2mult_add"
template <bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+1, sign_a || sign_b> Alt_2mult_add 
(
	ac_int<width_a,sign_a> a0, ac_int<width_b,sign_b> b0, 
	ac_int<width_a,sign_a> a1, ac_int<width_b,sign_b> b1
)
{
	return (a1*b1 + a0*b0);
}

template <bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register, int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+1, iwidth_a+iwidth_b+1, sign_a || sign_b> Alt_2mult_add
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_fixed<width_a,iwidth_a,sign_a> a1, ac_fixed<width_b,iwidth_b,sign_b> b1
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia1 = a1.template slc<width_a>(0);

	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib1 = b1.template slc<width_b>(0);

	ac_int<width_a+width_b+1,sign_a||sign_b> c;
    ac_fixed<width_a+width_b+1, iwidth_a+iwidth_b+1, sign_a || sign_b> r;
	
	c = Alt_2mult_add<use_extra_input_reg,use_mult_reg,output_register>(ia0,ib0,ia1,ib1);
    r.set_slc(0,c);
	return r; 
}

#pragma map_to_operator "Alt_2mult_sub"
template <bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+1, sign_a || sign_b> Alt_2mult_sub 
(
	ac_int<width_a,sign_a> a0, ac_int<width_b,sign_b> b0, 
	ac_int<width_a,sign_a> a1, ac_int<width_b,sign_b> b1
)
{
	return (a0*b0 - a1*b1);
}

template <bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register, int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+1, iwidth_a+iwidth_b+1, sign_a || sign_b> Alt_2mult_sub 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_fixed<width_a,iwidth_a,sign_a> a1, ac_fixed<width_b,iwidth_b,sign_b> b1
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia1 = a1.template slc<width_a>(0);

	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib1 = b1.template slc<width_b>(0);

	ac_int<width_a+width_b+1,sign_a||sign_b> c;
    ac_fixed<width_a+width_b+1, iwidth_a+iwidth_b+1, sign_a || sign_b> r;
	
	c = Alt_2mult_sub<use_extra_input_reg,use_mult_reg,output_register>(ia0,ib0,ia1,ib1);
    r.set_slc(0,c);
	return r; 
}

#pragma map_to_operator "Alt_divide_quotient"
template <unsigned int output_register,int width_a, bool sign_a, int width_b>
ac_int<width_a, sign_a> Alt_divide_quotient(
	ac_int<width_a,sign_a> a, 
	ac_int<width_b,sign_a> b
)
{
	ac_int<width_a, sign_a> q;
	div(a,b,q);
    return q;
}

template <unsigned int output_register,int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b>
ac_fixed<width_a, iwidth_a, sign_a> Alt_divide_quotient
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_a> b0
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);

	ac_int<width_b,sign_a> ib0 = b0.template slc<width_b>(0);

	ac_int<width_a, sign_a> q;
	ac_fixed<width_a, iwidth_a, sign_a> qf;

	div(a0,b0,q);

    qf.set_slc(0,q);
	return qf; 
}

#pragma map_to_operator "Alt_divide_remainder"
template <unsigned int output_register,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_b, sign_a> Alt_divide_remainder(
	ac_int<width_a,sign_a> a, 
	ac_int<width_b,sign_a> b
)
{
	ac_int<width_a, sign_a> q,r;
	div(a,b,q,r);
    return r;
}

template <unsigned int output_register,int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a, iwidth_a, sign_a> Alt_divide_remainder
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);

	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);

	ac_int<width_a, sign_a> q;
	ac_int<width_a, sign_a> r;
	ac_fixed<width_a, iwidth_a, sign_a> rf;

	div(a0,b0,q,r);

    rf.set_slc(0,r);
	return rf; 
}

#pragma map_to_operator "Alt_abs"
template <int width_a, bool sign_a>
ac_int<width_a,sign_a> Alt_abs(
  ac_int<width_a,sign_a> a
)
{
	if (a < 0)
	{
		return -a;
	}
	return a;
}

template <int width_a, int iwidth_a, bool sign_a>
ac_fixed<width_a,iwidth_a,sign_a> Alt_abs(
  ac_fixed<width_a,iwidth_a,sign_a> a
)
{
	ac_int<width_a,sign_a> ia = a.template slc<width_a>(0);
	ac_fixed<width_a, iwidth_a, sign_a> af;

	ia = Alt_abs(ia);
	af.set_slc(0,ia);
	return af;
}

#pragma map_to_operator "Alt_sqrt"
template <unsigned int output_register, int width_a>
ac_int<width_a,false> Alt_sqrt(
	ac_int<width_a,false> a
)
{
  ac_int<width_a,false> n1;
  sqrt(a,n1);
  return n1;
}

template <unsigned int output_register, int width_a, int iwidth_a>
ac_fixed<width_a,iwidth_a,false> Alt_sqrt(
	ac_fixed<width_a,iwidth_a,false> a
)
{
 	ac_int<width_a,false> ia = a.template slc<width_a>(0);
	ac_fixed<width_a, iwidth_a, false> af;

	ia = Alt_sqrt(ia);
	
	af.set_slc(0,ia);
	return af;
}

#pragma map_to_operator "Alt_sqrt_remainder"
template <unsigned int output_register, int width_a>
ac_int<width_a,false> Alt_sqrt_remainder(
	ac_int<width_a,false> a
)
{
 	ac_int<width_a,false> n  = 1;
  	ac_int<width_a,false> n1 = (n + a /n) >> 1;

	ac_int<width_a,false> diff = Alt_abs<width_a,false>(n1 - n);
	while(diff > 1) {
    		n  = n1;
	    	n1 = (n + a/n) >> 1;
		diff = Alt_abs<width_a,false>(n1 - n);
	}
  	while((n1*n1) > a) {
	    n1 -= 1;
	}
	ac_int<width_a,false> remain = a - n1*n1;
	return remain;
}

#pragma map_to_operator "Alt_shift_taps"
template <int opID, unsigned int tap_distance, int width_a, bool sign_a>
ac_int<width_a,sign_a> Alt_shift_taps(
	ac_int<width_a,sign_a> a
)
{
	static ac_int<width_a,sign_a> contents[tap_distance];
	static int cur_ptr = 0;
	ac_int<width_a,sign_a> result;
	
	contents[cur_ptr] = a;

	if (cur_ptr <tap_distance-1)
	{
		result = contents[cur_ptr+1];
	}
	else
	{
		result = contents[0];
	}
		
	cur_ptr++;	
	if (cur_ptr >= tap_distance)
	{
		cur_ptr = 0;	
	}	
   	return result;
}

template <int opID, unsigned int tap_distance, int width_a, int iwidth_a, bool sign_a>
ac_fixed<width_a,iwidth_a,sign_a> Alt_shift_taps(
	ac_fixed<width_a,iwidth_a,sign_a> a
)
{
	ac_int<width_a,sign_a> ia = a.template slc<width_a>(0);
    ac_fixed<width_a, iwidth_a, sign_a> af;
	
	ia = Alt_shift_taps<opID,tap_distance>(ia);
	af.set_slc(0,ia);	

   	return af;
}

#pragma map_to_operator "Alt_barrel_shift"
template <unsigned int pipeline, int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a,sign_a> Alt_barrel_shift(
	ac_int<width_a,sign_a> a, 
	ac_int<width_b,sign_b> b,
	ac_int<1, false> dir
)
{
	ac_int<width_a,sign_a> c;
	if (dir)
	{
		c = a << b;
	}
	else
	{
		c = a >> b;	
	}
	return c;
}

template <unsigned int pipeline, int width_a, int iwidth_a, bool sign_a, int width_b, bool sign_b>
ac_fixed<width_a,iwidth_a,sign_a>  Alt_barrel_shift(
	ac_fixed<width_a,iwidth_a,sign_a>  a, 
	ac_int<width_b,sign_b> b,
	ac_int<1, false> dir
)
{

	ac_int<width_a,sign_a> ia = a.template slc<width_a>(0);

	ac_int<width_a,sign_a> c;
	ac_fixed<width_a,iwidth_a,sign_a>  cf;
	
	c = Alt_barrel_shift<pipeline>(ia,b,dir);

	cf.set_slc(0,c);

	return cf;
}

#pragma map_to_operator "Alt_1mult_addsub_accum"
template <int opID, bool use_extra_input_reg, bool use_mult_reg,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+16, sign_a || sign_b> Alt_1mult_addsub_accum(
	ac_int<width_a,sign_a> a, 
	ac_int<width_b,sign_b> b, 
	ac_int<1, false> accum_sload,
	ac_int<1, false> isAdd
)
{
    static ac_int<width_a+width_b+16, sign_a || sign_b> acc;
    if (accum_sload == 1)
    {
      acc = 0;
    }
    if (isAdd == 1)
    {
      acc += a*b;
    }
    else 
    {
      acc -= a*b;
    }     
    return acc;
}

template <int opID, bool use_extra_input_reg, bool use_mult_reg, int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+16, iwidth_a+iwidth_b+16, sign_a || sign_b> Alt_1mult_addsub_accum 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_int<1, false> accum_sload,
	ac_int<1, false> isAdd
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);

	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);

	ac_int<width_a+width_b+16,sign_a||sign_b> c;
    ac_fixed<width_a+width_b+16, iwidth_a+iwidth_b+16, sign_a || sign_b> r;
	
	c = Alt_1mult_addsub_accum<opID,use_extra_input_reg,use_mult_reg>(ia0,ib0,accum_sload,isAdd);
    r.set_slc(0,c);
	return r; 
}

#pragma map_to_operator "Alt_1mult_add_accum"
template <int opID, bool use_extra_input_reg, bool use_mult_reg,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+16, sign_a || sign_b> Alt_1mult_add_accum(
	ac_int<width_a,sign_a> a, 
	ac_int<width_b,sign_b> b, 
	ac_int<1, false> accum_sload
)
{
    static ac_int<width_a+width_b+16, sign_a || sign_b> acc;
    if (accum_sload == 1)
    {
      acc = 0;
    }
    acc += a*b;
    return acc;
}

template <int opID, bool use_extra_input_reg, bool use_mult_reg, int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+16, iwidth_a+iwidth_b+16, sign_a || sign_b> Alt_1mult_add_accum 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_int<1, false> accum_sload
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);

	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);

	ac_int<width_a+width_b+16,sign_a||sign_b> c;
    ac_fixed<width_a+width_b+16, iwidth_a+iwidth_b+16, sign_a || sign_b> r;
	
	c = Alt_1mult_add_accum<opID,use_extra_input_reg,use_mult_reg>(ia0,ib0,accum_sload);
    r.set_slc(0,c);
	return r; 
}

#pragma map_to_operator "Alt_1mult_sub_accum"
template <int opID, bool use_extra_input_reg, bool use_mult_reg,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+16, sign_a || sign_b> Alt_1mult_sub_accum(
	ac_int<width_a,sign_a> a, 
	ac_int<width_b,sign_b> b, 
	ac_int<1, false> accum_sload
)
{
    static ac_int<width_a+width_b+16, sign_a || sign_b> acc;
    if (accum_sload == 1)
    {
      acc = 0;
    }
    acc -= a*b;
    return acc;
}

template <int opID, bool use_extra_input_reg, bool use_mult_reg, int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+16, iwidth_a+iwidth_b+16, sign_a || sign_b> Alt_1mult_sub_accum 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_int<1, false> accum_sload
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);

	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);

	ac_int<width_a+width_b+16,sign_a||sign_b> c;
    ac_fixed<width_a+width_b+16, iwidth_a+iwidth_b+16, sign_a || sign_b> r;
	
	c = Alt_1mult_sub_accum<opID,use_extra_input_reg,use_mult_reg>(ia0,ib0,accum_sload);
    r.set_slc(0,c);
	return r; 
}

#pragma map_to_operator "Alt_1mult_add_accum_siii"
template <int opID, bool use_extra_input_reg, bool use_mult_reg,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+8, sign_a || sign_b> Alt_1mult_add_accum_siii(
	ac_int<width_a,sign_a> a, 
	ac_int<width_b,sign_b> b, 
	ac_int<1, false> accum_sload
)
{
    static ac_int<width_a+width_b+8, sign_a || sign_b> acc;
    if (accum_sload == 1)
    {
      acc = 0;
    }
    acc += a*b;
    return acc;
}

template <int opID, bool use_extra_input_reg, bool use_mult_reg, int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+8, iwidth_a+iwidth_b+8, sign_a || sign_b> Alt_1mult_add_accum_siii 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_int<1, false> accum_sload
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);

	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);

	ac_int<width_a+width_b+8,sign_a||sign_b> c;
    ac_fixed<width_a+width_b+8, iwidth_a+iwidth_b+8, sign_a || sign_b> r;
	
	c = Alt_1mult_add_accum_siii<use_extra_input_reg,use_mult_reg>(ia0,ib0,accum_sload);
    r.set_slc(0,c);
	return r; 
}

#pragma map_to_operator "Alt_1mult_sub_accum_siii"
template <int opID, bool use_extra_input_reg, bool use_mult_reg,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+8, sign_a || sign_b> Alt_1mult_sub_accum_siii(
	ac_int<width_a,sign_a> a, 
	ac_int<width_b,sign_b> b, 
	ac_int<1, false> accum_sload
)
{
    static ac_int<width_a+width_b+8, sign_a || sign_b> acc;
    if (accum_sload == 1)
    {
      acc = 0;
    }
    acc -= a*b;
    return acc;
}

template <int opID, bool use_extra_input_reg, bool use_mult_reg, int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+8, iwidth_a+iwidth_b+8, sign_a || sign_b> Alt_1mult_sub_accum_siii 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_int<1, false> accum_sload
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);

	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);

	ac_int<width_a+width_b+8,sign_a||sign_b> c;
    ac_fixed<width_a+width_b+8, iwidth_a+iwidth_b+8, sign_a || sign_b> r;
	
	c = Alt_1mult_sub_accum_siii<use_extra_input_reg,use_mult_reg>(ia0,ib0,accum_sload);
    r.set_slc(0,c);
	return r; 
}

#pragma map_to_operator "Alt_4mult_add_accum"
template <int opID, bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+8, sign_a || sign_b> Alt_4mult_add_accum 
(
	ac_int<width_a,sign_a> a0, ac_int<width_b,sign_b> b0, 
	ac_int<width_a,sign_a> a1, ac_int<width_b,sign_b> b1,
	ac_int<width_a,sign_a> a2, ac_int<width_b,sign_b> b2,
	ac_int<width_a,sign_a> a3, ac_int<width_b,sign_b> b3,
	ac_int<1, false> accum_sload
)
{
	static ac_int<width_a+width_b+8, sign_a || sign_b> acc;
	if (accum_sload == 1)
	{
		acc = 0;
	}
	acc += ((a1*b1 + a0*b0) + (a3*b3 + a2*b2));
	return acc;
}

template <int opID, bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+8,iwidth_a+iwidth_b+8, sign_a || sign_b> Alt_4mult_add_accum 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_fixed<width_a,iwidth_a,sign_a> a1, ac_fixed<width_b,iwidth_b,sign_b> b1,
	ac_fixed<width_a,iwidth_a,sign_a> a2, ac_fixed<width_b,iwidth_b,sign_b> b2,
	ac_fixed<width_a,iwidth_a,sign_a> a3, ac_fixed<width_b,iwidth_b,sign_b> b3,
	ac_int<1, false> accum_sload
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia1 = a1.template slc<width_a>(0);
    ac_int<width_a,sign_a> ia2 = a2.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia3 = a3.template slc<width_a>(0);
	
	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib1 = b1.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib2 = b2.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib3 = b3.template slc<width_b>(0);

	ac_int<width_a+width_b+8,sign_a||sign_b> acc;
    ac_fixed<width_a+width_b+8, iwidth_a+iwidth_b+8, sign_a || sign_b> accf;
	
	acc = Alt_4mult_add_accum <opID,use_extra_input_reg,use_mult_reg,output_register>(ia0,ib0,ia1,ib1,ia2,ib2,ia3,ib3,accum_sload);
    accf.set_slc(0,acc);
	return accf;
}

#pragma map_to_operator "Alt_4mult_sub_accum"
template <int opID, bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+8, sign_a || sign_b> Alt_4mult_sub_accum 
(
	ac_int<width_a,sign_a> a0, ac_int<width_b,sign_b> b0, 
	ac_int<width_a,sign_a> a1, ac_int<width_b,sign_b> b1,
	ac_int<width_a,sign_a> a2, ac_int<width_b,sign_b> b2,
	ac_int<width_a,sign_a> a3, ac_int<width_b,sign_b> b3,
	ac_int<1, false> accum_sload
)
{
	static ac_int<width_a+width_b+8, sign_a || sign_b> acc;
	if (accum_sload == 1)
	{
		acc = 0;
	}
	acc -= ((a0*b0 - a1*b1) + (a2*b2 - a3*b3));
	return acc;
}

template <int opID, bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+8,iwidth_a+iwidth_b+8, sign_a || sign_b> Alt_4mult_sub_accum 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_fixed<width_a,iwidth_a,sign_a> a1, ac_fixed<width_b,iwidth_b,sign_b> b1,
	ac_fixed<width_a,iwidth_a,sign_a> a2, ac_fixed<width_b,iwidth_b,sign_b> b2,
	ac_fixed<width_a,iwidth_a,sign_a> a3, ac_fixed<width_b,iwidth_b,sign_b> b3,
	ac_int<1, false> accum_sload
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia1 = a1.template slc<width_a>(0);
    ac_int<width_a,sign_a> ia2 = a2.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia3 = a3.template slc<width_a>(0);
	
	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib1 = b1.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib2 = b2.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib3 = b3.template slc<width_b>(0);

	ac_int<width_a+width_b+8,sign_a||sign_b> acc;
    ac_fixed<width_a+width_b+8, iwidth_a+iwidth_b+8, sign_a || sign_b> accf;
	
	acc = Alt_4mult_sub_accum <opID,use_extra_input_reg,use_mult_reg,output_register>(ia0,ib0,ia1,ib1,ia2,ib2,ia3,ib3,accum_sload);
    accf.set_slc(0,acc);
	return accf;
}

#pragma map_to_operator "Alt_3mult_add_accum"
template <int opID, bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+8, sign_a || sign_b> Alt_3mult_add_accum 
(
	ac_int<width_a,sign_a> a0, ac_int<width_b,sign_b> b0, 
	ac_int<width_a,sign_a> a1, ac_int<width_b,sign_b> b1,
	ac_int<width_a,sign_a> a2, ac_int<width_b,sign_b> b2,
	ac_int<1, false> accum_sload
)
{
	static ac_int<width_a+width_b+8, sign_a || sign_b> acc;
	if (accum_sload == 1)
	{
		acc = 0;
	}
	acc += ((a1*b1 + a0*b0) + a2*b2);
	return acc;
}

template <int opID, bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+8,iwidth_a+iwidth_b+8, sign_a || sign_b> Alt_3mult_add_accum 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_fixed<width_a,iwidth_a,sign_a> a1, ac_fixed<width_b,iwidth_b,sign_b> b1,
	ac_fixed<width_a,iwidth_a,sign_a> a2, ac_fixed<width_b,iwidth_b,sign_b> b2,
	ac_int<1, false> accum_sload
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia1 = a1.template slc<width_a>(0);
    ac_int<width_a,sign_a> ia2 = a2.template slc<width_a>(0);
	
	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib1 = b1.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib2 = b2.template slc<width_b>(0);

	ac_int<width_a+width_b+8,sign_a||sign_b> acc;
    ac_fixed<width_a+width_b+8, iwidth_a+iwidth_b+8, sign_a || sign_b> accf;
	
	acc = Alt_3mult_add_accum <opID,use_extra_input_reg,use_mult_reg,output_register>(ia0,ib0,ia1,ib1,ia2,ib2,accum_sload);
    accf.set_slc(0,acc);
	return accf;
}

#pragma map_to_operator "Alt_3mult_sub_accum"
template <int opID, bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+8, sign_a || sign_b> Alt_3mult_sub_accum 
(
	ac_int<width_a,sign_a> a0, ac_int<width_b,sign_b> b0, 
	ac_int<width_a,sign_a> a1, ac_int<width_b,sign_b> b1,
	ac_int<width_a,sign_a> a2, ac_int<width_b,sign_b> b2,
	ac_int<1, false> accum_sload
)
{
	static ac_int<width_a+width_b+8, sign_a || sign_b> acc;
	if (accum_sload == 1)
	{
		acc = 0;
	}
	acc -= ((a0*b0 - a1*b1) + a2*b2);
	return acc;
}

template <int opID, bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+8,iwidth_a+iwidth_b+8, sign_a || sign_b> Alt_3mult_sub_accum 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_fixed<width_a,iwidth_a,sign_a> a1, ac_fixed<width_b,iwidth_b,sign_b> b1,
	ac_fixed<width_a,iwidth_a,sign_a> a2, ac_fixed<width_b,iwidth_b,sign_b> b2,
	ac_int<1, false> accum_sload
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia1 = a1.template slc<width_a>(0);
    ac_int<width_a,sign_a> ia2 = a2.template slc<width_a>(0);
	
	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib1 = b1.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib2 = b2.template slc<width_b>(0);

	ac_int<width_a+width_b+8,sign_a||sign_b> acc;
    ac_fixed<width_a+width_b+8, iwidth_a+iwidth_b+8, sign_a || sign_b> accf;
	
	acc = Alt_3mult_sub_accum <opID,use_extra_input_reg,use_mult_reg,output_register>(ia0,ib0,ia1,ib1,ia2,ib2,accum_sload);
    accf.set_slc(0,acc);
	return accf;
}

#pragma map_to_operator "Alt_2mult_add_accum"
template <int opID, bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+8, sign_a || sign_b> Alt_2mult_add_accum 
(
	ac_int<width_a,sign_a> a0, ac_int<width_b,sign_b> b0, 
	ac_int<width_a,sign_a> a1, ac_int<width_b,sign_b> b1,
	ac_int<1, false> accum_sload
)
{
	static ac_int<width_a+width_b+8, sign_a || sign_b> acc;
	if (accum_sload == 1)
	{
		acc = 0;
	}
	acc += (a1*b1 + a0*b0);
	return acc;
}

template <int opID, bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+8,iwidth_a+iwidth_b+8, sign_a || sign_b> Alt_2mult_add_accum 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_fixed<width_a,iwidth_a,sign_a> a1, ac_fixed<width_b,iwidth_b,sign_b> b1,
	ac_int<1, false> accum_sload
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia1 = a1.template slc<width_a>(0);
	
	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib1 = b1.template slc<width_b>(0);

	ac_int<width_a+width_b+8,sign_a||sign_b> acc;
    ac_fixed<width_a+width_b+8, iwidth_a+iwidth_b+8, sign_a || sign_b> accf;
	
	acc = Alt_2mult_add_accum <opID,use_extra_input_reg,use_mult_reg,output_register>(ia0,ib0,ia1,ib1,accum_sload);
    accf.set_slc(0,acc);
	return accf;
}

#pragma map_to_operator "Alt_2mult_sub_accum"
template <int opID, bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, bool sign_a, int width_b, bool sign_b>
ac_int<width_a+width_b+8, sign_a || sign_b> Alt_2mult_sub_accum 
(
	ac_int<width_a,sign_a> a0, ac_int<width_b,sign_b> b0, 
	ac_int<width_a,sign_a> a1, ac_int<width_b,sign_b> b1,
	ac_int<1, false> accum_sload
)
{
	static ac_int<width_a+width_b+8, sign_a || sign_b> acc;
	if (accum_sload == 1)
	{
		acc = 0;
	}
	acc -= (a0*b0 - a1*b1);
	return acc;
}

template <int opID, bool use_extra_input_reg, bool use_mult_reg, unsigned int output_register,int width_a, int iwidth_a, bool sign_a, int width_b, int iwidth_b, bool sign_b>
ac_fixed<width_a+width_b+8,iwidth_a+iwidth_b+8, sign_a || sign_b> Alt_2mult_sub_accum 
(
	ac_fixed<width_a,iwidth_a,sign_a> a0, ac_fixed<width_b,iwidth_b,sign_b> b0, 
	ac_fixed<width_a,iwidth_a,sign_a> a1, ac_fixed<width_b,iwidth_b,sign_b> b1,
	ac_int<1, false> accum_sload
)
{
	ac_int<width_a,sign_a> ia0 = a0.template slc<width_a>(0);
	ac_int<width_a,sign_a> ia1 = a1.template slc<width_a>(0);
	
	ac_int<width_b,sign_b> ib0 = b0.template slc<width_b>(0);
	ac_int<width_b,sign_b> ib1 = b1.template slc<width_b>(0);

	ac_int<width_a+width_b+8,sign_a||sign_b> acc;
    ac_fixed<width_a+width_b+8, iwidth_a+iwidth_b+8, sign_a || sign_b> accf;
	
	acc = Alt_2mult_sub_accum <opID,use_extra_input_reg,use_mult_reg,output_register>(ia0,ib0,ia1,ib1,accum_sload);
    accf.set_slc(0,acc);
	return accf;
}

#endif
