################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:\Users\David\eclipse\php-neon\eclipse\arduinoPlugin\libraries\Adafruit_NeoPixel\1.0.6\Adafruit_NeoPixel.cpp 

C_SRCS += \
C:\Users\David\eclipse\php-neon\eclipse\arduinoPlugin\libraries\Adafruit_NeoPixel\1.0.6\esp8266.c 

C_DEPS += \
.\libraries\Adafruit_NeoPixel\esp8266.c.d 

LINK_OBJ += \
.\libraries\Adafruit_NeoPixel\Adafruit_NeoPixel.cpp.o \
.\libraries\Adafruit_NeoPixel\esp8266.c.o 

CPP_DEPS += \
.\libraries\Adafruit_NeoPixel\Adafruit_NeoPixel.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries\Adafruit_NeoPixel\Adafruit_NeoPixel.cpp.o: C:\Users\David\eclipse\php-neon\eclipse\arduinoPlugin\libraries\Adafruit_NeoPixel\1.0.6\Adafruit_NeoPixel.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\David\eclipse\php-neon\eclipse\arduinoPlugin\tools\arduino\avr-gcc\4.9.2-atmel3.5.3-arduino2/bin/avr-g++" -c -g -Os -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10609 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR   -I"C:\Users\David\eclipse\php-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\cores\arduino" -I"C:\Users\David\eclipse\php-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\variants\standard" -I"C:\Users\David\eclipse\php-neon\eclipse\arduinoPlugin\libraries\Adafruit_NeoPixel\1.0.6" -I"C:\Users\David\eclipse\php-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\Wire" -I"C:\Users\David\eclipse\php-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\Wire\src" -I"C:\Users\David\eclipse\php-neon\eclipse\arduinoPlugin\libraries\RTClib\1.2.0" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<" -o "$@"  -Wall
	@echo 'Finished building: $<'
	@echo ' '

libraries\Adafruit_NeoPixel\esp8266.c.o: C:\Users\David\eclipse\php-neon\eclipse\arduinoPlugin\libraries\Adafruit_NeoPixel\1.0.6\esp8266.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:\Users\David\eclipse\php-neon\eclipse\arduinoPlugin\tools\arduino\avr-gcc\4.9.2-atmel3.5.3-arduino2/bin/avr-gcc" -c -g -Os -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10609 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR   -I"C:\Users\David\eclipse\php-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\cores\arduino" -I"C:\Users\David\eclipse\php-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\variants\standard" -I"C:\Users\David\eclipse\php-neon\eclipse\arduinoPlugin\libraries\Adafruit_NeoPixel\1.0.6" -I"C:\Users\David\eclipse\php-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\Wire" -I"C:\Users\David\eclipse\php-neon\eclipse\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\libraries\Wire\src" -I"C:\Users\David\eclipse\php-neon\eclipse\arduinoPlugin\libraries\RTClib\1.2.0" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<" -o "$@"  -Wall
	@echo 'Finished building: $<'
	@echo ' '


