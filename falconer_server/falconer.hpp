#ifndef FALCONER_H_
#define FALCONER_H_

#include "msl/socket.hpp"
#include <string>

extern "C"
{
	typedef unsigned long UINT64_C;
	#include "libavcodec/avcodec.h"
	#include "libswscale/swscale.h"
}

class ardrone
{
	public:
		ardrone(const std::string ip="192.168.1.1");
		~ardrone();
		operator bool() const;
		bool connect(unsigned int time_out=1);
		void navdata_update();
		void video_update();
		void land();
		void emergency_mode_toggle();
		void takeoff();
		void manuever(const float altitude,const float pitch,const float roll,const float yaw);
		void hover();
		void set_video_feed_front();
		void set_video_feed_bottom();
		unsigned int battery_percent() const;
		bool flying() const;
		bool emergency_mode() const;
		bool low_battery() const;
		bool ultrasonic_enabled() const;
		bool motors_good() const;
		float pitch() const;
		float roll() const;
		float yaw() const;
		int altitude() const;
		uint8_t* video_data() const;

	private:
		unsigned int _count;
		msl::socket _control_socket;
		msl::socket _navdata_socket;
		msl::socket _video_socket;
		unsigned int _battery_percent;
		bool _landed;
		bool _emergency_mode;
		bool _low_battery;
		bool _ultrasonic_enabled;
		bool _video_enabled;
		bool _motors_good;
		int _altitude;
		float _pitch;
		float _roll;
		float _yaw;
		bool _found_codec;
		uint8_t* _camera_data;
		AVPacket _av_packet;
		AVCodec* _av_codec;
		AVCodecContext* _av_context;
		AVFrame* _av_camera_cmyk;
		AVFrame* _av_camera_rgb;
};

#endif // FALCONER_H_
