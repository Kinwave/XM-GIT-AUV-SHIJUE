#include "main.h"

#ifdef _if_run_in_pi
	#ifndef pi_usart_h
	#define pi_usart_h
		#include <wiringPi.h>
		#include <wiringSerial.h>

		void usart_pthread_start(int boud);
		void usart_send(int send);

		extern double stm32_depth;
		extern double stm32_angle;
		extern double stm32_angle_relative;
		extern double stm32_angle_error;
	#endif
#endif
