/*
    Copyright (C) 2001  ITK
    Author  : Yevgen Bondar <elb@lg.bank.gov.ua>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/

//������ ��⨬���஢���� ���ᨨ �⠭���⭮�� Examplep.prg


/***
 StrZero( <nNumber>, <nLength>, <nDecimals> ) --> cNumber

 �८�ࠧ������ �᫠ � ��ப� 䨪�஢����� ����� � ����騬� ��ﬨ
*/

FUNC StrZero( n, nLength, nDecimals )
LOCAL cNumber

IF EMPTY(nDecimals)
	nDecimals=0
ENDIF
IF EMPTY(nLength)
	nLength=10
ENDIF

cNumber:=LTRIM(STR(ABS(n),nLength,nDecimals))
RETURN	IF(n<0,'-','')+REPL('0',nLength-LEN(cNumber))+cNumber

/***
	DAYS( <nSeconds> )

 �����頥� 楫�� �᫮ ���� ��室� �� ��������� �᫠ ᥪ㭤

 ����砭��: ���⮪ �� ������� �� 24 �� �����頥��� �㭪樥� TSTRING().
*/

FUNC DAYS(cl_secs)
RETURN INT( cl_secs / 86400 )

/***
	DBF()

 �����頥� ����� ⥪�饩 ���� ������ (��७�ᥭ� � ASM)

*/
EXTERNAL DBF


/***
	ELAPTIME( <nStartTime>, <nEndTime> )

 �����頥� ��ப� � ࠧ��楩 �६��� ����� ��砫�� � ������

* ����砭��:	�᫨ ��砫쭮� �६� ����� ����筮��, �।����������
		���室 �� ���� ��⪨. ���쪮 ��� 24-�ᮢ��� �ଠ�.
		� 24 ��� 86400 ᥪ㭤.
*/

FUNC ELAPTIME(cl_start, cl_end)

RETURN TSTRING( IF( cl_end < cl_start, 86400, 0) + ;
		SECS(cl_end) - SECS(cl_start) )

/***
	LENNUM( <nNum> )
 �����頥� ����� �� �८�ࠧ������ �᫠ � ��ப�
*/

FUNC LENNUM(cl_number)
RETURN	LEN(LTRIM(STR(cl_number)))

/***
	MOD( <expN1>, <expN2> )

 �����頥� ���⮪ �� ������� <expN1> �� <expN2> � ᮮ⢥��⢨�
 � �����⬮� dBASE-�㭪樨 MOD()

 ����砭��: �����稥 ����� dBASE � Clipper, ���⪮� �� �������,
 ���������� ��५���� "<-->":

	Clipper-������:        dBASE-�㭪��:
	-----------------        -----------------
	 3 %  0 ::=  0.00  <-->  MOD( 3, 0) ::=  3
	 3 % -2 ::=  1.00  <-->  MOD( 3,-2) ::= -1

	-3 %  2 ::= -1.00  <-->  MOD(-3, 2) ::=  1
	-3 %  0 ::=  0.00  <-->  MOD(-3, 0) ::= -3

	-1 %  3 ::= -1.00  <-->  MOD(-1, 3) ::=  2
	-2 %  3 ::= -2.00  <-->  MOD(-2, 3) ::=  1

	 2 % -3 ::=  2.00  <-->  MOD( 2,-3) ::= -1
	 1 % -3 ::=  1.00  <-->  MOD( 1,-3) ::= -2
*/

FUNC MOD(cl_num, cl_base)
LOCAL cl_result:= cl_num % cl_base

RETURN IF( cl_base = 0, ;
	   cl_num,;
	   IF(cl_result * cl_base < 0, cl_result + cl_base, cl_result) )


/***
	READKEY()

 �����頥� �����, �।�⠢���騩 ������� ������� �� ��室� �� ०���
 ������࠭���� ।���஢���� - ������ dBASE-�㭪樨 READKEY().

 ����砭��: ����� �।�⠢���� ࠧ���� dBASE-�㭪樨 READKEY() �
 Clipper-�㭪樨 LASTKEY():

	������ ��室�:    dBASE:      Clipper:
	--------------     ------      --------
	�����                  0       ��� ��室�
	^D, ^L                 1       ��� ��室�
	�����                  2       ��� ��室�
	��ࠢ�                 3       ��� ��室�
	�����                  4       ��� ��室�
	����                   5       ��� ��室�
	PgUp                   6          18
	PgDn                   7           3
	Esc, ^Q               12          27 (⮫쪮 Esc)
	^End, ^W              14          23 (⮫쪮 ^W)
	����� ��᫥ ����     15       ASCII-��� ��᫥����� ᨬ����
	Enter                 15          13
	^Home                 33       ��� ��室�
	^PgUp                 34       ��� ��室�
	^PgDn                 35       ��� ��室�
	F1                    36       ��� ��室�

 dBASE III �������� 256 � ��室���� ����, �᫨ ���짮��⥫� �������
 ��-����. Clipper �ᯮ���� ᢮� �㭪�� UPDATED() ��� ��।������,
 �뫨 �� ���������.
*/
FUNC READKEY()
LOCAL _res:=0,_lk:=LASTKEY()
DO CASE
	CASE _lk = 18 			// PgUp
		_res:= 6
	CASE _lk =  3 			// PgDn
		_res:= 7
	CASE _lk = 27 			// Esc
		_res:=12
	CASE _lk = 23 			// ^W
		_res:=14
	CASE _lk = 13 .OR. _lk>=32	// Enter,����� ��᫥ ���� ����
		_res:=15
	CASE _lk = 31 			// ^PgUp
		_res:=34
	CASE _lk = 30 			// ^PgDn
		_res:=35
ENDCASE

RETURN	_res+IF(UPDATED(),256,0)


/***
 SECS( <��ப� �६���>)

 �����頥� �᫮ ᥪ㭤, ᮤ�ঠ���� � ��।����� ��ࠬ���

	��᫮ ᥪ㭤 � ��ਮ��
	-------    -----------
	     60    1 �����
	   3600    1 ��
	  86400    1 ����
*/

FUNC SECS(cl_time)

RETURN	VAL( cl_time ) * 3600 +;
	VAL(SUBSTR(cl_time,4)) * 60 +;
	VAL(SUBSTR(cl_time,7))


/***
	TSTRING( <seconds> )

 �८�ࠧ�� ᥪ㭤� � ��ப� �६���

 �६� �ॢ���饥 24 �� �����頥��� �㭪樥� DAYS().
*/

FUNC TSTRING(cl_secs)

RETURN	STRZERO( INT(MOD(cl_secs/3600, 24)), 2 ) +':'+;
	STRZERO( INT(MOD(cl_secs/  60, 60)), 2 ) +':'+;
	STRZERO( INT(MOD(cl_secs     , 60)), 2 )
