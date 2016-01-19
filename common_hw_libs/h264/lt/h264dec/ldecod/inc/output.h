
/*!
 **************************************************************************************
 * \file
 *    output.h
 * \brief
 *    Picture writing routine headers
 * \author
 *    Main contributors (see contributors.h for copyright, address and affiliation details) 
 *      - Karsten Suehring        <suehring@hhi.de>
 ***************************************************************************************
 */

void write_stored_frame(FrameStore *fs, FILE *p_out);
void direct_output(StorablePicture *p, FILE *p_out);
void init_out_buffer();
void uninit_out_buffer();
void write_stored_frame(FrameStore *fs, FILE *p_out);
void write_picture_tlm(StorablePicture *p, FILE *p_out);
void write_picture_ppm(StorablePicture *p, FILE *p_out);
