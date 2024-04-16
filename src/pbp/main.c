#include <stdio.h>
#include <pspkernel.h>
#include <libpspexploit.h>
#include <common.h>

#define VER_MAJOR 1
#define VER_MINOR 0
#define VER_BUILD " final"

PSP_MODULE_INFO("iypcwc", 0, VER_MAJOR, VER_MINOR);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER|PSP_THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(4*1024);

extern void kmain();

int main(int argc, char*argv[]) {
	pspDebugScreenInit();
	pspDebugScreenClear();
	printf("\niypcwc v%i.%i%s - github.com/catmagicspell/iypcwc\n\n", VER_MAJOR, VER_MINOR, VER_BUILD);

	int ret;
	ret = pspXploitInitKernelExploit();

	if (ret == 0) {
		ret = pspXploitDoKernelExploit();

		if (ret == 0) {
			pspXploitExecuteKernel(kmain);
			sceDisplayWaitVblank();
		}

		else {
			TextColor(YELLOW);
			printf("\n Error: pspXploitDoKernelExploit = 0x%08x\n", ret);
			printf(" Exiting in 5 seconds...\n");
			TextColor(WHITE);
			sceKernelDelayThread(5*1000*1000);
		}
	}

	else {
		TextColor(YELLOW);
		printf("\n Error: pspXploitInitKernelExploit = 0x%08x\n", ret);
		printf(" Exiting in 5 seconds...\n");
		TextColor(WHITE);
		sceKernelDelayThread(5*1000*1000);
	}
		
	sceKernelExitGame();
	return 0;
}
