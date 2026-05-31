#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include "cxxlog.h"
#include "sql/baseSqlite3.h"
#include "decode/beastSound.h"
#include "decode/finallyRecovery.h"
#include "printf/myPrintf.h"

void ExitRoutine1(void) {
    INFO_LOG_CXX("while exit\n");
}

void ExitRoutine2(void) {
    INFO_LOG_CXX("exit\n");
}

void exitAT() {
    // 注册顺序和执行顺序相反
    // 对于ctrl+c之类的不起作用，只有exit()或关闭窗口时才会起作用
    atexit(ExitRoutine2);
    atexit(ExitRoutine1);

    // exit(0);
}

int main()
{
    exitAT();

    DEBUG_LOG_CXX("-------------------------------\n");

    testmyPrintf();
    
    CxxLog::getInstance().initLog("./alllog",
                                  1000,
                                  1024 * 1024 * 10);
    CxxLog::getInstance().setFileLogLevel(CxxLog::LOG_TYPE_DEBUG);
    CxxLog::getInstance().setFileLogLevel(CxxLog::LOG_TYPE_DEBUG);
    CxxLog::getInstance().setPrint(true);
    CxxLog::getInstance().setColorLog(true);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    {
		char srcdata[] = "\u2764\ufe0f❤️💖💖你好𐌰𰃺𰀀\1\2\3\4\5";//
		//const char *dict[4] = {"0","1","2","3"};//{"嗷","呜","啊","~"};
		const char *dict[4] = {"嗷","呜","啊","~"};
		char *pp = toBeastSound(srcdata,sizeof(srcdata),dict);
		if(pp!= NULL){
			DEBUG_LOG_CXX("%s\n",pp);
			int destsize;
			char *ppp = fromBeastSound((const char *)pp,strlen(pp),&destsize);	
			if(ppp!= NULL){
				DEBUG_LOG_CXX("%d,%s\n",destsize,ppp);
				for(int i=0;i<destsize;i++){
					printf("%02x",ppp[i]&0xff);
				}
				printf("\n");
				free(ppp);
			}
			free(pp);
		}
	}

    pthread_mutex_t mutex_lock=PTHREAD_MUTEX_INITIALIZER;
	{
		AUTOPTRNEW(p,100); // p会在生命周期结束后释放
		AUTOPTR p1 = malloc(20);
		DEBUG_LOG_CXX("p=%p,p1=%p\n",p,p1);
		AUTOPTR_MUTEX(mutex_lock);
		DEBUG_LOG_CXX("mutex_lock:%p\n",&mutex_lock);
	}
	pthread_mutex_destroy(&mutex_lock);

    {
		const char *tb = "酱雾纱雾雾酱酱酱纱匠雾匠酱匠酱纱酱酱酱雾酱匠纱匠酱雾雾酱纱酱雾纱纱酱酱纱纱纱雾匠纱匠酱雾纱雾雾酱雾匠酱匠纱匠雾纱雾酱酱酱纱酱雾雾雾酱酱纱纱纱雾匠雾雾匠雾纱雾雾匠雾纱雾匠纱匠雾酱纱匠匠酱纱酱雾纱纱酱匠纱纱纱雾匠雾纱酱雾纱雾雾酱纱匠纱匠纱匠雾纱纱匠雾酱纱酱雾纱纱酱雾纱纱纱雾匠纱雾雾雾纱雾雾酱纱匠纱匠纱匠雾纱纱雾纱酱纱酱雾雾纱纱酱纱纱纱雾匠雾匠纱雾纱雾雾匠纱雾雾匠纱匠雾纱纱酱匠酱纱酱雾雾纱酱匠纱纱纱雾匠纱纱纱雾纱雾雾匠纱雾匠匠纱匠雾纱雾酱匠酱纱酱雾纱纱酱匠纱纱纱雾雾雾雾纱雾纱雾雾酱纱雾纱匠纱匠雾酱雾酱酱酱纱酱雾纱酱纱匠纱纱纱雾雾匠匠酱雾纱雾雾匠酱匠纱匠纱匠雾酱酱酱雾酱纱酱雾纱酱纱雾纱纱纱雾雾酱酱雾雾纱雾雾酱纱酱雾匠纱匠雾酱酱纱纱酱纱酱雾雾纱雾纱纱纱纱雾雾匠酱纱雾纱雾雾酱纱酱雾匠纱匠雾酱雾酱酱酱纱酱雾纱匠酱匠纱纱纱雾匠纱纱纱雾纱雾雾匠雾酱匠匠纱匠雾酱匠纱匠酱纱酱雾纱匠纱匠纱纱纱雾雾纱纱纱雾纱雾雾匠匠匠匠匠纱匠雾纱纱纱匠酱纱酱雾纱雾酱匠纱纱纱雾匠雾纱酱雾纱雾雾酱雾酱匠匠纱匠雾纱纱酱酱酱纱酱雾纱雾酱纱纱纱纱雾雾雾雾雾雾纱雾雾匠雾匠雾匠纱匠雾酱雾酱匠酱纱酱雾纱雾纱匠纱纱纱雾纱匠酱雾匠雾酱匠匠雾酱酱酱雾匠匠酱匠雾匠匠酱酱纱酱酱匠雾纱纱纱雾雾纱纱酱雾纱雾雾酱纱酱酱匠纱匠雾纱雾酱酱酱纱酱雾雾雾纱酱纱纱纱雾匠雾纱酱雾纱雾雾酱雾匠匠匠纱匠雾酱雾雾雾酱纱酱雾纱纱酱匠纱纱纱雾雾纱纱匠雾纱雾雾酱雾雾酱匠纱匠雾纱纱酱纱酱纱酱雾雾纱酱雾纱纱纱雾雾纱纱雾雾纱雾雾酱雾匠纱匠纱匠雾纱雾纱纱酱纱酱雾雾纱纱纱纱纱纱雾匠雾雾雾雾纱雾雾酱纱纱匠匠纱匠雾酱纱匠雾酱纱酱雾雾纱纱匠纱纱纱雾匠纱纱雾雾纱雾雾匠纱雾匠匠纱匠雾纱雾酱匠酱纱酱雾纱纱酱匠纱纱纱雾雾雾雾纱雾纱雾雾匠雾匠匠匠纱匠雾酱雾纱雾酱纱酱雾纱雾雾纱纱纱纱雾雾雾酱纱雾纱雾雾匠雾匠酱匠纱匠雾酱雾雾纱酱纱酱雾纱雾酱酱纱纱纱雾雾雾雾纱雾纱雾雾酱纱酱纱匠纱匠雾纱纱纱纱酱纱酱雾纱雾纱酱纱纱纱雾纱匠酱雾匠匠匠纱纱酱雾酱酱雾酱纱雾酱纱雾酱纱酱雾纱雾纱雾纱纱纱雾雾雾雾雾雾纱雾雾匠雾匠匠匠纱匠雾酱雾酱匠酱纱酱雾纱纱酱酱匠纱匠匠纱纱雾匠匠雾匠纱酱匠纱匠匠纱匠雾酱匠纱酱酱纱酱雾纱酱匠纱纱纱纱雾雾匠雾雾雾纱雾雾匠匠酱酱匠纱匠雾酱匠雾酱酱纱酱雾酱匠匠雾匠";
		char *ppp = fromBeastSound_2byte((const unsigned char *)tb,strlen(tb));
		if(ppp!= NULL){
			DEBUG_LOG_CXX("%p,%s\n",ppp,ppp);
			free(ppp);
		}

		AUTOPTR p = fromBeastSound_2byte((const unsigned char *)tb,strlen(tb));
		if(p!= NULL){
			DEBUG_LOG_CXX("%p,%s\n",p,p);
		}
	}

    {
		sqlite3 *db = open_sqlite3_db("./alllog/test.db");
		if(db != NULL){
			int ret = create_table(db,"test","id int,name char(100),number char(100)");
			if(ret == 0){
				const int cmd_count = 1000;//1024*1024;
				char **cmds = (char**)malloc(cmd_count*sizeof(char*));
				DEBUG_LOG_CXX("执行%d条命令\n",cmd_count);
				for(int i=0;i<cmd_count;i++){
					cmds[i] = (char*)malloc(1000);
					sprintf(cmds[i],"insert into test values(%d,'%s%d','%s%d')",i,"name",i,"number",i);
					//DEBUG_PRINT_LOG("cmds[%d]=%s\n",i,cmds[i]);
					//ret = exec_sqlite3_sql(db,(const char*)cmds[i]);
				}
				DEBUG_LOG_CXX("使用事务执行%d条命令\n",cmd_count);
				ret = exec_sqlite3_sqls(db,(const char**)cmds,cmd_count);
				DEBUG_LOG_CXX("完成\n");
				if(ret == 0){
					DEBUG_LOG_CXX("exec_sqlite3_sqls success\n");
				}
				for(int i=0;i<cmd_count;i++){
					free(cmds[i]);
				}
				free(cmds);
			}

            int cnt = 1000*10;
            char cmd[512];
            INFO_LOG_CXX("开始%d条事务\n",cnt);
            ret = begin_sqlite3_affair(db);
            if(ret == 0){
                for(int i=0;i<cnt;i++){
                    sprintf(cmd,"insert into test values(%d,'%s%d','%s%d')",i,"namei",i,"numberi",i);
                    ret = exec_sqlite3_sql(db,(const char*)cmd);
                }
                INFO_LOG_CXX("提交 事务\n");
                ret = end_sqlite3_affair(db);
                INFO_LOG_CXX("完成 事务\n");
                if(ret != 0){
                    DEBUG_LOG_CXX("end_sqlite3_affair failed\n");
                }
            } else {
                DEBUG_LOG_CXX("begin_sqlite3_affair failed\n");
            }
            

			ret = get_table_names(db);
			ret = get_table_info(db,"test");
            ret = get_table_rowcount(db,"test");

			//drop_table(db,"test1");

			close_sqlite3_db(db);
		}
	}

    INFO_LOG_CXX("stop\n");

    return 0;
}