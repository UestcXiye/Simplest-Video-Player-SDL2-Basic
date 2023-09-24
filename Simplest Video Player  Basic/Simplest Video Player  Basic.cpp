// Simplest Video Player  Basic.cpp : �������̨Ӧ�ó������ڵ㡣
//

/**
* ��򵥵�SDL2������Ƶ�����ӣ�SDL2����RGB/YUV��
* Simplest Video Play SDL2 (SDL2 play RGB/YUV)
*
* ԭ����
* ������ Lei Xiaohua
* leixiaohua1020@126.com
* �й���ý��ѧ/���ֵ��Ӽ���
* Communication University of China / Digital TV Technology
* http://blog.csdn.net/leixiaohua1020
*
* �޸ģ�
* ���ĳ� Liu Wenchen
* 812288728@qq.com
* ���ӿƼ���ѧ/������Ϣ
* University of Electronic Science and Technology of China / Electronic and Information Science
* https://blog.csdn.net/ProgramNovice
* 
* ������ʹ��SDL2����RGB/YUV��Ƶ�������ݡ�
* SDLʵ�����ǶԵײ��ͼ API��Direct3D��OpenGL���ķ�װ��ʹ���������Լ���ֱ�ӵ��õײ�API��
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

// ����
// LNK2019 �޷��������ⲿ���� __imp__fprintf���÷����ں��� _ShowError �б�����
// LNK2019 �޷��������ⲿ���� __imp____iob_func���÷����ں��� _ShowError �б�����

// ����취��
// ������ı������汾���ڵ�ǰ����汾����Ҫ��������Դ����vs2017���±��룬����û�а������Դ�룬��·��ͨ��
// Ȼ��鵽˵��stdin, stderr, stdout �⼸������vs2015����ǰ�Ķ���ò�һ�������Ա���
// ��������أ�����ʹ��{ *stdin,*stdout,*stderr }�����Լ�����__iob_func()
#pragma comment(lib,"legacy_stdio_definitions.lib")
extern "C"
{
	FILE __iob_func[3] = { *stdin,*stdout,*stderr };
}

const int bpp = 12;

int screen_w = 640, screen_h = 360;// ���ڴ�С�̶�Ϊ640*360
const int pixel_w = 640, pixel_h = 360;

unsigned char buffer[pixel_w*pixel_h*bpp / 8];

int main(int argc, char* argv[])
{
	// ��ʼ��SDLϵͳ
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		printf("Could not initialize SDL - %s\n", SDL_GetError());
		return -1;
	}

	SDL_Window *screen;
	// SDL 2.0 Support for multiple windows
	// ��������SDL_Window
	screen = SDL_CreateWindow("Simplest Video Play SDL2 Basic", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		screen_w, screen_h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!screen)
	{
		printf("SDL: Couldn't create window - exiting:%s\n", SDL_GetError());
		return -1;
	}

	// ������Ⱦ��SDL_Renderer
	SDL_Renderer* sdlRenderer = SDL_CreateRenderer(screen, -1, 0);

	Uint32 pixformat = 0;
	// IYUV: Y + U + V  (3 planes)
	// YV12: Y + V + U  (3 planes)
	pixformat = SDL_PIXELFORMAT_IYUV;

	// ��������SDL_Texture
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

		// �������������
		SDL_UpdateTexture(sdlTexture, NULL, buffer, pixel_w);

		// sdlRect.x��sdlRect.yΪ�������Ͻǵ�����
		sdlRect.x = 0;
		sdlRect.y = 0;
		// sdlRect.w��sdlRect.hΪ����Ŀ�Ⱥ͸߶�
		sdlRect.w = screen_w;
		sdlRect.h = screen_h;

		// ʵ����Ƶ��Χ��Χ10���صġ��ڿ�
		// sdlRect.x = 10;
		// sdlRect.y = 10;
		// sdlRect.w = screen_w - 20;
		// sdlRect.h = screen_h - 20;

		// ʹ��ͼ����ɫ�����ǰ����ȾĿ��
		SDL_RenderClear(sdlRenderer);
		// ����������ݿ�������Ⱦ��
		SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &sdlRect);
		// ��ʾ���������
		SDL_RenderPresent(sdlRenderer);
		// ���������ٶȣ���ʱ40ms
		SDL_Delay(40);
		// 2�������ٶȣ�SDL_Delay(20);

	}

	// �˳�SDLϵͳ
	SDL_Quit();

	return 0;
}

