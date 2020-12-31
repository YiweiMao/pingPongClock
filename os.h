/*
 * File:   os.hpp
 * Author: TristanH
 *
 * Created 8th March 2019
*/

#ifndef OS_H
#define	OS_H

typedef unsigned long ostime_t;

void initOS();

void run_later(void (*)(), ostime_t);

void run();

void dream(short,short,short);

void initBlink();

#endif	/* OS_H */
