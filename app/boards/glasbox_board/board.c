#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
LOG_MODULE_REGISTER(glasbox_board, LOG_LEVEL_INF);

static int board_glasbox_board_init(void){
    //printk("testing \n");
    LOG_INF("board initialized!");
    return 0;
}

SYS_INIT(board_glasbox_board_init, POST_KERNEL, 99);