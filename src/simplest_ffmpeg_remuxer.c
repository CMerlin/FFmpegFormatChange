/**
 * 最简单的基于FFmpeg的封装格式转换器
 * Simplest FFmpeg Remuxer
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序实现了视频封装格式之间的转换。
 * 需要注意的是本程序并不改变视音频的编码格式。
 * 本程序修改自FFmpeg的tutorial。
 *
 * This software converts a media file from one container format
 * to another container format without encoding/decoding video files.
 * This software is modified from FFmpeg's tutorial.
 */

#include <stdio.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavfilter/avfilter.h>


#if 0

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavformat/avformat.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavformat/avformat.h>
#ifdef __cplusplus
};
#endif
#endif
#endif


/*merlin mp4*/
int main(int argc, char* argv[])
{
	printf("\e[32m[%s]: line:%d\e[0m\n", __func__, __LINE__);
#if 1
	AVOutputFormat *ofmt = NULL;
	//Input AVFormatContext and Output AVFormatContext
	AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL;
	AVPacket pkt;
	const char *in_filename, *out_filename;
	int ret, i;
	int frame_index=0;

	/*输入输出文件名*/
	in_filename  = "./cuc_ieschool1.flv";//Input file URL
	out_filename = "./cuc_ieschool1.mp4";//Output file URL

	printf("[%s]: line:%d\n", __func__, __LINE__);
	av_register_all();
	//Input
	if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) /*打开输入文件，初始化输入视频码流的ifmt_ctx*/
	{
		printf( "[%s][Error]:Could not open input file. line:%d\n", __func__, __LINE__);
		goto end;
	}
	if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
		printf( "[%s]:Failed to retrieve input stream information line:%d\n", __func__, __LINE__);
		goto end;
	}
	av_dump_format(ifmt_ctx, 0, in_filename, 0);
	//Output
	avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename); /*初始化输出视频码流的AVFormatContext*/
	if (!ofmt_ctx) {
		printf( "[%s]:Could not create output context line:%d\n", __func__, __LINE__);
		ret = AVERROR_UNKNOWN;
		goto end;
	}
	ofmt = ofmt_ctx->oformat;
	for (i = 0; i < ifmt_ctx->nb_streams; i++) {
		//Create output AVStream according to input AVStream
		AVStream *in_stream = ifmt_ctx->streams[i];
		AVStream *out_stream = NULL;
#if 0
		AVStream *out_stream = avformat_new_stream(ofmt_ctx, (in_stream->codec->codec)); /*创建输出码流的AVStream*/
		if (!out_stream) {
			printf("[%s]:Failed allocating output stream line:%d\n", __func__, __LINE__);
			ret = AVERROR_UNKNOWN;
			goto end;
		}
#endif
#if 0
		//Copy the settings of AVCodecContext
		if (avcodec_copy_context(out_stream->codec, in_stream->codec) < 0) /*拷贝输入视频码流的AVCodecContex的数值t到输出视频的AVCodecContext*/
		{
			printf("[%s]:Failed to copy context from input to output stream codec context line:%d\n", __func__, __LINE__);
			goto end;
		}
#endif
#if 0
		out_stream->codec->codec_tag = 0;
		if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
			out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
#endif
	}
	//Output information------------------
	av_dump_format(ofmt_ctx, 0, out_filename, 1);
	//Open output file
	if (!(ofmt->flags & AVFMT_NOFILE)) {
		ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE); /*打开输出文件*/
		if (ret < 0) {
			printf( "[%s]:Could not open output file '%s' line:%d\n", __func__, out_filename, __LINE__);
			goto end;
		}
	}
	//Write file header
	if (avformat_write_header(ofmt_ctx, NULL) < 0) /*写文件头（对于某些没有文件头的封装格式，不需要此函数。比如说MPEG2TS）*/
	{
		printf("[%s]:Error occurred when opening output file line:%d\n", __func__, __LINE__);
		goto end;
	}

	while (1) {
		AVStream *in_stream, *out_stream;
		//Get an AVPacket
		ret = av_read_frame(ifmt_ctx, &pkt); /*从输入文件中读取一个AVPacket*/
		if (ret < 0)
			break;
		in_stream  = ifmt_ctx->streams[pkt.stream_index];
		out_stream = ofmt_ctx->streams[pkt.stream_index];
#if 0
		//Convert PTS/DTS
		pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
		pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
#endif
#if 1
				//Convert PTS/DTS
				pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
				pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
#endif

		pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
		pkt.pos = -1;
		//Write
		if (av_interleaved_write_frame(ofmt_ctx, &pkt) < 0) /*将AVPacket（存储视频压缩码流数据）写入文件*/
		{
			printf("[%s]:Error muxing packet line:%d\n", __func__,  __LINE__);
			break;
		}
		printf("[%s]:Write %8d frames to output file line:%d\n", __func__, frame_index, __LINE__);
		//av_free_packet(&pkt);
		frame_index++;
	}
	//Write file trailer
	av_write_trailer(ofmt_ctx); /*写文件尾*/
end:
	printf("[%s]:deal with end! line:%d\n", __func__, __LINE__);
	avformat_close_input(&ifmt_ctx);
	/* close output */
	if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
		avio_close(ofmt_ctx->pb);
	avformat_free_context(ofmt_ctx);

	return 0;
#endif
}

