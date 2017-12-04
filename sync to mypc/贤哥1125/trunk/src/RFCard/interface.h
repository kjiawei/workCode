#ifndef INTERFACE_H
#define INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif
extern void setmodel(void);

extern int ReadPersonIDSexBorn(unsigned char *,unsigned char *);
extern int ReadPersonName(unsigned char *);
extern int ReadPersonSign(unsigned char *);
extern int ReadPersonDepartment(unsigned char *);
extern int ReadPersonDryWeight(unsigned char *);
extern int ReadPersonDiagnose(unsigned char *);
extern int ReadSetTimeTempNaHco3UFVRate(unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *);
extern int WriteSetTimeTempNaHco3UFVRate(unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char *);

#ifdef __cplusplus
}
#endif

#endif
