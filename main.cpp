#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include "cxxlog.h"
#include "sql/baseSqlite3.h"
#include "decode/beastSound.h"
#include "decode/finallyRecovery.h"
#include "printf/myPrintf.h"
#include "decode/generatesigal.h"

static int test() {
     try {
        // 1. 生成 1 秒、双通道、采样率 48000 Hz、频率 440 Hz、幅度 0.8 的 float 信号
        auto signalFloat = generateSignal<float>(48000.0, 440.0, 0.8f, 2, 1.0);
        std::cout << "Float signal size: " << signalFloat.size() << " samples (interleaved)\n";
        // 打印前 8 个样本（左声道，右声道交替）
        std::cout << "First 8 samples (L,R): ";
        for (size_t i = 0; i < 8 && i < signalFloat.size(); ++i) {
            std::cout << signalFloat[i] << " ";
        }
        std::cout << "\n\n";

        // 2. 生成 1 秒、单声道、采样率 16000 Hz、频率 1000 Hz、幅度 0.5 的 int16_t 信号
        auto signalInt16 = generateSignal<int16_t>(16000.0, 1000.0, 0.5, 1, 1.0);
        std::cout << "int16_t signal size: " << signalInt16.size() << " samples\n";
        std::cout << "First 10 samples: ";
        for (size_t i = 0; i < 10 && i < signalInt16.size(); ++i) {
            std::cout << signalInt16[i] << " ";
        }
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
	
	try {
        // 1. 创建生成器：采样率 48000 Hz，频率 440 Hz，幅度 0.8，双通道，float 类型
        ContinuousSignalGenerator<float> gen(48000.0, 440.0, 0.8f, 2);
        
        // 生成前 10 帧（每帧为左右声道，交错方式输出或逐帧查看）
        std::cout << "First 10 frames (L,R interleaved):" << std::endl;
        for (int i = 0; i < 10; ++i) {
            auto frame = gen.nextFrame();  // 返回 vector<float>，大小为2
            std::cout << "Frame " << i << ": L=" << frame[0] << ", R=" << frame[1] << std::endl;
        }
        
        // 或者直接生成一个交错块（一次性获取 100 个样本 = 50 帧）
        auto block = gen.generateBlock(100);
        std::cout << "\nBlock size: " << block.size() << " samples (interleaved)" << std::endl;
        
        // 2. 使用 int16_t 类型，单声道，幅度 0.5（归一化）
        ContinuousSignalGenerator<int16_t> genInt16(16000.0, 1000.0, 0.5, 1);
        std::cout << "\nint16_t samples (mono):" << std::endl;
        for (int i = 0; i < 8; ++i) {
            std::cout << genInt16.nextSample() << " ";
        }
        std::cout << std::endl;
        
        // 3. 演示频率变化（连续相位）
        ContinuousSignalGenerator<double> genVar(44100.0, 440.0, 0.5, 1);
        std::cout << "\nFrequency sweep (phase continuous):" << std::endl;
        for (int i = 0; i < 100; ++i) {
            if (i == 50) {
                genVar.setFrequency(880.0);  // 中途改变频率
                std::cout << "\nFrequency changed to 880 Hz at sample 50" << std::endl;
            }
            // 仅打印部分样本
            if (i % 20 == 0) {
                std::cout << genVar.nextSample() << " ";
            } else {
                genVar.nextSample(); // 消耗样本
            }
        }
        std::cout << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

	try {
        // 1. 生成双通道正弦波（float）
        WaveformGenerator<float> sineGen(48000.0, 440.0, 0.8f, 2, WaveformType::Sine);
        std::cout << "Sine wave, first 5 frames:\n";
        for (int i = 0; i < 5; ++i) {
            auto frame = sineGen.nextFrame();
            std::cout << "Frame " << i << ": L=" << frame[0] << ", R=" << frame[1] << std::endl;
        }
        
        // 2. 生成单声道方波（int16_t）
        WaveformGenerator<int16_t> squareGen(16000.0, 1000.0, 0.5, 1, WaveformType::Square);
        std::cout << "\nSquare wave (int16_t), first 8 samples:\n";
        for (int i = 0; i < 8; ++i) {
            std::cout << squareGen.nextSample() << " ";
        }
        std::cout << std::endl;
        
        // 3. 生成三角波并动态改变频率
        WaveformGenerator<double> triGen(44100.0, 200.0, 0.6, 1, WaveformType::Triangle);
        std::cout << "\nTriangle wave, frequency sweep (phase continuous):\n";
        for (int i = 0; i < 100; ++i) {
            if (i == 50) {
                triGen.setFrequency(400.0);
                std::cout << "\nFrequency changed to 400 Hz at sample 50\n";
            }
            if (i % 25 == 0) {
                std::cout << triGen.nextSample() << " ";
            } else {
                triGen.nextSample();
            }
        }
        std::cout << std::endl;
        
        // 4. 生成白噪声（双声道，浮点）
        WaveformGenerator<float> noiseGen(48000.0, 0.0, 0.3, 2, WaveformType::Noise);
        std::cout << "\nWhite noise, first 8 samples (interleaved L,R):\n";
        auto noiseBlock = noiseGen.generateBlock(16);  // 16个样本 = 8帧
        for (size_t i = 0; i < noiseBlock.size(); ++i) {
            std::cout << noiseBlock[i] << (i % 2 == 1 ? "\n" : " ");
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
	
    return 0;
}

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

	INFO_LOG_CXX("generateSignal\n");

	test();

    INFO_LOG_CXX("stop\n");

    return 0;
}