#include "dialog.h"
#include <QFileDialog>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "public.h"
#include "crc32.h"
#include "cmd.h"

dialog::dialog(QWidget *parent) :QDialog(parent)
{
    //
    FILE *fp;
    //
    char *product_file_name = (char *)"product.ini";
    //
    char *module_file_name = (char *)"module.ini";
    //
    errorMessageDialog = new QErrorMessage(this);
    //用来读配置文件，一行数据的用。
    char linebuf[4096];
    //
    char *ch;
    //
    QString str;

    int i;

    QRect rect;

    char *pos;

    product_label = new QLabel(tr("product"),this);
    rect.setRect(20,30,54,12);
    product_label->setGeometry(rect);

    product_combo = new QComboBox(this);
    rect.setRect(80,20,69,22);
    product_combo->setGeometry(rect);

    //设置动态的产品型号
    fp = fopen(product_file_name,"r");
    if (fp == NULL)
    {
        errorMessageDialog->showMessage(tr("can not open config product"));
        return ;
    }

    memset(&linebuf[0],0,sizeof(linebuf));
    while(fgets(linebuf,4096,fp))
    {
        //去掉回车换行符
        ch  = &linebuf[0];
        while (*ch != '\0')
        {
            if ( (*ch == '\n') || (*ch == '\r') )
            {
                *ch = '\0';
                break;
            }
            ch++;
        }

        //添加产品项
        str = QString(QLatin1String(linebuf));
        product_combo->addItem(str);
        memset(&linebuf[0],0,sizeof(linebuf));
    }

    fclose(fp);

    //
    fp = fopen(module_file_name,"r");
    if (fp == NULL)
    {
        errorMessageDialog->showMessage(tr("can not open module file"));
        return;
    }

    memset(&linebuf[0],0,sizeof(linebuf));
    count = 0;
    while(fgets(linebuf,4096,fp))
    {
        //去掉回车换行符
        ch  = &linebuf[0];
        while (*ch != '\0')
        {
            if ( (*ch == '\n') || (*ch == '\r') )
            {
                *ch = '\0';
                break;
            }
            ch++;
        }

        pos = strstr(linebuf," ");
        if (pos != NULL)
        {
            *pos = '\0';
            pos++;
            i = 0;
        }
        else
        {
            errorMessageDialog->showMessage(tr("can not load tag"));
            return ;
        }

        memset(module[count].tag,0,sizeof(module[count].tag));
        while ( (*pos != ' ') && (*pos != '\0') )
        {
            module[count].tag[i]= *pos;
            *pos = '\0';
            i++;
            pos++;
        }

        //
        str = QString(QLatin1String(linebuf));
        module[count].check = new QCheckBox(str,this);
        rect.setRect(30,70 + (40*count),91,16);
        module[count].check->setGeometry(rect);

        module[count].edit = new QLineEdit(this);
        rect.setRect(130,70 + (40*count),201,20);
        module[count].edit->setGeometry(rect);

        str = ".....";
        module[count].button = new CMyButton(str,this);
        rect.setRect(340,70 +(40*count),31,23);
        module[count].button->setGeometry(rect);
        module[count].button->setId(count);

        connect(module[count].button,
                SIGNAL(ClickedEvent(int)),
                this,
                SLOT(module_Button_clicked(int)));

        count++;
        memset(&linebuf[0],0,sizeof(linebuf));
    }

    fclose(fp);

    output_label = new QLabel(tr("output file"),this);
    rect.setRect(30,70 + (40*count),80,20);
    output_label->setGeometry(rect);

    output_Edit = new QLineEdit(this);
    rect.setRect(130,70 + (40*count),201,20);
    output_Edit->setGeometry(rect);

    str = ".....";
    output_Button = new QPushButton(str,this);
    rect.setRect(340,70 + (40*count),31,23);
    output_Button->setGeometry(rect);

    connect(output_Button,
            SIGNAL(clicked()),
            this,
            SLOT(output_Button_clicked()));

    str = "pack";
    pack_Button = new QPushButton(str,this);
    rect.setRect(24 ,110 + (40*count),351,23);
    pack_Button->setGeometry(rect);

    connect(pack_Button,
            SIGNAL(clicked()),
            this,
            SLOT(pack_Button_clicked()));

    if (output_Edit->text() == "")
    {
        str = "c:\\update.pkg";
        output_Edit->setText(str);
    }
}

dialog::~dialog()
{
    delete errorMessageDialog;
}


void dialog::module_Button_clicked(int i)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "c:\\",
                                                     tr("bin (*.bin)"));

    if (!fileName.isEmpty())
        module[i].edit->setText(fileName);
}

void dialog::output_Button_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                "c:\\update.pkg",
                                tr("pkg (*.pkg)"));

    if (!fileName.isEmpty())
        output_Edit->setText(fileName);
}

void dialog::pack_Button_clicked()
{
    FILE *fp;
    FILE *tmp_file;
    struct Args_t args;
    char buf[2048];
    char lf[3] = "\n";
    int i;

    if (product_combo->currentIndex() ==  -1)
    {
        errorMessageDialog->showMessage(tr("you no choose product type"));
        return ;
    }

    fp = fopen("pack.cfg","w");
    if (fp == NULL)
    {
        errorMessageDialog->showMessage(tr("can build config file"));
        return ;
    }

    for(i=0;i<count;i++)
    {
        if (module[i].check->checkState() == Qt::Checked)
        {
            tmp_file = fopen(module[i].edit->text().toLatin1().data(),"rb");
            if (tmp_file == NULL)
            {
                errorMessageDialog->showMessage(tr("no exist input file"));
                fclose(fp);
                return ;
            }
            else
            {
                fclose(tmp_file);
            }
        }
    }


    for(i=0;i<count;i++)
    {
        if (module[i].check->checkState() == Qt::Checked)
        {
            memset(buf,0,sizeof(buf));
            strcpy(buf,"tag=");
            strcat(buf,module[i].tag);
            strcat(buf," ");
            strcat(buf,"input=");
            strcat(buf,module[i].edit->text().toLatin1().data());
            fwrite(&buf[0],strlen(buf),1,fp);
            fwrite(lf,strlen(lf),1,fp);
        }
    }

    fclose(fp);

    args.configFileName = (char *)"pack.cfg";
    args.outFile = NULL;
    args.outFileName = output_Edit->text().toLatin1().data();
    args.productName = product_combo->currentText().toLatin1().data();

    pkg(args);
}

//计算文件大小
uint32_t dialog::get_file_size(const char *filename)
{
    unsigned long size;
    FILE* fp = fopen( filename, "rb" );

    if(fp==NULL)
    {
        errorMessageDialog->showMessage(tr("ERROR: Open file %s failed.\n", filename));
        return 0;
    }

    fseek( fp, SEEK_SET, SEEK_END );
    size=ftell(fp);

    fclose(fp);
    return size;
}

int dialog::pkg(struct Args_t args)
{
    //配置文件，就是说明要打包那些文件的文件
    FILE *config_file = NULL;
    //输入文件，就是配置文件里面的一项的文件
    FILE *input_file = NULL;
    //用来读配置文件，一行数据的用。
    char linebuf[4096];
    //输入文件的名笱
    char input_file_name[1024];
    //用于分析和定位tag和input等关键的坐标
    char *tag_pos,*input_pos,*tag_value_pos,*input_value_pos;
    //打包的结构
    UPDATE_PKG pkg;
    //计算现在有多少个需要打包的组件
    uint32_t target_components_num;
    //用于暂时存放打包组件的数据data，一个链表
    struct TMP_DATA * data = NULL;
    struct TMP_DATA * first_data = NULL;
    struct TMP_DATA * tmp_data = NULL;
    //用于暂时存放打包组件的compoent
    TARGET_COMPONENT component[256];
    P_TARGET_COMPONENT p_component;
    //计数器
    uint32_t i;

    //打开配置文件
    config_file = fopen(args.configFileName,"r");
    if (config_file == NULL)
    {
        errorMessageDialog->showMessage(tr("can not open config file"));
        return false;
    }

    //读到的配置项
    target_components_num = 0;

    //循环来读取配置文件的每一行，然后暂时放在缓存里面
    while(fgets(linebuf,4096,config_file))
    {
        //查找tag项
        tag_pos = strstr(linebuf,"tag");
        //查找input项
        input_pos = strstr(linebuf,"input");

        //没有找到tag项，则错误输出，并返回
        if (tag_pos == NULL)
        {
            errorMessageDialog->showMessage(tr("can not find tag,must be have tag"));
            fclose(config_file);
            return false;
        }

        //没有找到input项，则错误输出，并返回
        if (input_pos == NULL)
        {
            errorMessageDialog->showMessage(tr("can not find input,must be have input"));
            fclose(config_file);
            return false;
        }

        //查找tag项和input项的＝号
        tag_value_pos = strstr(tag_pos,"=");
        input_value_pos = strstr(input_pos,"=");

        //没有查找到=号，则错误输出，并返回。有找到，就读=后一个位置
        if ( tag_value_pos != NULL )
        {
            tag_value_pos++;
        }
        else
        {
            errorMessageDialog->showMessage(tr("can not find = , must be have ="));
            return false;
        }

        //没有查找到=号，则错误输出，并返回。有找到，就读=后一个位置
        if ( input_value_pos != NULL )
        {
            input_value_pos++;
        }
        else
        {
            printf("can not find = , must be have =\n");
            printf("\n");
            return false;
        }

        //读缓存comonent的位置
        p_component = (P_TARGET_COMPONENT) (&component[target_components_num]);

        //
        memset(&p_component->reserved[0],0,sizeof(p_component->reserved));
        memset(&p_component->tag[0],0,sizeof(p_component->tag));

        //input的名称读取，即input等号后面的字符串
        i = 0;
        memset(&input_file_name[0],0,sizeof(input_file_name));
        while ( ( *input_value_pos != ' ' ) &&  ( *input_value_pos != '\r' ) &&( *input_value_pos != '\n' )  )
        {
            *(input_file_name + i) = *input_value_pos;
            input_value_pos++;
            i++;
        }

        //tag的名称读取，即tag等号后面的字符串
        i = 0;
        memset(&p_component->tag[0],0,sizeof(p_component->tag));
        while ( ( *tag_value_pos != ' ' ) &&  ( *tag_value_pos != '\r' ) && ( *tag_value_pos != '\n' )  )
        {
            p_component->tag[i] = *tag_value_pos;
            tag_value_pos++;
            i++;
        }

        //如果组件文件，是一个不存在的文件，就错误通知，并退出
        input_file = fopen(input_file_name,"rb");
        if ( input_file == NULL )
        {
            errorMessageDialog->showMessage(tr("can not find input file\n"));
            return false;
        }

        //读取组件文件的大小
        p_component->data_size = get_file_size(input_file_name);

        //如果组件文件，小于等于0，则错误，并返回
        if (p_component->data_size <= 0)
        {
            errorMessageDialog->showMessage(tr("input file length is less than or equ 0"));
            fclose(config_file);
            fclose(input_file);
            return false;
        }

        //分配缓冲组件项
        data = (struct TMP_DATA *)malloc(sizeof(struct TMP_DATA));
        if (data == NULL)
        {
            printf("can't malloc\n");
        }

        //根据组件大小，分配缓冲组件的文件大小
        data->data = (uint8_t *)malloc(p_component->data_size);
        if (data->data == NULL)
        {
            errorMessageDialog->showMessage(tr("can't malloc"));
        }

        //把组件的内容，写到缓冲组件去。
        fread(data->data,p_component->data_size,1,input_file);
        fclose(input_file);

        //设置缓冲组件的大小
        data->size = p_component->data_size;

        //把缓冲组件，加去缓冲组件的链表里面去
        if ( target_components_num == 0 )
        {
            tmp_data = data;
            first_data = data;
            data->next = NULL;
        }
        else
        {
            tmp_data->next = data;
            tmp_data = tmp_data->next;
            data->next = NULL;
        }

        //读取到的组件数加一
        target_components_num++;
    }

    fclose(config_file);

    //读取配置文件，看是不是只输入了0项。
    if ( target_components_num <= 0 )
    {
        errorMessageDialog->showMessage(tr("target_componets_num is less than or equ 0"));
        return false;
    }

    //配置pkg，只配置一部分。
    memset(&pkg.product_tag[0],0,sizeof(pkg.product_tag));
    memcpy(&pkg.product_tag[0],args.productName,strlen(args.productName));
    pkg.target_components_num = target_components_num;

    //计算crc
    pkg.CRC = 0xffffffff;
    pkg.CRC = my_crc32(pkg.CRC,(const unsigned char *)&pkg.product_tag[0],sizeof(pkg.product_tag));
    pkg.CRC = my_crc32(pkg.CRC,(const unsigned char *)&pkg.target_components_num,sizeof(pkg.target_components_num));
    pkg.CRC = my_crc32(pkg.CRC,(const unsigned char *)&component[0],sizeof(TARGET_COMPONENT)*pkg.target_components_num);

    tmp_data = first_data;

    for(i=0;i<pkg.target_components_num;i++)
    {
        pkg.CRC = my_crc32(pkg.CRC,tmp_data->data,tmp_data->size);
        tmp_data = tmp_data->next;
    }

    //生成打包文件
    args.outFile = fopen(args.outFileName,"wb+");
    if (args.outFile == NULL)
    {
        errorMessageDialog->showMessage(tr("output file fail"));
        tmp_data = first_data;
        for(i=0;i<pkg.target_components_num;i++)
        {
            first_data = tmp_data->next;
            tmp_data->next = NULL;
            free(tmp_data->data);
            free(tmp_data);
            tmp_data = first_data;
        }
        return false;
    }

    fwrite(&pkg.CRC,sizeof(pkg.CRC),1,args.outFile);
    fwrite(&pkg.product_tag[0],sizeof(pkg.product_tag),1,args.outFile);
    fwrite(&pkg.target_components_num,sizeof(pkg.target_components_num),1,args.outFile);
    fwrite(&component[0],sizeof(TARGET_COMPONENT) * pkg.target_components_num,1,args.outFile);

    tmp_data = first_data;

    for(i=0;i<pkg.target_components_num;i++)
    {
        fwrite(tmp_data->data,tmp_data->size,1,args.outFile);
        first_data = tmp_data->next;
        tmp_data->next = NULL;
        free(tmp_data->data);
        free(tmp_data);
        tmp_data = first_data;
    }

    fclose(args.outFile);

    errorMessageDialog->showMessage(tr("finish"));

    return true;
}
