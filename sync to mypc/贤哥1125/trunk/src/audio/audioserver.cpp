#include "audioserver.h"
#include <signal.h>
#include "sound_adjust.h"
#include "config_ui.h"
#include "common_func.h"
#include "global_var.h"

#define BUFSIZE 512
#define SLIENT

struct RIFF_Header {
    char RIFF_ID[4];
    uint32_t RIFF_Size;
    char RIFF_Format[4];
};

struct Chunk_Header {
    char Chunk_ID[4];
    uint32_t Chunk_Size;
};

struct Wave_Format {
    uint16_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t AvgBytesPerSec;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;
};

FILE  fdingwave;
int dingsize;
FILE falarmmedwave;
int alarmmedsize;
static char buf[BUFSIZE];
bool read_flag=false;

extern "C" {
    void sigroutine(int dunno);
}

int dsp_fd;

int buttons_fd;

audioserver::audioserver(void)
    :audio_sem(0)
{
    flag=1;
    alarmBit = false;//true;#/

//    audio_sem=0;

#ifdef OS_LINUX
    signal(SIGKILL, sigroutine);

    //system("/bin/adjust_sound -b 1");#/
    SoundAdjustMode(1);

    dsp_fd = open("/dev/dsp", O_RDWR);

    //buttons_fd = open("/dev/buttons", O_RDWR);
    if (buttons_fd < 0)
    {
        qDebug()<<"open of /dev/buttons failed";
    }
    if (dsp_fd < 0)
    {
        //qDebug()<<"open of /dev/dsp failed";
    }
    else
    {
        fwave = NULL;
        beep_filename = "./resource/sounds/ding.wav";              //default wav
        alarm_filename = "./resource/sounds/genmed.wav";//
        notice_filename = "./resource/sounds/notice.wav";//

        //开始一个线程，线程任务通过调用虚函数run实现#/
        start(QThread::TimeCriticalPriority);
    }
#endif
}

audioserver::~audioserver()
{
#ifdef OS_LINUX
    close(dsp_fd);
    flag = 0;
#endif
}

void sigroutine(int dunno)
{
    dunno = dunno;

#ifdef OS_LINUX
    qDebug()<<"killall signal\r\n";
    close(dsp_fd);
    //close(buttons_fd);#/
#endif
}

bool audioserver::alarmSilenceState()
{
    return AlarmSilent;
}

void audioserver::beep()
{
    flag = 2;
    saveflag=flag;
    if(G_sound_shielding==false)
        audio_sem.release();
}

void audioserver::alarmSilence(bool tag)
{
    if(AlarmSilent==tag)
        return;
#ifdef SLIENT
    //system("/bin/adjust_sound -b 0");
    if(tag==true)
    {
        //SoundAdjustMode(0);
        AlarmSilent=true;
    }
   else
    {
        //SoundAdjustMode(1);
        AlarmSilent=false;
        flag =3;
    }

    if(G_sound_shielding==false)
        audio_sem.release();
#endif
}

void audioserver::playAlarm(QString file, int msDelay)
{
    if(alarm_filename==file&&this->msDelay==msDelay&&flag==4)
        return;

    if(alarmBit == true) {
        if(!AlarmSilent)
          flag = 4;
        alarm_filename = file;
        savewrited = 0;
        this -> msDelay = msDelay;
    }

    if(G_sound_shielding==false)
        audio_sem.release();
}

void audioserver::playNotice(QString file, int msDelay)
{
    if(notice_filename==file&&this->msDelay==msDelay&&flag==5)
        return;

        flag = 5;//
        notice_filename = file;
        savewrited = 0;
        this -> msDelay = msDelay;

        if(G_sound_shielding==false)
            audio_sem.release();
}

void audioserver::alarmStart()
{
    alarmBit = true;
}

void audioserver::alarmStop()
{
    if(alarmBit==false&&flag==1)
        return;

    alarmBit = false;
    alarm_filename = "";
    flag = 1;
    //alarmSilence();

    if(G_sound_shielding==false)
        audio_sem.release();
}

int audioserver::alarmState()
{
   return alarmBit;
}

int audioserver::initDSP(QString filename)
{

#ifdef OS_LINUX
    int status;
    int arg;
    struct RIFF_Header riff_header;
    struct Chunk_Header fmt_chunk, data_chunk;
    struct Wave_Format wavfmt;

    //打开音频文件#/
    fwave = fopen(filename.toLatin1().data(), "r");
    if(fwave == NULL) {
        //qDebug()<<"file is not exist";
        flag = 1;
        return -1;
    }
    //将音频文件按一定格式读取出来#/
    arg = fread(&riff_header, 1, sizeof(struct RIFF_Header), fwave);
    if( strncmp(riff_header.RIFF_ID, "RIFF", 4) || strncmp(riff_header.RIFF_Format, "WAVE",4)){
        qDebug() << "Unknown file format.\n";
    }

    arg = fread(&fmt_chunk, 1, sizeof(struct Chunk_Header), fwave);
    if( !strncmp(fmt_chunk.Chunk_ID, "fmt ", 4) ) {
        /* this is a fmt chunk */
        arg = fread(&wavfmt, 1, sizeof(struct Wave_Format), fwave);

        arg = wavfmt.BitsPerSample;
        status = ioctl(dsp_fd, SOUND_PCM_WRITE_BITS, &arg);
        if (status == -1)
            qDebug()<<"SOUND_PCM_WRITE_BITS ioctl failed";
        if (arg != wavfmt.BitsPerSample)
            qDebug()<<"unable to set sample size";

        arg = wavfmt.NumChannels;
        status = ioctl(dsp_fd, SOUND_PCM_WRITE_CHANNELS, &arg);
        if (status == -1)
            qDebug()<<"SOUND_PCM_WRITE_CHANNELS ioctl failed";
        if (arg != wavfmt.NumChannels)
            qDebug()<<"unable to set number of channels";

        arg = wavfmt.SampleRate;
        status = ioctl(dsp_fd, SOUND_PCM_WRITE_RATE, &arg);
        if (status == -1)
            qDebug()<<"SOUND_PCM_WRITE_WRITE ioctl failed";

        /* skip extra bytes */
        fseek(fwave, fmt_chunk.Chunk_Size - 16 + fmt_chunk.Chunk_Size%2, SEEK_CUR);
    } else {
        qDebug()<<"Can't find fmt chunk.\n";
        return -1;
    }

    //
//    if(playmap.contains(filename))
//    {
//        sdata=playmap.value(filename);
//        if (sdata != NULL){
//        playsize=sdata->size;
//        fwave=sdata->wave;
//        }

//        if(filename=="./resource/sounds/ding.wav"){
//            playsize=dingsize;
//            *fwave=fdingwave;
//        }
//        else{
//            playsize=alarmmedsize;
//            *fwave=falarmmedwave;
//         }


//       return  playsize;
//     }
//    else
    {


    while( fread(&data_chunk, 1, sizeof(struct Chunk_Header), fwave) != 0 ){
        if( strncmp(data_chunk.Chunk_ID, "data", 4) ){
            /* skip unknown chunks */
            fseek(fwave, data_chunk.Chunk_Size + fmt_chunk.Chunk_Size%2, SEEK_CUR);
        } else {
            break;
        }
    }

    return  data_chunk.Chunk_Size;
    }
#endif

#ifdef OS_WIN32
    filename = filename;
#endif
    return 0;
}

//声音告警线程任务内容处理#/
void audioserver::run()
{
#ifdef OS_LINUX
    int readbytes = 0;
    int writebytes = 0;
    int writed = 0;
    AlarmSilent=false;

    while(flag != 0){
#if DEBUG_OTHER_FLAG
    Debug_insert("audioserver::run_in");
#endif
            if(G_sound_shielding==true)      //工程模式声音静音标志#/
            {
                audio_sem.acquire();
                msleep(100);

            }else if(flag == 3){
                if(writed >= (playsize-58))
                { //当播放的长度达到要求长度时，即音频播放完毕时#/
                    //qDebug()<<"play over"<<playsize<<"writed"<<writed;
                    //判断如是报警音，则延时后，重复播放#/
                    if(alarmBit == true && alarm_filename != "" && AlarmSilent==false)
                    {
                        flag = 4;
                        msleep(this->msDelay);
                        saveflag=0;
                    }
                    else if(saveflag!=2)   //如是按键音和提示音，则停止#/
                    {
                        flag  = 1;
                    }
                }
                else
                {
                     //把音频文件读出来，并写入dsp中，写入一次播放一次#/
                    /*   //20151022 修改#/
                    if(read_flag==false)
                    {
                        readbytes = fread(buf, 1, BUFSIZE, fwave);
                        read_flag=true;
                    }
                    */

                    readbytes = fread(buf, 1, BUFSIZE, fwave);  //20151022 修改#/

                    writebytes = write(dsp_fd, buf, readbytes);
                    if( writebytes != readbytes ) {
                        qDebug()<<"wrote wrong number of byte";
                        continue;
                    }
                    writed += readbytes; //将播放的音频文件长度记录下来#/
                }
            } else if(flag == 2) {            //beep 按键音#/
                if(currentfile.compare(beep_filename)==0) //如上次播放的音频文件按键音，则不需重新读取#/
                {
                    fseek(fwave,58,SEEK_SET);
                }
                else //需重新读取设置音频文件#/
                {
                    if(alarmBit == true && AlarmSilent==false) {
                        savewrited = writed;  //将报警音已写入的长度保存，在下一次报警音时从相应位置开始播放#/
                    }

                    if(fwave != NULL) {   //关闭之前打开的音频文件#/
                        fclose(fwave);
                        fwave = NULL;
                    }
                    currentfile = beep_filename;

                    playsize = initDSP(currentfile);   //根据声音类型重新打开相应的音频文件#/
                    read_flag=false;

                    if(playsize <= 0) {
                        currentfile = "";
                        flag = 1;
                        continue;
                    }
                }

                flag = 3;    //进入播放操作#/
                writed = 0;  //重头开始写入音频文件#/
            }else if(flag == 5) {  //notice 提示音#/
                        if(currentfile.compare(notice_filename)==0) {
                            fseek(fwave,58,SEEK_SET);
                        } else {
                            if(alarmBit == true && AlarmSilent==false) {
                                savewrited = writed;
                            }
                            if(fwave != NULL) {
                                fclose(fwave);
                                fwave = NULL;
                            }
                            currentfile = notice_filename;

                            playsize = initDSP(currentfile);
                            read_flag=false;

                            if(playsize <= 0) {
                                currentfile = "";
                                flag = 1;
                                continue;
                            }
                        }

                        flag = 3;
                        writed = 0;

            } else if(flag == 4){            //alarm 告警音#/
                writed = 0;

                //qDebug()<<"flag==4"<<playsize<<"writed:"<<writed;
                if(currentfile.compare(alarm_filename) ==0 ){
                    fseek(fwave, 58, SEEK_SET);
                } else {
                    if(fwave != NULL) {
                        fclose(fwave);
                        fwave = NULL;
                    }
                    currentfile = alarm_filename;

                    playsize = initDSP(currentfile);
                    read_flag=false;

                    if(playsize <= 0){
                        currentfile = "";
                        flag = 1;
                        continue;
                    }
                    if(savewrited != 0) {
                        fseek(fwave, 32 + savewrited, SEEK_SET);
                    }
                    writed = savewrited;
                }
                savewrited = 0;
                flag = 3;
            }
           else {

                audio_sem.acquire();

                usleep(5000);
            }
#if DEBUG_OTHER_FLAG
    Debug_insert("audioserver::run_out");
#endif
    }
    fclose(fwave);
#endif
}
