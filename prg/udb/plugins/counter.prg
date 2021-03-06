/*
    Copyright (C) 2002  ITK
    Author   : Uri (uri@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/

/*
  fun-ctions for view and edit COUNTER information
*/

#include "box.ch"

/* Can`t defined MAIN function name */

local ret := NIL
parameters query_key
	query_key := alltrim(upper(query_key))
	do case
		case query_key == "VIEW_CARD"
			ret:={|p1,p2,p3,p4,p5| ab_count_view_card(p1,p2,p3,p4,p5)}
		case query_key == "EDIT_CARD"
			ret:={|p1,p2,p3,p4,p5| ab_count_edit_card(p1,p2,p3,p4,p5)}
		otherwise
			ret:=[COUNTER plugins not supported function:]+query_key
	end
return ret

************************************************************
static function ab_count_view_card(oDep,data,oBox,colorSpec)
	local ret:=.t.,oldcol:=setcolor(colorSpec)
	local x1:=oBox:nTop,y1:=oBox:nleft, y2:=oBox:nRight
	local atype:=codb_info("CODB_COUNTER_TYPES")
	local mtype:={},ntype,actype,s,n,lctype,ictype,sss,pic,i,m
	local source:=.f.,fname,scr
	local tmp,odict

	//odict := codb_dict_reference()
	oDict:=oDep:dictionary()

	for i=1 to len(atype)
		aadd(mtype,atype[i][2])
	next

	dispbegin()
	@ x1,y1,oBox:nBottom,y2 box B_DOUBLE+" "
	x1++; y1++

	@ x1++,y1 say padc([Counter parameters],y2-y1)
	@ x1++,y1 say replicate("�",y2-y1) // utf-8: "─"
	if empty(data)
		@ x1++,y1 say [Can`t display information]
		dispend()
		return .f.
	endif
	@ x1++,y1 say [Identification..]+padr(data:id,y2-y1-16)
	@ x1++,y1 say [Name............]+data:name

	ntype:=ascan(atype,{|x|x[1]==data:type})
	actype:=iif(ntype==0,[Undefined],atype[ntype][2])
	@ x1++,y1 say [Type............]+actype
	@ x1++,y1 say [Depository......]+data:deposit

	@ x1++,y1 say [MAX value.......]+toString(data:max_value)
	@ x1++,y1 say [LAST value......]+toString(data:last_value)
	dispend()
	setcolor(oldcol)
return ret

************************************************************
static function ab_count_edit_card(oDep,data,oBox,colorSpec)
	local ret:=.t.,oldcol:=setcolor(colorSpec)
	local x1:=oBox:nTop,y1:=oBox:nleft, y2:=oBox:nRight
	local atype:=codb_info("CODB_COUNTER_TYPES")
	local mtype:={},ntype,actype,s,n,lctype,ictype,sss,pic,i,m
	local source:=.f.,fname,scr,getlist:={},x
	local gColor := set("ab_colors_dialog")
	local tmp,odict,aSc,nSc:=1,m1

	//odict := codb_dict_reference()
	oDict:=oDep:dictionary()

	for i=1 to len(atype)
		aadd(mtype,atype[i][2])
	next

	dispbegin()
	@ x1,y1,oBox:nBottom,y2 box B_DOUBLE+" "
	x1++; y1++

	@ x1++,y1 say padc([Counter parameters],y2-y1)
	@ x1++,y1 say replicate("�",y2-y1) // utf-8: "─"

	if empty(data)
		@ x1++,y1 say [Can`t edit information]
		dispend()
		return .f.
	endif

	ab_padrBody(data,codb_info("CODB_COUNTER_BODY"))

	if empty(data:id)
		@ x1++,y1 say [Identification..]+[Generation avtomatically ]
	else
		@ x1++,y1 say [Identification..]+padr(data:id,y2-y1-16)
	endif

	pic:="@S"+alltrim(str(y2-y1-17),3,0)
	@ x1  ,y1 say [Name............]
	@ x1++,y1+16 get data:name  picture pic color (gcolor)

	ntype:=ascan(atype,{|x|x[1]==data:type})
	@ x1  ,y1 say [Type............]
	@ x1++,y1+16, x1+6, y1+51 get ntype listbox mtype dropdown color (gcolor)

	@ x1++,y1 say [Depository......]+data:deposit

	@ x1  ,y1 say [MAX value.......]
	@ x1++,y1+16 get data:max_value picture pic color (gcolor)

	@ x1  ,y1 say [LAST value......]
	@ x1++,y1+16 get data:last_value picture pic color (gcolor)

	dispend()
	read

	data:type:=iif( ntype>0 .and. ntype<=len(mtype), atype[ntype][1], "")

	if lastkey()==27
		ret:=.f.
	endif
	setcolor(oldcol)
return ret

