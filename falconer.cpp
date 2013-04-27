#include "falconer.hpp"

#include "msl/string_util.hpp"
#include <time.h>

//https://github.com/elliotwoods/ARDrone-GStreamer-test/blob/master/plugin/src/pave.h
struct parrot_video_encapsulation_t
{
	uint8_t signature[4];
	uint8_t version;
	uint8_t video_codec;
	uint16_t header_size;
	uint32_t payload_size;					/* Amount of data following this PaVE */
	uint16_t encoded_stream_width;			/* ex: 640 */
	uint16_t encoded_stream_height;			/* ex: 368 */
	uint16_t display_width;					/* ex: 640 */
	uint16_t display_height;				/* ex: 360 */
	uint32_t frame_number;					/* frame position inside the current stream */
	uint32_t timestamp;						/* in milliseconds */
	uint8_t total_chuncks;					/* number of UDP packets containing the current decodable payload */
	uint8_t chunck_index ;					/* position of the packet - first chunk is #0 */
	uint8_t frame_type;						/* I-frame, P-frame */
	uint8_t control;						/* Special commands like end-of-stream or advertised frames */
	uint32_t stream_byte_position_lw;		/* Byte position of the current payload in the encoded stream - lower 32-bit word */
	uint32_t stream_byte_position_uw;		/* Byte position of the current payload in the encoded stream - upper 32-bit word */
	uint16_t stream_id;						/* This ID indentifies packets that should be recorded together */
	uint8_t total_slices;					/* number of slices composing the current frame */
	uint8_t slice_index ;					/* position of the current slice in the frame */
	uint8_t header1_size;					/* H.264 only : size of SPS inside payload - no SPS present if value is zero */
	uint8_t header2_size;					/* H.264 only : size of PPS inside payload - no PPS present if value is zero */
	uint8_t reserved2[2];					/* Padding to align on 48 bytes */
	uint32_t advertised_size;				/* Size of frames announced as advertised frames */
	uint8_t reserved3[12];					/* Padding to align on 64 bytes */
};

ardrone::ardrone(const std::string ip):_count(1),_control_socket(ip+":5556"),_navdata_socket(ip+":5554"),_video_socket(ip+":5555"),
	_battery_percent(2000),_landed(true),_emergency_mode(false),_low_battery(false),_ultrasonic_enabled(false),_video_enabled(false),
	_motors_good(false),_pitch(0),_roll(0),_yaw(0),_altitude(0),_found_codec(true)
{
	av_log_set_level(AV_LOG_QUIET);

	_camera_data=new uint8_t[640*368*3];

	avcodec_register_all();

	av_init_packet(&_av_packet);
	_av_packet.data=new uint8_t[100000];

	_av_codec=avcodec_find_decoder(CODEC_ID_H264);

	if(!_av_codec)
		_found_codec=false;

	if(_found_codec)
	{
		_av_context=avcodec_alloc_context3(_av_codec);
		_av_camera_cmyk=avcodec_alloc_frame();
		_av_camera_rgb=avcodec_alloc_frame();

		if(avcodec_open2(_av_context,_av_codec,NULL)<0)
			_found_codec=false;
	}
}

ardrone::~ardrone()
{
	_control_socket.close();
	_navdata_socket.close();
	_video_socket.close();
	delete[] _camera_data;
	avcodec_close(_av_context);
	av_free(_av_context);
	av_free(_av_camera_cmyk);
	av_free(_av_camera_rgb);
	delete[] _av_packet.data;
}

ardrone::operator bool() const
{
	return (_control_socket&&_navdata_socket&&_video_socket&&_found_codec);
}

bool ardrone::connect(unsigned int time_out)
{
	bool connected=false;

	if(!*this)
	{
		_control_socket.connect_udp();
		_navdata_socket.connect_udp();
		_video_socket.connect_tcp();
	}

	if(*this)
	{
		_count=1;

		std::string initialize_command="AT*FTRIM="+msl::to_string(_count)+"\r";
		++_count;
		_control_socket<<initialize_command;

		std::string outdoor_hull_command="AT*CONFIG="+msl::to_string(_count)+",\"control:outdoor\",\"FALSE\"\r";
		++_count;
		_control_socket<<outdoor_hull_command;

		std::string shell_is_on_command="AT*CONFIG="+msl::to_string(_count)+",\"control:flight_without_shell\",\"TRUE\"\r";
		++_count;
		_control_socket<<shell_is_on_command;

		std::string motor_type_command="AT*CONFIG="+msl::to_string(_count)+",\"control:brushless\",\"TRUE\"\r";
		++_count;
		_control_socket<<motor_type_command;

		std::string navdata_enable_command="AT*CONFIG="+msl::to_string(_count)+",\"general:navdata_demo\",\"FALSE\"\r";
		++_count;
		_control_socket<<navdata_enable_command;

		std::string navdata_send_all_command="AT*CONFIG="+msl::to_string(_count)+",\"general:navdata_options\",\"65537\"\r";
		++_count;
		_control_socket<<navdata_send_all_command;

		std::string watchdog_command="AT*COMWDG="+msl::to_string(_count)+"\r";
		++_count;
		_control_socket<<watchdog_command;

		std::string altitude_min_command="AT*CONFIG="+msl::to_string(_count)+",\"control:altitude_min\",\"10\"\r";
		++_count;
		_control_socket<<altitude_min_command;

		std::string altitude_max_command="AT*CONFIG="+msl::to_string(_count)+",\"control:altitude_max\",\"4000\"\r";
		++_count;
		_control_socket<<altitude_max_command;

		unsigned int time_start=time(0);
		uint8_t redirect_navdata_command[14]={1,0,0,0,0,0,0,0,0,0,0,0,0,0};
		uint8_t video_wakeup_command[1]={1};

		do
		{
			_navdata_socket.write(redirect_navdata_command,14);
			_video_socket.write(video_wakeup_command,1);
		}
		while(time(0)-time_start<time_out&&(_navdata_socket.check()<=0||_video_socket.check()<=0));

		if(_navdata_socket.check()>0&&_video_socket.check()>0)
			connected=true;
	}

	return (connected&&*this);
}

void ardrone::navdata_update()
{
	if(*this)
	{
		const int packet_size=548;			//nav-data-full packet size=548, nav-data-demo packet size=24
		uint8_t byte[packet_size];

		if(_navdata_socket.check()>0&&_navdata_socket.read(byte,packet_size)==packet_size)
		{
			if(byte[0]==0x88&&byte[1]==0x77&&byte[2]==0x66&&byte[3]==0x55)
			{
				unsigned int states=byte[7]<<24|byte[6]<<16|byte[5]<<8|byte[4]<<0;
				_landed=!static_cast<bool>(states&(1<<0));
				_emergency_mode=static_cast<bool>(states&(1<<31));
				_low_battery=static_cast<bool>(states&(1<<15));
				_ultrasonic_enabled=!static_cast<bool>(states&(1<<21));
				_video_enabled=static_cast<bool>(states&(1<<1));
				_motors_good=!static_cast<bool>(states&(1<<12));

				int header=byte[17]<<8|byte[16]<<0;

				if(header==0)
				{
					for(unsigned char ii=0;ii<4;++ii)
					{
						reinterpret_cast<uint8_t*>(&_battery_percent)[ii]=byte[24+ii];
						reinterpret_cast<uint8_t*>(&_pitch)[ii]=byte[28+ii];
						reinterpret_cast<uint8_t*>(&_roll)[ii]=byte[32+ii];
						reinterpret_cast<uint8_t*>(&_yaw)[ii]=byte[36+ii];
						reinterpret_cast<uint8_t*>(&_altitude)[ii]=byte[40+ii];
					}
				}
			}
		}
	}
}

void ardrone::video_update()
{
	if(*this)
	{
		char video_keepalive_command[1]={1};
		_video_socket<<video_keepalive_command;

		parrot_video_encapsulation_t video_packet;
		_av_packet.size=_video_socket.read(_av_packet.data,sizeof(parrot_video_encapsulation_t),MSG_WAITALL);
		memcpy(&video_packet,_av_packet.data,_av_packet.size);
		_av_packet.size=_video_socket.read(_av_packet.data,video_packet.payload_size,MSG_WAITALL);

		_av_packet.flags=0;

		if(video_packet.frame_type==1)
			_av_packet.flags=AV_PKT_FLAG_KEY;

		int frame_decoded=0;

		if(avcodec_decode_video2(_av_context,_av_camera_cmyk,&frame_decoded,&_av_packet)>0&&frame_decoded>0)
		{
			SwsContext* _sws_context=sws_getContext(video_packet.encoded_stream_width,video_packet.encoded_stream_height,PIX_FMT_YUV420P,video_packet.encoded_stream_width,
				video_packet.encoded_stream_height,PIX_FMT_RGB24,SWS_BICUBIC,NULL,NULL,NULL);
			avpicture_fill(reinterpret_cast<AVPicture*>(_av_camera_rgb),_camera_data,PIX_FMT_BGR24,video_packet.encoded_stream_width,video_packet.encoded_stream_height);
			sws_scale(_sws_context,_av_camera_cmyk->data,_av_camera_cmyk->linesize,0,video_packet.display_height,_av_camera_rgb->data,_av_camera_rgb->linesize);
			sws_freeContext(_sws_context);
		}
	}
}

void ardrone::land()
{
	if(*this)
	{
		int land_flags=1<<18|1<<20|1<<22|1<<24|1<<28;
		std::string command="AT*REF="+msl::to_string(_count)+","+msl::to_string(land_flags)+"\r";
		++_count;
		_control_socket<<command;
	}
}

void ardrone::emergency_mode_toggle()
{
	if(*this)
	{
		int emergency_flags=1<<8|1<<18|1<<20|1<<22|1<<24|1<<28;
		std::string command="AT*REF="+msl::to_string(_count)+","+msl::to_string(emergency_flags)+"\r";
		++_count;
		_control_socket<<command;
	}
}

void ardrone::takeoff()
{
	if(*this)
	{
		int takeoff_flags=1<<9|1<<18|1<<20|1<<22|1<<24|1<<28;
		std::string command="AT*REF="+msl::to_string(_count)+","+msl::to_string(takeoff_flags)+"\r";
		++_count;
		_control_socket<<command;
	}
}

void ardrone::manuever(const float altitude,const float pitch,const float roll,const float yaw)
{
	if(*this)
	{
		bool hover=false;
		std::string command="AT*PCMD="+msl::to_string(_count)+",1,"+msl::to_string(*(int*)(&roll))+","+msl::to_string(*(int*)(&pitch))
			+","+msl::to_string(*(int*)(&altitude))+","+msl::to_string(*(int*)(&yaw))+"\r";
		++_count;
		_control_socket<<command;
	}
}

void ardrone::hover()
{
	if(*this)
	{
		std::string command="AT*PCMD="+msl::to_string(_count)+",0,0,0,0,0\r";
		++_count;
		_control_socket<<command;
	}
}

void ardrone::set_video_feed_front()
{
	if(*this)
	{
		std::string command="AT*CONFIG="+msl::to_string(_count)+",\"video:video_channel\",\"2\"\r";
		++_count;
		_control_socket<<command;
	}
}

void ardrone::set_video_feed_bottom()
{
	if(*this)
	{
		std::string command="AT*CONFIG="+msl::to_string(_count)+",\"video:video_channel\",\"3\"\r";
		++_count;
		_control_socket<<command;
	}
}

unsigned int ardrone::battery_percent() const
{
	return _battery_percent;
}

bool ardrone::flying() const
{
	return !_landed;
}

bool ardrone::emergency_mode() const
{
	return _emergency_mode;
}

bool ardrone::low_battery() const
{
	return _low_battery;
}

bool ardrone::ultrasonic_enabled() const
{
	return _ultrasonic_enabled;
}

bool ardrone::motors_good() const
{
	return _motors_good;
}

int ardrone::altitude() const
{
	return _altitude;
}

float ardrone::pitch() const
{
	return _pitch;
}

float ardrone::roll() const
{
	return _roll;
}

float ardrone::yaw() const
{
	return _yaw;
}

uint8_t* ardrone::video_data() const
{
	return _camera_data;
}
