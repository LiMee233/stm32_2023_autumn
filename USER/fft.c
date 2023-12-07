#include "stm32f10x.h"
#include "stm32_dsp.h"
#include "math.h"
#include "def.h"
#include "timer.h"

int long fftout[NPT]; // FFT 输出
int long fftin[NPT]; // FFT 输入
uint32_t FFT_Mag[NPT/2] = {0}; // 幅频特性
uint32_t fre = 72000000 ; // 采样频率 kHz
uint16_t temp = 0; // 幅值最大的频率成分

extern uint16_t Frequency; // 波形频率

/******************************************************************
函数名称：GetPowerMag()
函数功能：计算各次谐波幅值
参数说明：
备　　注：先将 lBufOutArray 分解成实部 (X) 和虚部 (Y)，然后计算幅值 (sqrt(X*X + Y*Y)
*******************************************************************/
void GetPowerMag(void)
{
	float X, Y, Mag, magmax; // 实部，虚部，各频率幅值，最大幅值
	uint16_t i;

	// 调用自 cr4_fft_1024_stm32
	cr4_fft_1024_stm32(fftout, fftin, NPT);

	for(i=1; i<NPT/2; i++)
	{
		X = (fftout[i] << 16) >> 16;	//低 16 位存实部
		Y = (fftout[i] >> 16);			//高 16 位存虚部

		Mag = sqrt(X * X + Y * Y); //计算模值
		FFT_Mag[i] = Mag; //存入缓存，用于输出查验
		
		// 获取最大频率分量及其幅值
		if(Mag > magmax)
		{
			magmax = Mag;
			temp = i;
		}
	}
	Frequency = (uint16_t)(temp*(fre*1.0/NPT));	// FFT 所得实际频率 f = 点数 i * (采样频率 F / 总采样点数 N)
}
