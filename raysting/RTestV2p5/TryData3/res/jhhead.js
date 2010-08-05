function getjhtblst()
{
	return document.jhform.tbselect;
}
function addjhlist()
{
	var lstctl = getjhtblst();
	var lstdata="@@jhlst@@";
	var lstarray = lstdata.split(',');//seperate by ','
	lstctl.length = lstarray.length-1;//ignore the last ,
	var index=0;
	for (index=0;index< lstctl.length;index++){
		lstctl[index].value=lstarray[index];
		lstctl[index].text=lstarray[index];
	}
	lstctl.value = "@@jhsz@@";
}

function exitJhform()
{
	return true;
}
function loadJhform()
{
	var ptr = document.jhform;
	addjhlist();
	if(ptr.jhsel0){	ptr.jhsel0.value='@@jhsel0@@'};
	if(ptr.jhsel1){	ptr.jhsel1.value='@@jhsel1@@'};
	if(ptr.jhsel2){	ptr.jhsel2.value='@@jhsel2@@'};
	if(ptr.jhsel3){	ptr.jhsel3.value='@@jhsel3@@'};
	if(ptr.jhsel4){	ptr.jhsel4.value='@@jhsel4@@'};
	if(ptr.jhsel5){	ptr.jhsel5.value='@@jhsel5@@'};
	if(ptr.jhsel6){	ptr.jhsel6.value='@@jhsel6@@'};
	if(ptr.jhsel7){	ptr.jhsel7.value='@@jhsel7@@'};
	if(ptr.jhsel8){	ptr.jhsel8.value='@@jhsel8@@'};
	if(ptr.jhsel9){	ptr.jhsel9.value='@@jhsel9@@'};
	if(ptr.jhsel10){ptr.jhsel10.value='@@jhsel10@@'};	
	if (GetCookie('showjhtb') == 'true')
		jhpopUp();	
}


function jhsubmit(choice)
{
	//check the validaty first
	var tbsel=getjhtblst();
	
	
	if(choice == 'tbcancel')
		return;
	if(choice == 'tbadd')
	{	//jhadd
		var newname=window.prompt('请选择一个名字：');
		if(newname ==  null) return;
		if(newname ==  '') return;
		var lstdata="@@jhlst@@";
		if(lstdata.search(newname) >= 0){
			alert('名字('+newname+')已经存在');
			return;
		}
		document.jhform.cmdtype.value='jh_add';
		document.jhform.jhsz.value=newname;
		document.jhform.submit();
		return;
	}
	if(choice == 'tbdelete')
	{	//jhdelete
		if(tbsel.value == '')		return;
		if(window.confirm('你确定要删除设置-'+tbsel.value+'?') == true )
		{
			document.jhform.cmdtype.value='jh_delete';
			document.jhform.jhsz.value = tbsel.value;
			document.jhform.submit();
			return;
		}
	}
	if(choice == 'tbclose')
	{	//regupdate
		showpage('datahtm');
		return;
	}
	//regselect
	if(tbsel.value == '')		return;
	var lstdata="@@jhlst@@";
	if(lstdata.search(tbsel.value) >= 0)
	{
		document.jhform.cmdtype.value='jh_select';
		document.jhform.jhsz.value=document.jhform.tbselect.value;
		document.jhform.submit();
		return;
	}
}
function jhpopUp(distype)
{
    var tb1 = document.getElementById('jhitems');
    if(tb1.style.display == '')
    {
    	tb1.style.display = 'none';
    	SetCookie('showjhtb','false');
    }
    else
    {
	tb1.style.display = '';
	SetCookie('showjhtb','true');
    }	
}
document.writeln("<form name='jhform' method='POST' action='ToHtmlCtrl'>  ");
document.writeln("<table border='0' background='texture.jpg'><tr><td>");
document.writeln("<div id='jhtbdiv' style='position:absolute;width:100; color: #8C6D5A; zindex:0'>");
document.writeln("	 <input type='hidden' name='jhsz'>");
document.writeln("       <input type='hidden' name='cmdtype'>");
document.writeln("       <table id='jhitems' style='' border = '0'  width='100' cellspacing='0' >  ");
document.writeln("       <tr><td>");
document.writeln("<a href=# ><img name='jhImage2' onClick='jhpopUp()' value='tbcancel' src='imgorg/pinon.gif' width='20' height='18' ></a>");
document.writeln("      </td>");
document.writeln(" 	<td ><select name='tbselect' onchange=jhsubmit('tbselect')></select></td>  ");
document.writeln("		<td ><font color='green'>|</font></td>  ");
document.writeln("		<td ><button type='button' onclick=jhsubmit('tbadd')>保存设置为..</td>  ");
document.writeln("		<td ><button type='button' onclick=jhsubmit('tbdelete')>删除设置..</td>  ");
document.writeln("		<td ><font color='green'>|</font></td>  ");
document.writeln("		<td ><button type='button' onclick=jhsubmit('tbclose')>关闭..</td>  ");
document.writeln("		<td ><font color='green'>|</font></td>  ");
document.writeln("		</tr></table></div>");
document.writeln("<img name='jhImagel' onClick='jhpopUp()' src='imgorg/pinon.gif' width='20' height='18' >");
document.writeln("</td></tr><tr><td>");
