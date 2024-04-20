#include <common.h>

int gen, baryon, tachyon;
int (*_sceSysconGetBaryonVersion)(int *) = NULL;
int (*_sceSysregGetTachyonVersion)(void) = NULL;
int (*_sceKernelGetModel)(void) = NULL;
int (*_sceKernelDelayThread)(int) = NULL;
int (*_sceKernelExitDeleteThread)(int) = NULL;

int _sceGetModelGen(void)
{
	for (u32 addr = 0x88000000; addr < 0x88400000; addr += 4)
		if (_lw(addr) == 0x3C03BC10)
			// i just dont understand whats going on in here..
			return pspXploitFindFirstJALReverse(addr);
	return 0;
}

int kexploitKernelGetModel(void)
{
	int k1 = pspSdkSetK1(0);
	int g = _sceKernelGetModel();
	pspSdkSetK1(k1);
	return g;
}

int kexploitSysregGetTachyonVersion(void)
{
	int k1 = pspSdkSetK1(0);
	int sv = _sceSysregGetTachyonVersion();
	pspSdkSetK1(k1);
	return sv;
}

void ithread(void)
{
	_sceSysconGetBaryonVersion(&baryon);
	tachyon = kexploitSysregGetTachyonVersion();
	gen = kexploitKernelGetModel() + 1;

	switch (tachyon)
	{
	case 0x00600000:
		switch (baryon)
		{
		case 0x00243000:
			BackColor(RED);
			TextColor(BLACK);
			printf("ta-088v3! CANNOT install Classic cIPL");
			break;
		default:
			break;
		}
		break;

	default:
		BackColor(GREEN);
		TextColor(BLACK);
		printf("NOT ta-088v3!\n");
		if (gen == 1 || gen == 2)
			printf("01g/02g! can install Classic cIPL");
		else
		{
			BackColor(RED);
			printf("NOT 01g/02g! CANNOT install Classic cIPL");
		}
		break;
	}

	BackColor(BLACK);
	TextColor(WHITE);
	printf("\n\nGeneration : %02ig\n", gen);
	printf("Baryon : 0x%08x\n", baryon);
	printf("Tachyon : 0x%08x\n\n", tachyon);
	printf("Press any button to exit.");
}

void kthread(void)
{
	_sceKernelGetModel = _sceGetModelGen();
	_sceSysconGetBaryonVersion = pspXploitFindFunction("sceSYSCON_Driver", "sceSyscon_driver", 0x7EC5A957);
	_sceSysregGetTachyonVersion = pspXploitFindFunction("sceLowIO_Driver", "sceSysreg_driver", 0xE2A5D1EE);
	_sceKernelDelayThread = pspXploitFindFunction("sceThreadManager", "ThreadManForUser", 0xCEADEB47);
	_sceKernelExitDeleteThread = pspXploitFindFunction("sceThreadManager", "ThreadManForUser", 0x809CE29B);

	SceUID kthreadID = k_tbl->KernelCreateThread("iypcwc_kthread", (void *)KERNELIFY(&ithread), 1, 0x20000, PSP_THREAD_ATTR_VFPU, NULL);
	if (kthreadID >= 0)
	{
		k_tbl->KernelStartThread(kthreadID, 0, NULL);
		k_tbl->waitThreadEnd(kthreadID, NULL);
	}
}