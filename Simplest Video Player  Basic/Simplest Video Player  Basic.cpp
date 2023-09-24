// Simplest Video Player  Basic.cpp : 定义控制台应用程序的入口点。
//

/**
* 最简单的SDL2播放视频的例子（SDL2播放RGB/YUV）
* Simplest Video Play SDL2 (SDL2 play RGB/YUV)
*
* 原程序：
* 雷霄骅 Lei Xiaohua
* leixiaohua1020@126.com
* 中国传媒大学/数字电视技术
* Communication University of China / Digital TV Technology
* http://blog.csdn.net/leixiaohua1020
*
* 修改：
* 刘文晨 Liu Wenchen
* 812288728@qq.com
* 电子科技大学/电子信息
* University of Electronic Science and Technology of China / Electronic and Information Science
* https://blog.csdn.net/ProgramNovice
* 
* 本程序使用SDL2播放RGB/YUV视频像素数据。
* SDL实际上是对底层绘图 API（Direct3D，OpenGL）的封装，使用起来明显简单于直接调用底层API。
*
* This software plays RGB/YUV raw video data using SDL2.
* SDL is a wrapper of low-level API (Direct3D, OpenGL).
* Use SDL is much easier than directly call these low-level API.
*/

#include "stdafx.h"

#include <stdio.h>

extern "C"
{
#include "sdl/SDL.h"
};

// 报错：
// LNK2019 无法解析的外部符号 __imp__fprintf，该符号在函数 _ShowError 中被引用
// LNK2019 无法解析的外部符号 __imp____iob_func，该符号在函数 _ShowError 中被引用

// 解决办法：
// 包含库的编译器版本低于当前编译版本，需要将包含库源码用vs2017重新编译，由于没有包含库的源码，此路不通。
// 然后查到说是stdin, stderr, stdout 这几个函数vs2015和以前的定义得不一样，所以报错。
// 解决方法呢，就是使用{ *stdin,*stdout,*stderr }数组自己定义__iob_func()
#pragma comment(lib,"legacy_stdio_definitions.lib")
extern "C"
{
	FILE __iob_func[3] = { *stdin,*stdout,*stderr };
}

const int bpp = 12;

int screen_w = 640, screen_h = 360;// 窗口大小固定为640*360
const int pixel_w = 640, pixel_h = 360;

unsigned char buffer[pixel_w*pixel_h*bpp / 8];

int main(int argc, char* argv[])
{
	// 初始化SDL系统
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		printf("Could not initialize SDL - %s\n", SDL_GetError());
		return -1;
	}

	SDL_Window *screen;
	// SDL 2.0 Support for multiple windows
	// 创建窗口SDL_Window
	screen = SDL_CreateWindow("Simplest Video Play SDL2 Basic", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		screen_w, screen_h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!screen)
	{
		printf("SDL: Couldn't create window - exiting:%s\n", SDL_GetError());
		return -1;
	}

	// 创建渲染器SDL_Renderer
	SDL_Renderer* sdlRenderer = SDL_CreateRenderer(screen, -1, 0);

	Uint32 pixformat = 0;
	// IYUV: Y + U + V  (3 planes)
	// YV12: Y + V + U  (3 planes)
	pixformat = SDL_PIXELFORMAT_IYUV;

	// 创建纹理SDL_Texture
	SDL_Texture* sdlTexture = SDL_CreateTexture(sdlRenderer, pixformat, SDL_TEXTUREACCESS_STREAMING, pixel_w, pixel_h);

	FILE *fp = NULL;
	fp = fopen("sintel_640_360.yuv", "rb+");

	if (fp == NULL)
	{
		printf("Can't open this file\n");
		return -1;
	}

	SDL_Rect sdlRect;

	while (1)
	{
		if (fread(buffer, 1, pixel_w*pixel_h*bpp / 8, fp) != pixel_w*pixel_h*bpp / 8)
		{
			// Loop
			fseek(fp, 0, SEEK_SET);
			fread(buffer, 1, pixel_w*pixel_h*bpp / 8, fp);
		}

		// 设置纹理的数据
		SDL_UpdateTexture(sdlTexture, NULL, buffer, pixel_w);

		// sdlRect.x和sdlRect.y为画面左上角的坐标
		sdlRect.x = 0;
		sdlRect.y = 0;
		// sdlRect.w和sdlRect.h为画面的宽度和高度
		sdlRect.w = screen_w;
		sdlRect.h = screen_h;

		// 实现视频周围包围10像素的“黑框”
		// sdlRect.x = 10;
		// sdlRect.y = 10;
		// sdlRect.w = screen_w - 20;
		// sdlRect.h = screen_h - 20;

		// 使用图形颜色清除当前的渲染目标
		SDL_RenderClear(sdlRenderer);
		// 将纹理的数据拷贝给渲染器
		SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &sdlRect);
		// 显示纹理的数据
		SDL_RenderPresent(sdlRenderer);
		// 正常播放速度：延时40ms
		SDL_Delay(40);
		// 2倍播放速度：SDL_Delay(20);

	}

	// 退出SDL系统
	SDL_Quit();

	return 0;
}

