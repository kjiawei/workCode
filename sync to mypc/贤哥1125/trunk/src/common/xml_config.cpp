#include "xml_config.h"
#include "xml_parse.h"
#include "global_var.h"

XmlConfig::XmlConfig()
{
    listXmlConfig.push_back(this);
}

void XmlConfigRead(const char* file)
{
    QMap<QString, QString> mapV;

    PacketFluid packetFluid;
    dataRead(PACKET_NAME_DIALYSATE,&packetFluid);

    PacketSdoRangeTemp packetSdoRangeTemp;
    QDomDocument doc;

    xml_DocReadOpen(&doc, file);

    if ((xml_NodeReadValue(&doc, (char *)"/Config/Dialysate/Default", &mapV) ==0) &&
            (mapV.isEmpty() == false)) {
  //      packetFluid.tempTarget = mapV["temp"].toUShort();
  //      packetFluid.tempTarget *= 10;
        packetFluid.tempTarget=mapV.value("temp").toFloat()*10;
        packetFluid.Wspeed = mapV["flux"].toUShort();
        PacketWorkMode workModeData;
        dataRead(PACKET_NAME_WORKMODE,&workModeData);
        if(workModeData.treatmode.current == HDF_pre_dilute||workModeData.treatmode.current == HDF_post_dilute)
            packetFluid.Wspeed=700;

    //    dataNodeUpdate(PACKET_NAME_DIALYSATE, &packetFluid);

        packetSdoRangeTemp.N3_set_temp_Up = mapV["upper"].toUShort();
        packetSdoRangeTemp.N3_set_temp_Lower = mapV["lower"].toUShort();
        dataNodeUpdate(PACKET_SDO_RANGETEMP, &packetSdoRangeTemp);
    }
    mapV.clear();


//    PacketSdoBlood packetSdoBlood;
//    if ((xml_NodeReadValue(&doc, (char *)"/Config/BloodPump/Default", &mapV) ==0) &&
//            (mapV.isEmpty() == false)) {
//        packetSdoBlood.N3_set_BPspeed = mapV["bloodFlux"].toUShort();
//        packetSdoBlood.N3_set_BPtube = (unsigned char)mapV["dia"].toUShort();
//        dataNodeUpdate(PACKET_SDO_BLOOD, &packetSdoBlood);
//    }
//    mapV.clear();

    PacketSdoHeparin packetSdoHeparin;
    if ((xml_NodeReadValue(&doc, (char *)"/Config/Heparin/Default", &mapV) ==0) &&
            (mapV.isEmpty() == false)) {
        packetSdoHeparin.N3_set_heparin_first = mapV["firstDose"].toUShort();
        packetSdoHeparin.N3_set_heparin_speed = mapV["inFlux"].toUShort();
        dataNodeUpdate(PACKET_SDO_HEPARIN, &packetSdoHeparin);
    }
    mapV.clear();

    //PacketPreflush packetPreflush;
    
if ((xml_NodeReadValue(&doc, (char *)"/Config/Preflush/Default", &mapV) ==0) &&
            (mapV.isEmpty() == false))
    {
        //packetPreflush.PreflushBloodSpeed = mapV["bloodspeed"].toUShort();
        //packetPreflush.PreflushDialysateFlow = mapV["dialysateflow"].toUShort();
        //packetPreflush.PreflushTime = mapV["time"].toUShort();
        //packetPreflush.PreflushUfRate= mapV["Ufrate"].toUShort();
        //packetPreflush.PreflushUfVolume= mapV["Ufvolume"].toUShort();
        //packetPreflush.TractBloodVolume= mapV["bloodvolume"].toUShort();
        //packetPreflush.TractBloodVolume= mapV["bloodvolume"].toUShort();
        //dataNodeUpdate(PACKET_PREFLUSH, &packetPreflush);
    }
    mapV.clear();

    //PacketAP APTmep;
    //PacketVP VPTmep;
    //PacketTMP TMPTmep;
    //if ((xml_NodeReadValue(&doc, (char *)"/Config/Pressure/Default", &mapV) ==0) &&
    //        (mapV.isEmpty() == false)) {
    //    APTmep.apUp = mapV["apUpper"].toUShort();
    //    APTmep.apL = mapV["apLower"].toUShort();
    //    dataNodeUpdate(PACKET_NAME_AP, &APTmep);

    //    VPTmep.vpUp = mapV["vpUpper"].toUShort();
    //    VPTmep.vpL = mapV["vpLower"].toUShort();
    //    dataNodeUpdate(PACKET_NAME_VP, &VPTmep);

    //    TMPTmep.tmpUp = mapV["tmpUpper"].toUShort();
    //    TMPTmep.tmpL = mapV["tmpLower"].toUShort();
    //    dataNodeUpdate(PACKET_NAME_TMP, &TMPTmep);
    //}
    //mapV.clear();
}

void ReUpdateXmlConfig(const char* file, const char* cmd)
{
    list<XmlConfig*>::iterator it;
    for (it=listXmlConfig.begin(); it != listXmlConfig.end(); ++it) {
        (*it)->CustomXmlConfig(file, cmd);
    }
    XmlConfigRead(file);
}
